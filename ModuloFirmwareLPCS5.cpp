#include "ModuloFirmwareRPCS5.h"
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdio>
#include <algorithm>

// 🌍 Variable global del firmware
EstadoFirmware firmware;

// 📋 Lista de archivos esperados del firmware de PS5
static const char* ARCHIVOS_ESPERADOS[][2] = {
    {"eoot.bin", "Módulo de arranque y seguridad inicial"},
    {"sbl.bin", "Cargador secundario — valida y arranca el kernel"},
    {"kernel.img", "Núcleo principal del sistema operativo"},
    {"rtos.bin", "Sistema en tiempo real — control de hardware"},
    {"syslib.sprx", "Bibliotecas del sistema — funciones compartidas"},
    {"libc.sprx", "Biblioteca estándar de C"},
    {"libSceSystem.sprx", "Funciones del sistema de Sony"},
    {"libSceKernel.sprx", "API del kernel para juegos"},
    {"libSceVideoOut.sprx", "Salida de vídeo y pantalla"},
    {"libSceAudioOut.sprx", "Salida de audio"},
    {"libScePad.sprx", "Manejo de mandos DualSense"},
    {"libSceNet.sprx", "Conexión de red y online"},
    {"libSceNp.sprx", "PlayStation Network — cuentas y trofeos"},
    {"libSceUsbd.sprx", "Controlador USB"},
    {"libSceStorage.sprx", "Acceso a disco y almacenamiento"},
    {"version.txt", "Archivo con número de versión del firmware"},
    {nullptr, nullptr} // Fin de lista
};

// 🔍 Calcular CRC32 simple para verificar integridad
static uint32_t calcularCRC32(const void* datos, size_t tamano) {
    const uint32_t polinomio = 0xEDB88320;
    uint32_t crc = 0xFFFFFFFF;
    const uint8_t* bytes = static_cast<const uint8_t*>(datos);

    for (size_t i = 0; i < tamano; i++) {
        crc ^= bytes[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ ((crc & 1) ? polinomio : 0);
        }
    }
    return crc ^ 0xFFFFFFFF;
}

// 📖 Leer encabezado y extraer versión
static VersionFirmware leerVersionArchivo(const std::string& ruta) {
    VersionFirmware ver = {0, 0, 0, "", ""};
    FILE* archivo = fopen(ruta.c_str(), "r");
    if (!archivo) return ver;

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        if (strstr(linea, "version") || strstr(linea, "Version")) {
            char etiqueta[128];
            if (sscanf(linea, "%127s", etiqueta) >= 1) {
                ver.etiqueta = etiqueta;
                unsigned int maj, min, par;
                if (sscanf(etiqueta, "%u.%u-%u", &maj, &min, &par) >= 2) {
                    ver.mayor = maj;
                    ver.menor = min;
                    ver.parche = par;
                }
                break;
            }
        }
    }
    fclose(archivo);
    return ver;
}

// 🔍 Escanear y listar todos los archivos del firmware
std::vector<ArchivoFirmware> escanearArchivosFirmware(const std::string& ruta) {
    std::vector<ArchivoFirmware> lista;
    DIR* dir = opendir(ruta.c_str());
    if (!dir) return lista;

    std::vector<std::pair<std::string, std::string>> esperados;
    for (int i = 0; ARCHIVOS_ESPERADOS[i][0] != nullptr; i++) {
        esperados.emplace_back(ARCHIVOS_ESPERADOS[i][0], ARCHIVOS_ESPERADOS[i][1]);
    }

    struct dirent* entrada;
    while ((entrada = readdir(dir)) != nullptr) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
            continue;

        std::string nombre = entrada->d_name;
        std::string rutaCompleta = ruta + "/" + nombre;
        struct stat info;
        if (stat(rutaCompleta.c_str(), &info) != 0) continue;
        if (!S_ISREG(info.st_mode)) continue;

        ArchivoFirmware arch;
        arch.nombre = nombre;
        arch.ruta = rutaCompleta;
        arch.tamano = static_cast<uint64_t>(info.st_size);
        arch.presente = true;
        arch.valido = false;
        arch.esCifrado = false;

        for (const auto& par : esperados) {
            if (par.first == nombre) {
                arch.descripcion = par.second;
                break;
            }
        }
        if (arch.descripcion.empty()) {
            arch.descripcion = "Archivo del sistema";
        }

        lista.push_back(arch);
    }
    closedir(dir);

    firmware.archivosTotales = static_cast<uint32_t>(esperados.size());
    return lista;
}

// ✅ Verificar integridad de un archivo
bool verificarIntegridad(ArchivoFirmware& archivo) {
    if (!archivo.presente) return false;

    FILE* f = fopen(archivo.ruta.c_str(), "rb");
    if (!f) {
        archivo.valido = false;
        archivo.mensaje = "No se puede leer el archivo";
        return false;
    }

    const size_t TAMANO_PRUEBA = 65536;
    std::vector<uint8_t> buffer(TAMANO_PRUEBA);
    size_t leidos = fread(buffer.data(), 1, TAMANO_PRUEBA, f);
    fclose(f);

    if (leidos == 0) {
        archivo.valido = false;
        archivo.mensaje = "Archivo vacío o ilegible";
        return false;
    }

    archivo.hashCRC32 = calcularCRC32(buffer.data(), leidos);
    archivo.valido = true;
    return true;
}

// 🔓 Descifrar archivo
bool descifrarArchivo(ArchivoFirmware& archivo) {
    if (!archivo.esCifrado) return true;
    archivo.mensaje = "⚠️ Archivo cifrado — se requieren claves de descifrado";
    return false;
}

// 📋 VERIFICAR TODO EL FIRMWARE
EstadoFirmware verificarFirmware(const std::string& rutaCarpeta) {
    firmware = EstadoFirmware();
    firmware.rutaCarpeta = rutaCarpeta;

    struct stat info;
    if (stat(rutaCarpeta.c_str(), &info) != 0 || !S_ISDIR(info.st_mode)) {
        firmware.mensajeError = "Carpeta de firmware no encontrada";
        return firmware;
    }

    firmware.archivos = escanearArchivosFirmware(rutaCarpeta);

    if (firmware.archivos.empty()) {
        firmware.mensajeError = "No se encontraron archivos de firmware";
        return firmware;
    }

    firmware.archivosValidos = 0;
    for (auto& arch : firmware.archivos) {
        if (verificarIntegridad(arch)) {
            firmware.archivosValidos++;
            if (arch.nombre == "eoot.bin" || arch.nombre == "sbl.bin" ||
                arch.nombre == "kernel.img" || arch.nombre == "rtos.bin") {
                arch.esCifrado = true;
            }
        }
    }

    std::string rutaVersion = rutaCarpeta + "/version.txt";
    firmware.version = leerVersionArchivo(rutaVersion);

    firmware.cargado = (firmware.archivosValidos > 0);

    if (firmware.archivosValidos < firmware.archivosTotales / 2) {
        firmware.mensajeError = "Faltan archivos esenciales del firmware";
    }

    return firmware;
}

// 🚀 CARGAR E INICIALIZAR
bool cargarFirmware(const std::string& rutaCarpeta) {
    verificarFirmware(rutaCarpeta);
    if (!firmware.cargado) {
        return false;
    }
    firmware.inicializado = true;
    firmware.mensajeError.clear();
    return true;
}

// ⚙️ INICIALIZAR COMPONENTES
bool inicializarComponentes() {
    if (!firmware.inicializado) {
        firmware.mensajeError = "Firmware no cargado";
        return false;
    }
    return true;
}

// 🔄 REINICIAR SISTEMA
void reiniciarSistema() {
    firmware = EstadoFirmware();
}

// 📥 OBTENER VERSIÓN Y ESTADO
VersionFirmware obtenerVersionFirmware() {
    return firmware.version;
}

EstadoFirmware obtenerEstadoFirmware() {
    return firmware;
}
