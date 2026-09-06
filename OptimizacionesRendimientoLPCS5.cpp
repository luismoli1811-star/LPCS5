#include "OptimizacionesRendimientoRPCS5.h"
#include <cstring>
#include <algorithm>
#include <cmath>
#include <fstream>

// 🌍 Variables globales
ConfiguracionRendimiento rendimiento;
EstadisticasRendimiento statsRendimiento;

// 📋 LISTA MAESTRA DE TODAS LAS OPTIMIZACIONES
static const std::vector<Optimizacion> listaOptimizaciones = {
    // 🧠 CPU
    {TipoOptimizacion::VECTORIZACION_SIMD, "Vectorización NEON", "Usa instrucciones rápidas del procesador ARM", true, false, false, 30},
    {TipoOptimizacion::MULTIHILO, "Multihilo", "Divide el trabajo entre varios núcleos", true, false, false, 40},
    {TipoOptimizacion::INLINE_FUNCIONES, "Inlining", "Integra funciones pequeñas para evitar saltos", true, false, false, 10},
    {TipoOptimizacion::CACHE_INSTRUCCIONES, "Caché de instrucciones", "Mantiene código frecuente en memoria rápida", true, false, false, 15},
    {TipoOptimizacion::FUSION_BULES, "Fusión de bucles", "Une bucles para reducir sobrecarga", true, false, false, 12},

    // 🎮 GRÁFICOS
    {TipoOptimizacion::REDUCCION_RESOLUCION, "Escala de resolución", "Reduce resolución interna", false, true, false, 50},
    {TipoOptimizacion::SALTO_CUADROS, "Salto de cuadros", "Salta cuadros lentos para mantener fluidez", false, true, false, 35},
    {TipoOptimizacion::COMPRESION_TEXTURAS, "Compresión de texturas", "Reduce uso de VRAM", true, false, false, 20},
    {TipoOptimizacion::CACHE_TEXTURAS, "Caché de texturas", "Reutiliza texturas ya cargadas", true, false, false, 25},
    {TipoOptimizacion::DESACTIVAR_EFECTOS, "Desactivar efectos", "Quita sombras y posprocesado", false, true, false, 45},
    {TipoOptimizacion::SINCRONIZACION_ADAPTATIVA, "VSync adaptativo", "Sincroniza solo cuando es posible", true, false, false, 10},

    // 💾 MEMORIA
    {TipoOptimizacion::CACHE_DATOS, "Caché de datos", "Almacena acceso frecuente en memoria rápida", true, false, false, 18},
    {TipoOptimizacion::PRECARGA_ZONAS, "Precarga de zonas", "Carga datos antes de que se necesiten", true, false, false, 15},
    {TipoOptimizacion::COMPRESION_RAM, "Compresión de RAM", "Comprime memoria para liberar espacio", false, false, false, -5},

    // 🔊 AUDIO
    {TipoOptimizacion::REDUCCION_CALIDAD_AUDIO, "Calidad de audio", "Reduce frecuencia de muestreo", false, true, false, 8},
    {TipoOptimizacion::SALTO_MUESTRAS_SILENCIO, "Saltar silencio", "No procesar muestras de silencio", true, false, false, 5},

    // 📱 ANDROID
    {TipoOptimizacion::BLOQUEO_RELOJ_CPU, "Bloquear reloj CPU", "Evita que el sistema baje la velocidad", true, false, false, 20},
    {TipoOptimizacion::PRIORIDAD_HILOS, "Alta prioridad", "Dar prioridad al emulador", true, false, false, 15},
    {TipoOptimizacion::USO_NEON, "Aceleración NEON", "Instrucciones SIMD nativas de ARM", true, false, false, 35},
};

// ==============================================================
// 🚀 INICIALIZAR — Cargar valores por defecto
// ==============================================================
bool inicializarOptimizaciones() {
    std::memset(&rendimiento, 0, sizeof(ConfiguracionRendimiento));
    std::memset(&statsRendimiento, 0, sizeof(EstadisticasRendimiento));

    // Valores por defecto — EQUILIBRADO
    rendimiento.nivel = NivelOptimizacion::EQUILIBRADO;
    rendimiento.habilitarVectorizacion = true;
    rendimiento.habilitarMultihilo = true;
    rendimiento.precargarInstrucciones = true;
    rendimiento.fusionarBucle = true;
    rendimiento.hilosTrabajo = 4;

    rendimiento.escalaResolucion = 1.0f;
    rendimiento.limiteFPS = 60;
    rendimiento.saltoCuadros = false;
    rendimiento.comprimirTexturas = true;
    rendimiento.cacheTexturas = true;
    rendimiento.sincronizacionAdaptativa = true;
    rendimiento.desactivarSombras = false;
    rendimiento.desactivarPosprocesado = false;
    rendimiento.nivelMipmap = 0;

    rendimiento.habilitarCacheMemoria = true;
    rendimiento.precargarZonas = true;
    rendimiento.comprimirRAM = false;
    rendimiento.limiteCacheMB = 512;

    rendimiento.frecuenciaAudio = 48000;
    rendimiento.tamañoBufferAudio = 20;
    rendimiento.reducirCanales = false;

    rendimiento.bloquearRelojCPU = true;
    rendimiento.altaPrioridad = true;
    rendimiento.reducirAnimacionesUI = true;
    rendimiento.usarNEON = true;
    rendimiento.suspenderEnSegundoPlano = true;

    rendimiento.monitoreoActivo = true;
    rendimiento.umbralFPSBajo = 45;

    return true;
}

// ==============================================================
// ⚙️ RESTABLECER POR DEFECTO
// ==============================================================
void restablecerPorDefecto() {
    aplicarNivelOptimizacion(NivelOptimizacion::EQUILIBRADO);
}

// ==============================================================
// 🎚️ APLICAR NIVEL DE OPTIMIZACIÓN — El usuario elige y se configura TODO
// ==============================================================
void aplicarNivelOptimizacion(NivelOptimizacion nivel) {
    rendimiento.nivel = nivel;

    switch (nivel) {
        case NivelOptimizacion::MINIMO:
            rendimiento.habilitarVectorizacion = false;
            rendimiento.habilitarMultihilo = false;
            rendimiento.precargarInstrucciones = false;
            rendimiento.fusionarBucle = false;
            rendimiento.hilosTrabajo = 1;
            rendimiento.escalaResolucion = 1.0f;
            rendimiento.saltoCuadros = false;
            rendimiento.comprimirTexturas = false;
            rendimiento.cacheTexturas = false;
            rendimiento.sincronizacionAdaptativa = false;
            rendimiento.desactivarSombras = false;
            rendimiento.desactivarPosprocesado = false;
            rendimiento.frecuenciaAudio = 48000;
            rendimiento.usarNEON = false;
            rendimiento.bloquearRelojCPU = false;
            rendimiento.altaPrioridad = false;
            break;

        case NivelOptimizacion::BASICO:
            rendimiento.habilitarVectorizacion = true;
            rendimiento.habilitarMultihilo = true;
            rendimiento.precargarInstrucciones = true;
            rendimiento.fusionarBucle = false;
            rendimiento.hilosTrabajo = 2;
            rendimiento.escalaResolucion = 1.0f;
            rendimiento.saltoCuadros = false;
            rendimiento.comprimirTexturas = true;
            rendimiento.cacheTexturas = true;
            rendimiento.sincronizacionAdaptativa = true;
            rendimiento.desactivarSombras = false;
            rendimiento.desactivarPosprocesado = false;
            rendimiento.frecuenciaAudio = 48000;
            rendimiento.usarNEON = true;
            rendimiento.bloquearRelojCPU = true;
            rendimiento.altaPrioridad = false;
            break;

        case NivelOptimizacion::EQUILIBRADO:
            rendimiento.habilitarVectorizacion = true;
            rendimiento.habilitarMultihilo = true;
            rendimiento.precargarInstrucciones = true;
            rendimiento.fusionarBucle = true;
            rendimiento.hilosTrabajo = 4;
            rendimiento.escalaResolucion = 1.0f;
            rendimiento.saltoCuadros = false;
            rendimiento.comprimirTexturas = true;
            rendimiento.cacheTexturas = true;
            rendimiento.sincronizacionAdaptativa = true;
            rendimiento.desactivarSombras = false;
            rendimiento.desactivarPosprocesado = false;
            rendimiento.frecuenciaAudio = 48000;
            rendimiento.usarNEON = true;
            rendimiento.bloquearRelojCPU = true;
            rendimiento.altaPrioridad = true;
            break;

        case NivelOptimizacion::AGRESIVO:
            rendimiento.habilitarVectorizacion = true;
            rendimiento.habilitarMultihilo = true;
            rendimiento.precargarInstrucciones = true;
            rendimiento.fusionarBucle = true;
            rendimiento.hilosTrabajo = 6;
            rendimiento.escalaResolucion = 0.85f;
            rendimiento.saltoCuadros = true;
            rendimiento.comprimirTexturas = true;
            rendimiento.cacheTexturas = true;
            rendimiento.sincronizacionAdaptativa = true;
            rendimiento.desactivarSombras = true;
            rendimiento.desactivarPosprocesado = false;
            rendimiento.nivelMipmap = 1;
            rendimiento.frecuenciaAudio = 44100;
            rendimiento.usarNEON = true;
            rendimiento.bloquearRelojCPU = true;
            rendimiento.altaPrioridad = true;
            break;

        case NivelOptimizacion::MAXIMO:
            rendimiento.habilitarVectorizacion = true;
            rendimiento.habilitarMultihilo = true;
            rendimiento.precargarInstrucciones = true;
            rendimiento.fusionarBucle = true;
            rendimiento.hilosTrabajo = 8;
            rendimiento.escalaResolucion = 0.7f;
            rendimiento.saltoCuadros = true;
            rendimiento.comprimirTexturas = true;
            rendimiento.cacheTexturas = true;
            rendimiento.sincronizacionAdaptativa = true;
            rendimiento.desactivarSombras = true;
            rendimiento.desactivarPosprocesado = true;
            rendimiento.nivelMipmap = 2;
            rendimiento.frecuenciaAudio = 32000;
            rendimiento.tamañoBufferAudio = 30;
            rendimiento.reducirCanales = true;
            rendimiento.usarNEON = true;
            rendimiento.bloquearRelojCPU = true;
            rendimiento.altaPrioridad = true;
            rendimiento.reducirAnimacionesUI = true;
            break;
    }
}

// ==============================================================
// 🔧 ACTIVAR/DESACTIVAR UNA OPTIMIZACIÓN INDIVIDUAL
// ==============================================================
bool setOptimizacion(TipoOptimizacion id, bool activar) {
    switch (id) {
        case TipoOptimizacion::VECTORIZACION_SIMD: rendimiento.habilitarVectorizacion = activar; break;
        case TipoOptimizacion::MULTIHILO: rendimiento.habilitarMultihilo = activar; break;
        case TipoOptimizacion::CACHE_INSTRUCCIONES: rendimiento.precargarInstrucciones = activar; break;
        case TipoOptimizacion::FUSION_BULES: rendimiento.fusionarBucle = activar; break;
        case TipoOptimizacion::SALTO_CUADROS: rendimiento.saltoCuadros = activar; break;
        case TipoOptimizacion::COMPRESION_TEXTURAS: rendimiento.comprimirTexturas = activar; break;
        case TipoOptimizacion::CACHE_TEXTURAS: rendimiento.cacheTexturas = activar; break;
        case TipoOptimizacion::SINCRONIZACION_ADAPTATIVA: rendimiento.sincronizacionAdaptativa = activar; break;
        case TipoOptimizacion::DESACTIVAR_EFECTOS: rendimiento.desactivarSombras = rendimiento.desactivarPosprocesado = activar; break;
        case TipoOptimizacion::CACHE_DATOS: rendimiento.habilitarCacheMemoria = activar; break;
        case TipoOptimizacion::PRECARGA_ZONAS: rendimiento.precargarZonas = activar; break;
        case TipoOptimizacion::COMPRESION_RAM: rendimiento.comprimirRAM = activar; break;
        case TipoOptimizacion::USO_NEON: rendimiento.usarNEON = activar; break;
        case TipoOptimizacion::BLOQUEO_RELOJ_CPU: rendimiento.bloquearRelojCPU = activar; break;
        case TipoOptimizacion::PRIORIDAD_HILOS: rendimiento.altaPrioridad = activar; break;
        default: return false;
    }
    return true;
}

bool getOptimizacion(TipoOptimizacion id) {
    switch (id) {
        case TipoOptimizacion::VECTORIZACION_SIMD: return rendimiento.habilitarVectorizacion;
        case TipoOptimizacion::MULTIHILO: return rendimiento.habilitarMultihilo;
        case TipoOptimizacion::CACHE_INSTRUCCIONES: return rendimiento.precargarInstrucciones;
        case TipoOptimizacion::FUSION_BULES: return rendimiento.fusionarBucle;
        case TipoOptimizacion::SALTO_CUADROS: return rendimiento.saltoCuadros;
        case TipoOptimizacion::COMPRESION_TEXTURAS: return rendimiento.comprimirTexturas;
        case TipoOptimizacion::CACHE_TEXTURAS: return rendimiento.cacheTexturas;
        case TipoOptimizacion::SINCRONIZACION_ADAPTATIVA: return rendimiento.sincronizacionAdaptativa;
        case TipoOptimizacion::DESACTIVAR_EFECTOS: return rendimiento.desactivarSombras;
        case TipoOptimizacion::CACHE_DATOS: return rendimiento.habilitarCacheMemoria;
        case TipoOptimizacion::PRECARGA_ZONAS: return rendimiento.precargarZonas;
        case TipoOptimizacion::COMPRESION_RAM: return rendimiento.comprimirRAM;
        case TipoOptimizacion::USO_NEON: return rendimiento.usarNEON;
        case TipoOptimizacion::BLOQUEO_RELOJ_CPU: return rendimiento.bloquearRelojCPU;
        case TipoOptimizacion::PRIORIDAD_HILOS: return rendimiento.altaPrioridad;
        default: return false;
    }
}

// ==============================================================
// 📊 ACTUALIZAR ESTADÍSTICAS — Cada cuadro se mide
// ==============================================================
void actualizarEstadisticas(const EstadisticasRendimiento& nuevasStats) {
    statsRendimiento = nuevasStats;

    // Actualizar FPS mínimo y máximo
    if (nuevasStats.fpsActual > 0) {
        statsRendimiento.fpsMinimo = std::min(statsRendimiento.fpsMinimo, nuevasStats.fpsActual);
        statsRendimiento.fpsMaximo = std::max(statsRendimiento.fpsMaximo, nuevasStats.fpsActual);
    }

    // Detectar si va lento
    statsRendimiento.bajoRendimiento = detectarBajoRendimiento();
}

// ==============================================================
// 🧠 AJUSTE AUTOMÁTICO — Si baja de FPS, activa optimizaciones solas
// ==============================================================
void ajusteDinamicoRendimiento() {
    if (!rendimiento.monitoreoActivo) return;
    if (!statsRendimiento.bajoRendimiento) return;

    static int contadorBajoFPS = 0;
    contadorBajoFPS++;

    // Solo ajustar si se mantiene bajo por varios cuadros
    if (contadorBajoFPS < 60) return; // ~1 segundo

    // ⚡ Aplicar optimizaciones progresivas automáticamente
    if (rendimiento.escalaResolucion > 0.9f) {
        rendimiento.escalaResolucion = 0.9f;
        statsRendimiento.mensajeOptimizacion = "Bajando resolución para mejorar rendimiento...";
    } else if (rendimiento.escalaResolucion > 0.75f) {
        rendimiento.escalaResolucion = 0.75f;
        statsRendimiento.mensajeOptimizacion = "Resolución reducida — rendimiento prioritario";
    } else if (!rendimiento.saltoCuadros) {
        rendimiento.saltoCuadros = true;
        statsRendimiento.mensajeOptimizacion = "Salto de cuadros activado";
    } else if (!rendimiento.desactivarSombras) {
        rendimiento.desactivarSombras = true;
        statsRendimiento.mensajeOptimizacion = "Sombras desactivadas";
    }

    // Reiniciar contador tras ajuste
    contadorBajoFPS = 0;
}

// ==============================================================
// 🔍 DETECTAR BAJO RENDIMIENTO
// ==============================================================
bool detectarBajoRendimiento() {
    return statsRendimiento.fpsActual < rendimiento.umbralFPSBajo;
}

// ==============================================================
// 💡 OBTENER RECOMENDACIÓN DE MEJORA
// ==============================================================
std::string obtenerRecomendacion() {
    if (statsRendimiento.fpsActual >= 55) {
        return "✅ Rendimiento excelente — todo funciona al máximo";
    } else if (statsRendimiento.fpsActual >= 30) {
        return "⚠️ Rendimiento aceptable — puedes mejorar bajando la resolución";
    } else {
        return "🐢 Rendimiento bajo — activa nivel Agresivo o Máximo";
    }
}

// ==============================================================
// 💾 GUARDAR / CARGAR CONFIGURACIÓN
// ==============================================================
bool guardarConfiguracionRendimiento(const std::string& ruta) {
    std::ofstream archivo(ruta, std::ios::binary);
    if (!archivo) return false;

    archivo.write(reinterpret_cast<const char*>(&rendimiento), sizeof(ConfiguracionRendimiento));
    return archivo.good();
}

bool cargarConfiguracionRendimiento(const std::string& ruta) {
    std::ifstream archivo(ruta, std::ios::binary);
    if (!archivo) return false;

    archivo.read(reinterpret_cast<char*>(&rendimiento), sizeof(ConfiguracionRendimiento));
    return archivo.good();
}

// ==============================================================
// 📋 LISTA DE OPTIMIZACIONES PARA MOSTRAR AL USUARIO
// ==============================================================
std::vector<Optimizacion> obtenerListaOptimizaciones() {
    return listaOptimizaciones;
}

// ==============================================================
// 📦 ACCESO GLOBAL
// ==============================================================
ConfiguracionRendimiento& obtenerConfiguracionRendimiento() {
    return rendimiento;
}

EstadisticasRendimiento& obtenerEstadisticasRendimiento() {
    return statsRendimiento;
}
