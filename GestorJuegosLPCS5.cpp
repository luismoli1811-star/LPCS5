#include "GestorJuegosRPCS5.h"
#include "GestorCarpetas RPCS5.h"
#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <cstring>
#include <climits>  // Para INT_MAX

// 🌍 Variables globales
std::vector<JuegoInfo> listaJuegos;
EstadoCarga estadoCarga;
JuegoInfo juegoActual;

// 📋 Convertir formato a texto
std::string formatoAString(FormatoJuego formato) {
    switch (formato) {
        case FormatoJuego::PKG: return "PKG";
        case FormatoJuego::ISO: return "ISO";
        case FormatoJuego::CARPETA: return "Carpeta";
        case FormatoJuego::PS5: return "PS5";
        default: return "Desconocido";
    }
}

// 🔍 Detectar el formato según la extensión
FormatoJuego detectarFormato(const std::string& ruta) {
    std::string ext;
    size_t punto = ruta.find_last_of('.');
    if (punto != std::string::npos) {
        ext = ruta.substr(punto + 1);
        std::transform(ext.begin(), ext.end(), ext.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    }

    if (ext == "pkg") return FormatoJuego::PKG;
    if (ext == "iso" || ext == "bin") return FormatoJuego::ISO;
    if (ext == "ps5") return FormatoJuego::PS5;

    // Si no tiene extensión → podría ser carpeta de juego extraído
    struct stat info;
    if (stat(ruta.c_str(), &info) == 0 && S_ISDIR(info.st_mode)) {
        std::string rutaEjec = ruta + "/eboot.bin";
        if (stat(rutaEjec.c_str(), &info) == 0) {
            return FormatoJuego::CARPETA;
        }
    }

    return FormatoJuego::DESCONOCIDO;
}

// 🖼️ Buscar la portada del juego en la carpeta de Media
std::string buscarPortada(const JuegoInfo& juego) {
    struct stat info;
    std::string rutaPortada = rutas.portadas + juego.id + ".png";
    if (stat(rutaPortada.c_str(), &info) == 0) return rutaPortada;

    rutaPortada = rutas.portadas + juego.nombre + ".png";
    if (stat(rutaPortada.c_str(), &info) == 0) return rutaPortada;

    rutaPortada = rutas.portadas + juego.id + ".jpg";
    if (stat(rutaPortada.c_str(), &info) == 0) return rutaPortada;

    return "";
}

// 📊 Calcular tamaño total de un archivo o carpeta
static uint64_t calcularTamano(const std::string& ruta) {
    struct stat info;
    if (stat(ruta.c_str(), &info) != 0) return 0;
    if (!S_ISDIR(info.st_mode)) return static_cast<uint64_t>(info.st_size);

    uint64_t total = 0;
    DIR* dir = opendir(ruta.c_str());
    if (!dir) return 0;

    struct dirent* entrada;
    while ((entrada = readdir(dir)) != nullptr) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
            continue;
        total += calcularTamano(ruta + "/" + entrada->d_name);
    }
    closedir(dir);
    return total;
}

// 📖 Leer información del juego desde el archivo
bool leerInfoJuego(JuegoInfo& juego) {
    juego.formato = detectarFormato(juego.ruta);
    if (juego.formato == FormatoJuego::DESCONOCIDO) {
        juego.esValido = false;
        return false;
    }

    // Extraer nombre desde la ruta
    size_t ultimaBarra = juego.ruta.find_last_of('/');
    std::string nombreArchivo = (ultimaBarra != std::string::npos)
        ? juego.ruta.substr(ultimaBarra + 1)
        : juego.ruta;

    // Quitar extensión
    size_t punto = nombreArchivo.find_last_of('.');
    if (punto != std::string::npos)
        juego.nombre = nombreArchivo.substr(0, punto);
    else
        juego.nombre = nombreArchivo;

    juego.titulo = juego.nombre;
    juego.tamano = calcularTamano(juego.ruta);
    juego.portadaRuta = buscarPortada(juego);
    juego.esValido = true;

    juego.id = "DESCONOCIDO";
    juego.version = "1.00";
    juego.region = "Desconocida";
    juego.estaCifrado = true;

    return true;
}

// 🔍 Escanear toda la carpeta de juegos
std::vector<JuegoInfo> escanearCarpetaJuegos(const std::string& ruta) {
    std::vector<JuegoInfo> resultados;
    DIR* dir = opendir(ruta.c_str());
    if (!dir) return resultados;

    estadoCarga.cargando = true;
    estadoCarga.progreso = 0;
    estadoCarga.mensaje = "Escaneando juegos...";
    estadoCarga.cancelado = false;

    struct dirent* entrada;
    int contador = 0;

    while ((entrada = readdir(dir)) != nullptr) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
            continue;
        if (estadoCarga.cancelado) break;

        std::string rutaCompleta = ruta + entrada->d_name;

        JuegoInfo juego;
        juego.ruta = rutaCompleta;

        if (leerInfoJuego(juego) && juego.esValido) {
            resultados.push_back(juego);
        }

        contador++;
        // ✅ Progreso seguro sin división por cero ni desbordamiento
        estadoCarga.progreso = std::min(contador * 100 / std::max(contador, 10), 99);
    }

    closedir(dir);
    estadoCarga.progreso = 100;
    estadoCarga.cargando = false;
    estadoCarga.mensaje = "Completado";

    listaJuegos = resultados;
    return resultados;
}

// 🚀 Preparar archivos antes de iniciar la emulación
bool prepararArchivosJuego(const JuegoInfo& juego) {
    estadoCarga.cargando = true;
    estadoCarga.progreso = 0;
    estadoCarga.mensaje = "Preparando archivos...";
    estadoCarga.cancelado = false;

    struct stat info;
    if (stat(juego.ruta.c_str(), &info) != 0) {
        estadoCarga.mensaje = "Error: Archivo no encontrado";
        estadoCarga.cargando = false;
        return false;
    }

    if (juego.estaCifrado) {
        estadoCarga.mensaje = "⚠️ Juego cifrado — se requiere clave de descifrado";
    }

    estadoCarga.progreso = 50;

    // Crear carpeta de trabajo para este juego
    std::string carpetaJuego = rutas.temporal + "/" + juego.id + "/";
    mkdir(carpetaJuego.c_str(), 0755);

    estadoCarga.progreso = 100;
    estadoCarga.cargando = false;
    estadoCarga.mensaje = "Listo para iniciar";

    return true;
}

// 🎮 Cargar el juego — llamado al darle "Jugar"
bool cargarJuego(const JuegoInfo& juego) {
    if (!juego.esValido) return false;

    juegoActual = juego;

    if (!prepararArchivosJuego(juego)) {
        return false;
    }

    return true;
}
