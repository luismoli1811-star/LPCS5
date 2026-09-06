#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>

// ==============================================================
// 📂 UBICACIÓN DE ARCHIVOS — Rutas adaptables a Android
// ==============================================================
namespace RutasGuardados {
    // Rutas base — configurables según plataforma
    extern std::string directorioBase;       // Carpeta principal de guardados
    extern std::string directorioManuales;   // Ranuras normales del juego
    extern std::string directorioEstados;    // Estados rápidos (savestates)
    extern std::string directorioCapturas;   // Capturas de pantalla
    extern std::string directorioConfiguracion; // Configuración por juego

    // Extensiones de archivo
    constexpr const char* EXT_GUARDADO_MANUAL = ".sav";    // Formato nativo del juego
    constexpr const char* EXT_ESTADO_RAPIDO = ".state";   // Estado completo del emulador
    constexpr const char* EXT_CAPTURA = ".png";           // Captura de pantalla
    constexpr const char* EXT_METADATA = ".json";         // Información adicional
}

// ==============================================================
// 📋 INFORMACIÓN DE UNA RANURA DE GUARDADO
// ==============================================================
struct RanuraGuardado {
    uint32_t indice;                  // Número de ranura (0-9 = manuales, 10+ = rápidos)
    std::string rutaArchivo;          // Ruta completa del archivo
    std::string nombreJuego;          // Nombre del juego
    std::string nombreUsuario;        // Nombre personalizado por el usuario
    uint64_t tamañoBytes;             // Peso del archivo
    std::chrono::system_clock::time_point fechaCreacion;
    std::chrono::system_clock::time_point fechaUltimaModificacion;
    uint32_t tiempoJugadoMinutos;    // Tiempo total de juego en esta partida
    bool esEstadoRapido;              // true = savestate, false = guardado manual
    bool existe;                      // true = hay un guardado en esta ranura
    bool valido;                      // true = el archivo no está corrupto
    std::string rutaCaptura;         // Captura de pantalla asociada (si la hay)
    std::string descripcion;         // Notas del usuario o información automática
};

// ==============================================================
// 📊 ESTADÍSTICAS DE GUARDADOS POR JUEGO
// ==============================================================
struct EstadisticasGuardados {
    uint32_t totalGuardadosManuales = 0;
    uint32_t totalEstadosRapidos = 0;
    uint64_t espacioTotalUsadoMB = 0;
    uint32_t tiempoTotalJugadoHoras = 0;
    std::chrono::system_clock::time_point ultimaPartida;
};

// ==============================================================
// 🔧 CONFIGURACIÓN DEL SISTEMA DE GUARDADOS
// ==============================================================
struct ConfiguracionGuardados {
    bool autoGuardarHabilitado = true;          // 💾 Autoguardado cada X minutos
    uint32_t autoGuardarIntervaloMinutos = 5;  // Cada cuánto guardar automáticamente
    bool autoGuardarAlSalir = true;             // 💾 Guardar al cerrar el juego
    bool cargarUltimoAlIniciar = false;        // 📂 Cargar el último al abrir
    bool incluirCapturaEnEstado = true;        // 📸 Guardar captura con el estado
    bool comprimirEstados = true;              // 📦 Comprimir para ahorrar espacio
    uint32_t maxEstadosRapidos = 20;           // Límite de estados rápidos por juego
    bool guardarEnNube = false;                 // ☁️ Sincronización en nube (futuro)
};

// ==============================================================
// 🎯 FUNCIONES PRINCIPALES
// ==============================================================

// 🚀 INICIALIZACIÓN
// Preparar rutas, crear carpetas si no existen
bool inicializarSistemaGuardados(const std::string& rutaBase);

// 🛑 CERRAR
// Guardar todo pendiente y liberar recursos
void apagarSistemaGuardados();

// 💾 GUARDAR
// Guardado manual — ranura 0-9
bool guardarEnRanura(uint32_t indiceRanura, const std::string& nombreJuego,
                     const std::string& descripcion = "");

// ⚡ GUARDADO RÁPIDO (Savestate) — estado completo del emulador
bool guardarEstadoRapido(uint32_t indiceRanura, const std::string& nombreJuego,
                         bool incluirCaptura = true);

// 🤖 AUTO-GUARDADO — llamado automáticamente por el sistema
bool autoGuardar(const std::string& nombreJuego);

// 📂 CARGAR
// Cargar desde ranura de guardado manual
bool cargarDeRanura(uint32_t indiceRanura, RanuraGuardado* infoCargada = nullptr);

// ⚡ CARGAR ESTADO RÁPIDO
bool cargarEstadoRapido(uint32_t indiceRanura, RanuraGuardado* infoCargada = nullptr);

// 📋 LISTAR
// Obtener lista de todos los guardados de un juego
std::vector<RanuraGuardado> listarGuardados(const std::string& nombreJuego);

// Obtener lista de ranuras manuales (0-9)
std::vector<RanuraGuardado> listarRanurasManuales(const std::string& nombreJuego);

// Obtener lista de estados rápidos
std::vector<RanuraGuardado> listarEstadosRapidos(const std::string& nombreJuego);

// 🔍 INFORMACIÓN
// Obtener información detallada de una ranura específica
RanuraGuardado obtenerInfoRanura(uint32_t indiceRanura, bool esEstadoRapido,
                                 const std::string& nombreJuego);

// Verificar si hay un guardado en esa ranura
bool existeGuardado(uint32_t indiceRanura, bool esEstadoRapido,
                   const std::string& nombreJuego);

// 🗑️ ELIMINAR
// Borrar un guardado específico
bool eliminarGuardado(uint32_t indiceRanura, bool esEstadoRapido,
                     const std::string& nombreJuego);

// Borrar TODOS los guardados de un juego
bool eliminarTodosGuardados(const std::string& nombreJuego);

// ✏️ EDITAR
// Cambiar nombre o descripción de un guardado
bool renombrarGuardado(uint32_t indiceRanura, bool esEstadoRapido,
                      const std::string& nombreJuego,
                      const std::string& nuevoNombre,
                      const std::string& nuevaDescripcion = "");

// 📊 ESTADÍSTICAS
// Obtener uso de espacio y tiempo jugado
EstadisticasGuardados obtenerEstadisticas(const std::string& nombreJuego);

// 💾 CONFIGURACIÓN
// Leer y modificar la configuración del sistema
ConfiguracionGuardados obtenerConfiguracionGuardados();
void setConfiguracionGuardados(const ConfiguracionGuardados& config);

// ☁️ SINCRONIZACIÓN (preparación para futuro)
bool exportarGuardado(uint32_t indiceRanura, bool esEstadoRapido,
                     const std::string& nombreJuego,
                     const std::string& rutaDestino);
bool importarGuardado(const std::string& rutaOrigen,
                     uint32_t indiceDestino, bool esEstadoRapido,
                     const std::string& nombreJuego);

// 🌍 ACCESO GLOBAL
extern ConfiguracionGuardados configGuardados;
