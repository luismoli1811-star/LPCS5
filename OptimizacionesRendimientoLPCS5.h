#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <functional>

// ==============================================================
// ⚙️ NIVELES DE OPTIMIZACIÓN — El usuario elige equilibrio
// ==============================================================
enum class NivelOptimizacion : uint32_t {
    MINIMO = 0,    // 🐌 Sin optimizar — para depuración
    BASICO,        // 🐢 Lo esencial — compatible con todo
    EQUILIBRADO,   // ⚡ Recomendado — velocidad + calidad
    AGRESIVO,      // 🚀 Más rápido — puede tener pequeños errores
    MAXIMO         // 🚀🚀 Lo más rápido posible — sin límites
};

// ==============================================================
// 📊 TIPOS DE OPTIMIZACIÓN — ¿Qué se puede acelerar?
// ==============================================================
enum class TipoOptimizacion : uint32_t {
    NINGUNA = 0,

    // 🧠 PROCESADOR / CPU
    DESBORDAMIENTO_BUFER,
    ELIMINACION_SALTOS,
    PRECALCULO_CONSTANTES,
    INLINE_FUNCIONES,
    VECTORIZACION_SIMD,
    MULTIHILO,
    CACHE_INSTRUCCIONES,
    FUSION_BULES,

    // 🎮 GRÁFICOS / GPU
    REDUCCION_RESOLUCION,
    SALTO_CUADROS,
    COMPRESION_TEXTURAS,
    CACHE_TEXTURAS,
    FILTRO_BILINEAL,
    SINCRONIZACION_ADAPTATIVA,
    DESACTIVAR_EFECTOS,
    LIMPIEZA_PARCIAL,

    // 💾 MEMORIA
    PAGINACION_DEMANDADA,
    CACHE_DATOS,
    PRECARGA_ZONAS,
    COMPRESION_RAM,
    ELIMINACION_DUPLICADOS,

    // 🔊 AUDIO
    REDUCCION_CALIDAD_AUDIO,
    TAMAÑO_BUFFER_AUDIO,
    SALTO_MUESTRAS_SILENCIO,

    // 📱 ESPECÍFICAS DE ANDROID
    SUSPENSION_BACKGROUND,
    BLOQUEO_RELOJ_CPU,
    PRIORIDAD_HILOS,
    REDUCCION_EVENTOS_UI,
    USO_NEON
};

// ✅ ESTADO DE CADA OPTIMIZACIÓN
struct Optimizacion {
    TipoOptimizacion id;
    std::string nombre;
    std::string descripcion;
    bool activada;
    bool afectaCalidad;    // ⚠️ Si al activar baja la calidad visual/auditiva
    bool peligrosa;       // ⚠️ Si puede causar errores en algunos juegos
    int32_t gananciaPorcentaje; // ⚡ Estimación de cuánto acelera
};

// ⚙️ CONFIGURACIÓN COMPLETA DE RENDIMIENTO
struct ConfiguracionRendimiento {
    NivelOptimizacion nivel = NivelOptimizacion::EQUILIBRADO;

    // 🧠 CPU
    bool habilitarVectorizacion = true;
    bool habilitarMultihilo = true;
    bool precargarInstrucciones = true;
    bool fusionarBucle = true;
    uint32_t hilosTrabajo = 4; // Núcleos usados

    // 🎮 GRÁFICOS
    float escalaResolucion = 1.0f; // 1.0 = nativo, 0.75 = -25%, 1.5 = +50%
    uint32_t limiteFPS = 60;       // 30, 60, 120, 0 = sin límite
    bool saltoCuadros = false;     // Saltar cuadros lentos para fluidez
    bool comprimirTexturas = true;
    bool cacheTexturas = true;
    bool sincronizacionAdaptativa = true; // VSync dinámico
    bool desactivarSombras = false;
    bool desactivarPosprocesado = false;
    uint32_t nivelMipmap = 0;     // 0 = máximo, 1, 2 = menor detalle

    // 💾 MEMORIA
    bool habilitarCacheMemoria = true;
    bool precargarZonas = true;
    bool comprimirRAM = false;     // Menos memoria usada, un poco más lento
    uint32_t limiteCacheMB = 512;  // Límite para no saturar el celular

    // 🔊 AUDIO
    uint32_t frecuenciaAudio = 48000; // 44100, 48000, 32000, 22050
    uint32_t tamañoBufferAudio = 20;  // ms — mayor = menos cortes, más latencia
    bool reducirCanales = false;      // Estéreo → Mono

    // 📱 ANDROID / SISTEMA
    bool bloquearRelojCPU = true;     // Evitar que baje la velocidad
    bool altaPrioridad = true;        // Dar prioridad al emulador
    bool reducirAnimacionesUI = true; // Menos carga extra
    bool usarNEON = true;             // Aceleración ARM SIMD
    bool suspenderEnSegundoPlano = true; // Pausar al cambiar de app

    // 📊 ESTADÍSTICAS
    float rendimientoEstimado = 1.0f; // 1.0 = 100%, 0.5 = 50%
    bool monitoreoActivo = false;
    uint32_t umbralFPSBajo = 45;      // Activar optimizaciones si baja de aquí
};

// 📈 ESTADÍSTICAS EN TIEMPO REAL
struct EstadisticasRendimiento {
    float fpsActual = 0.0f;
    float fpsPromedio = 0.0f;
    float fpsMinimo = 999.0f;
    float fpsMaximo = 0.0f;

    float usoCPU_Porcentaje = 0.0f;
    float usoGPU_Porcentaje = 0.0f;
    float usoRAM_MB = 0.0f;
    float usoVRAM_MB = 0.0f;

    uint64_t tiempoFrame_Ms = 0;
    uint64_t tiempoGrafico_Ms = 0;
    uint64_t tiempoCPU_Ms = 0;
    uint64_t tiempoEspera_Ms = 0;

    bool bajoRendimiento = false;
    std::string mensajeOptimizacion;
};

// 🎯 FUNCIONES PRINCIPALES
// Inicializar el sistema de optimizaciones
bool inicializarOptimizaciones();

// Restaurar valores por defecto recomendados
void restablecerPorDefecto();

// 🎚️ APLICAR NIVEL PREESTABLECIDO — el usuario solo elige uno
void aplicarNivelOptimizacion(NivelOptimizacion nivel);

// 🔧 ACTIVAR/DESACTIVAR UNA OPTIMIZACIÓN INDIVIDUAL
bool setOptimizacion(TipoOptimizacion id, bool activar);
bool getOptimizacion(TipoOptimizacion id);

// 📊 MONITOREO — medir y ajustar en tiempo real
void actualizarEstadisticas(const EstadisticasRendimiento& stats);
void ajusteDinamicoRendimiento(); // ⚡ Ajusta solo si va lento
bool detectarBajoRendimiento();
std::string obtenerRecomendacion();

// 💾 GUARDAR / CARGAR
bool guardarConfiguracionRendimiento(const std::string& ruta);
bool cargarConfiguracionRendimiento(const std::string& ruta);

// 📋 LISTA DE TODAS LAS OPTIMIZACIONES
std::vector<Optimizacion> obtenerListaOptimizaciones();

// 📦 ACCESO GLOBAL
ConfiguracionRendimiento& obtenerConfiguracionRendimiento();
EstadisticasRendimiento& obtenerEstadisticasRendimiento();

// 🌍 Variable global
extern ConfiguracionRendimiento rendimiento;
extern EstadisticasRendimiento statsRendimiento;
