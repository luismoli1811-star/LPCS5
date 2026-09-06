#include "SistemaGuardadosRPCS5.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <zlib.h> // Para compresión de estados
#include <sys/stat.h>

// 🌍 RUTAS GLOBALES
namespace RutasGuardados {
    std::string directorioBase;
    std::string directorioManuales;
    std::string directorioEstados;
    std::string directorioCapturas;
    std::string directorioConfiguracion;
}

// ⚙️ CONFIGURACIÓN GLOBAL
ConfiguracionGuardados configGuardados;

// ==============================================================
// 🚀 INICIALIZAR — Crear carpetas y rutas
// ==============================================================
bool inicializarSistemaGuardados(const std::string& rutaBase) {
    RutasGuardados::directorioBase = rutaBase;

    // Definir subcarpetas
    RutasGuardados::directorioManuales = rutaBase + "/guardados/";
    RutasGuardados::directorioEstados = rutaBase + "/estados_rapidos/";
    RutasGuardados::directorioCapturas = rutaBase + "/capturas/";
    RutasGuardados::directorioConfiguracion = rutaBase + "/config_juegos/";

    // 📂 Crear todas las carpetas si no existen
    auto crearCarpeta = [](const std::string& ruta) {
        struct stat info;
        if (stat(ruta.c_str(), &info) != 0) {
            // No existe → crear
            return mkdir(ruta.c_str(), 0755) == 0;
        }
        return (info.st_mode & S_IFDIR) != 0;
    };

    bool ok = true;
    ok &= crearCarpeta(RutasGuardados::directorioManuales);
    ok &= crearCarpeta(RutasGuardados::directorioEstados);
    ok &= crearCarpeta(RutasGuardados::directorioCapturas);
    ok &= crearCarpeta(RutasGuardados::directorioConfiguracion);

    // Cargar configuración guardada
    std::string rutaConfig = RutasGuardados::directorioConfiguracion + "config_guardados.json";
    std::ifstream archivoConfig(rutaConfig);
    if (archivoConfig.is_open()) {
        // ⚠️ En versión completa: parsear JSON y cargar config
        archivoConfig.close();
    }

    return ok;
}

// ==============================================================
// 🛑 APAGAR — Guardar configuración final
// ==============================================================
void apagarSistemaGuardados() {
    // Guardar configuración actual
    std::string rutaConfig = RutasGuardados::directorioConfiguracion + "config_guardados.json";
    std::ofstream archivoConfig(rutaConfig);
    if (archivoConfig.is_open()) {
        archivoConfig << "{\n";
        archivoConfig << "  \"autoGuardar\": " << (configGuardados.autoGuardarHabilitado ? "true" : "false") << ",\n";
        archivoConfig << "  \"intervaloMinutos\": " << configGuardados.autoGuardarIntervaloMinutos << ",\n";
        archivoConfig << "  \"autoGuardarAlSalir\": " << (configGuardados.autoGuardarAlSalir ? "true" : "false") << ",\n";
        archivoConfig << "  \"maxEstadosRapidos\": " << configGuardados.maxEstadosRapidos << "\n";
        archivoConfig << "}\n";
        archivoConfig.close();
    }
}

// ==============================================================
// 📂 AYUDAS INTERNAS — Construir rutas de archivos
// ==============================================================
static std::string obtenerRutaArchivo(uint32_t indice, bool esEstadoRapido,
                                       const std::string& nombreJuego) {
    // Limpiar nombre de juego para nombre de archivo
    std::string nombreLimpio = nombreJuego;
    std::replace(nombreLimpio.begin(), nombreLimpio.end(), ' ', '_');

    if (esEstadoRapido) {
        return RutasGuardados::directorioEstados + nombreLimpio + "_" +
               std::to_string(indice) + RutasGuardados::EXT_ESTADO_RAPIDO;
    } else {
        return RutasGuardados::directorioManuales + nombreLimpio + "_" +
               std::to_string(indice) + RutasGuardados::EXT_GUARDADO_MANUAL;
    }
}

static std::string obtenerRutaCaptura(uint32_t indice, bool esEstadoRapido,
                                       const std::string& nombreJuego) {
    std::string nombreLimpio = nombreJuego;
    std::replace(nombreLimpio.begin(), nombreLimpio.end(), ' ', '_');
    return RutasGuardados::directorioCapturas + nombreLimpio + "_" +
           std::to_string(indice) + RutasGuardados::EXT_CAPTURA;
}

// ==============================================================
// 💾 GUARDADO MANUAL — Ranura 0-9
// ==============================================================
bool guardarEnRanura(uint32_t indiceRanura, const std::string& nombreJuego,
                     const std::string& descripcion) {
    if (indiceRanura > 9) return false; // Ranuras manuales 0-9 solamente

    std::string rutaArchivo = obtenerRutaArchivo(indiceRanura, false, nombreJuego);

    // ⚠️ En versión completa: escribir el estado del juego
    // Aquí se simula la escritura del archivo de guardado
    std::ofstream archivo(rutaArchivo, std::ios::binary);
    if (!archivo.is_open()) return false;

    // Escribir encabezado de metadatos
    std::string fecha = std::to_string(std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()));
    std::string metadatos = "RPCS5_SAVE\nVERSION:1.0\nJUEGO:" + nombreJuego +
                            "\nFECHA:" + fecha + "\nDESCRIPCION:" + descripcion + "\nEND_HEADER\n";

    archivo.write(metadatos.c_str(), metadatos.size());

    // ⚠️ Aquí iría el contenido real del guardado del juego
    // Por ahora escribimos un marcador de datos de ejemplo
    const char* datosEjemplo = "[DATOS_DE_GUARDADO_DEL_JUEGO]";
    archivo.write(datosEjemplo, 30);

    archivo.close();
    return true;
}

// ==============================================================
// ⚡ GUARDADO RÁPIDO — Estado completo del emulador
// ==============================================================
bool guardarEstadoRapido(uint32_t indiceRanura, const std::string& nombreJuego,
                         bool incluirCaptura) {
    if (indiceRanura >= configGuardados.maxEstadosRapidos) return false;

    std::string rutaArchivo = obtenerRutaArchivo(indiceRanura, true, nombreJuego);

    // ⚠️ En versión completa: capturar TODO el estado del emulador
    // Registrar memoria, registros, gráficos, audio, todo
    std::ofstream archivo(rutaArchivo, std::ios::binary);
    if (!archivo.is_open()) return false;

    // Encabezado de estado rápido
    std::string fecha = std::to_string(std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()));
    std::string encabezado = "RPCS5_STATE\nVERSION:1.0\nJUEGO:" + nombreJuego +
                             "\nFECHA:" + fecha + "\nCOMPRIMIDO:" +
                             std::to_string(configGuardados.comprimirEstados ? 1 : 0) + "\nEND_HEADER\n";

    archivo.write(encabezado.c_str(), encabezado.size());

    // ⚠️ Aquí iría el volcado completo de memoria y estado del sistema
    // Si está comprimido, se usaría zlib para reducir el tamaño
    const char* estadoEjemplo = "[ESTADO_COMPLETO_DEL_EMULADOR]";
    archivo.write(estadoEjemplo, 32);

    archivo.close();

    // 📸 Guardar captura de pantalla si se pidió
    if (incluirCaptura && configGuardados.incluirCapturaEnEstado) {
        std::string rutaCaptura = obtenerRutaCaptura(indiceRanura, true, nombreJuego);
        // ⚠️ En versión completa: llamar al sistema de captura de pantalla
        // y guardar la imagen en rutaCaptura
    }

    return true;
}

// ==============================================================
// 🤖 AUTO-GUARDADO — Llamado por el sistema cada X minutos
// ==============================================================
bool autoGuardar(const std::string& nombreJuego) {
    if (!configGuardados.autoGuardarHabilitado) return false;

    // Usamos la ranura 9 para auto-guardado
    return guardarEnRanura(9, nombreJuego, "Auto-guardado automático");
}

// ==============================================================
// 📂 CARGAR GUARDADO MANUAL
// ==============================================================
bool cargarDeRanura(uint32_t indiceRanura, RanuraGuardado* infoCargada) {
    if (indiceRanura > 9) return false;

    // ⚠️ Necesitamos saber el nombre del juego — en versión completa se pasa
    // Aquí se asume que el nombre se obtiene de otra forma
    std::string rutaArchivo = obtenerRutaArchivo(indiceRanura, false, "juego_desconocido");

    std::ifstream archivo(rutaArchivo, std::ios::binary);
    if (!archivo.is_open()) return false;

    // ⚠️ En versión completa: leer encabezado y restaurar estado del juego
    if (infoCargada) {
        infoCargada->existe = true;
        infoCargada->valido = true;
        infoCargada->indice = indiceRanura;
        infoCargada->esEstadoRapido = false;
    }

    archivo.close();
    return true;
}

// ==============================================================
// ⚡ CARGAR ESTADO RÁPIDO
// ==============================================================
bool cargarEstadoRapido(uint32_t indiceRanura, RanuraGuardado* infoCargada) {
    std::string rutaArchivo = obtenerRutaArchivo(indiceRanura, true, "juego_desconocido");

    std::ifstream archivo(rutaArchivo, std::ios::binary);
    if (!archivo.is_open()) return false;

    // ⚠️ En versión completa: leer encabezado, descomprimir si es necesario,
    // restaurar TODO el estado del emulador (memoria, registros, GPU, audio...)
    if (infoCargada) {
        infoCargada->existe = true;
        infoCargada->valido = true;
        infoCargada->indice = indiceRanura;
        infoCargada->esEstadoRapido = true;
    }

    archivo.close();
    return true;
}

// ==============================================================
// 🔍 VERIFICAR SI EXISTE GUARDADO
// ==============================================================
bool existeGuardado(uint32_t indiceRanura, bool esEstadoRapido,
                   const std::string& nombreJuego) {
    std::string ruta = obtenerRutaArchivo(indiceRanura, esEstadoRapido, nombreJuego);
    struct stat buffer;
    return stat(ruta.c_str(), &buffer) == 0;
}

// ==============================================================
// 📋 OBTENER INFORMACIÓN DE UNA RANURA
// ==============================================================
RanuraGuardado obtenerInfoRanura(uint32_t indiceRanura, bool esEstadoRapido,
                                  const std::string& nombreJuego) {
    RanuraGuardado info;
    info.indice = indiceRanura;
    info.esEstadoRapido = esEstadoRapido;
    info.existe = existeGuardado(indiceRanura, esEstadoRapido, nombreJuego);
    info.valido = false;

    if (!info.existe) return info;

    info.rutaArchivo = obtenerRutaArchivo(indiceRanura, esEstadoRapido, nombreJuego);
    info.nombreJuego = nombreJuego;

    // Obtener tamaño y fecha
    struct stat archivoInfo;
    if (stat(info.rutaArchivo.c_str(), &archivoInfo) == 0) {
        info.tamañoBytes = archivoInfo.st_size;
        info.fechaCreacion = std::chrono::system_clock::from_time_t(archivoInfo.st_ctime);
        info.fechaUltimaModificacion = std::chrono::system_clock::from_time_t(archivoInfo.st_mtime);
        info.valido = true;
    }

    // Ruta de captura
    info.rutaCaptura = obtenerRutaCaptura(indiceRanura, esEstadoRapido, nombreJuego);
    struct stat capturaInfo;
    if (stat(info.rutaCaptura.c_str(), &capturaInfo) != 0) {
        info.rutaCaptura.clear(); // No hay captura
    }

    return info;
}

// ==============================================================
// 📋 LISTAR TODOS LOS GUARDADOS DE UN JUEGO
// ==============================================================
std::vector<RanuraGuardado> listarGuardados(const std::string& nombreJuego) {
    std::vector<RanuraGuardado> lista;

    // Ranuras manuales 0-9
    for (uint32_t i = 0; i <= 9; i++) {
        lista.push_back(obtenerInfoRanura(i, false, nombreJuego));
    }

    // Estados rápidos
    for (uint32_t i = 0; i < configGuardados.maxEstadosRapidos; i++) {
        lista.push_back(obtenerInfoRanura(i, true, nombreJuego));
    }

    return lista;
}

std::vector<RanuraGuardado> listarRanurasManuales(const std::string& nombreJuego) {
    std::vector<RanuraGuardado> lista;
    for (uint32_t i = 0; i <= 9; i++) {
        lista.push_back(obtenerInfoRanura(i, false, nombreJuego));
    }
    return lista;
}

std::vector<RanuraGuardado> listarEstadosRapidos(const std::string& nombreJuego) {
    std::vector<RanuraGuardado> lista;
    for (uint32_t i = 0; i < configGuardados.maxEstadosRapidos; i++) {
        lista.push_back(obtenerInfoRanura(i, true, nombreJuego));
    }
    return lista;
}

// ==============================================================
// 🗑️ ELIMINAR GUARDADO
// ==============================================================
bool eliminarGuardado(uint32_t indiceRanura, bool esEstadoRapido,
                      const std::string& nombreJuego) {
    std::string rutaArchivo = obtenerRutaArchivo(indiceRanura, esEstadoRapido, nombreJuego);
    std::string rutaCaptura = obtenerRutaCaptura(indiceRanura, esEstadoRapido, nombreJuego);

    bool okArchivo = std::remove(rutaArchivo.c_str()) == 0;
    bool okCaptura = true;
    struct stat capturaInfo;
    if (stat(rutaCaptura.c_str(), &capturaInfo) == 0) {
        okCaptura = std::remove(rutaCaptura.c_str()) == 0;
    }

    return okArchivo && okCaptura;
}

bool eliminarTodosGuardados(const std::string& nombreJuego) {
    bool ok = true;
    for (uint32_t i = 0; i <= 9; i++) {
        ok &= eliminarGuardado(i, false, nombreJuego);
    }
    for (uint32_t i = 0; i < configGuardados.maxEstadosRapidos; i++) {
        ok &= eliminarGuardado(i, true, nombreJuego);
    }
    return ok;
}

// ==============================================================
// ✏️ RENOMBRAR GUARDADO
// ==============================================================
bool renombrarGuardado(uint32_t indiceRanura, bool esEstadoRapido,
                       const std::string& nombreJuego,
                       const std::string& nuevoNombre,
                       const std::string& nuevaDescripcion) {
    // ⚠️ En versión completa: actualizar metadatos dentro del archivo
    // Por ahora se considera exitoso
    return true;
}

// ==============================================================
// 📊 ESTADÍSTICAS
// ==============================================================
EstadisticasGuardados obtenerEstadisticas(const std::string& nombreJuego) {
    EstadisticasGuardados stats;
    auto lista = listarGuardados(nombreJuego);

    for (const auto& ranura : lista) {
        if (!ranura.existe) continue;
        if (ranura.esEstadoRapido)
            stats.totalEstadosRapidos++;
        else
            stats.totalGuardadosManuales++;
        stats.espacioTotalUsadoMB += ranura.tamañoBytes / (1024.0 * 1024.0);
    }

    return stats;
}

// ==============================================================
// ⚙️ CONFIGURACIÓN
// ==============================================================
ConfiguracionGuardados obtenerConfiguracionGuardados() {
    return configGuardados;
}

void setConfiguracionGuardados(const ConfiguracionGuardados& config) {
    configGuardados = config;
}

// ==============================================================
// ☁️ EXPORTAR / IMPORTAR
// ==============================================================
bool exportarGuardado(uint32_t indiceRanura, bool esEstadoRapido,
                      const std::string& nombreJuego,
                      const std::string& rutaDestino) {
    std::string rutaOrigen = obtenerRutaArchivo(indiceRanura, esEstadoRapido, nombreJuego);
    std::ifstream origen(rutaOrigen, std::ios::binary);
    if (!origen.is_open()) return false;

    std::ofstream destino(rutaDestino, std::ios::binary);
    if (!destino.is_open()) return false;

    destino << origen.rdbuf();
    return true;
}

bool importarGuardado(const std::string& rutaOrigen,
                      uint32_t indiceDestino, bool esEstadoRapido,
                      const std::string& nombreJuego) {
    std::string rutaDestino = obtenerRutaArchivo(indiceDestino, esEstadoRapido, nombreJuego);
    std::ifstream origen(rutaOrigen, std::ios::binary);
    if (!origen.is_open()) return false;

    std::ofstream destino(rutaDestino, std::ios::binary);
    if (!destino.is_open()) return false;

    destino << origen.rdbuf();
    return true;
}
