#include "GraficaRPCS5.h"
#include <cstring>
#include <algorithm>
#include <chrono>

// 🌍 Variable global del sistema gráfico
EstadoGrafico grafico;

// 📋 Nombres de los modos de renderizado
std::string nombreModoRender(ModoRender modo) {
    switch (modo) {
        case ModoRender::VULKAN:    return "Vulkan (Recomendado)";
        case ModoRender::OPENGL:    return "OpenGL (Compatible)";
        case ModoRender::SOFTWARE:  return "Software (Sin aceleración)";
        case ModoRender::METAL:     return "Metal";
        case ModoRender::DIRECT3D:  return "Direct3D";
        default:                    return "Desconocido";
    }
}

// ⏱️ Variables para calcular FPS
static uint64_t cuadrosContados = 0;
static auto tiempoInicio = std::chrono::steady_clock::now();

// ✅ INICIALIZAR — crear contexto gráfico y ventana
bool inicializarGrafico(ModoRender modo, uint32_t ancho, uint32_t alto) {
    // Limpiar estado anterior
    std::memset(&grafico, 0, sizeof(EstadoGrafico));
    grafico.inicializado = false;
    grafico.modo = modo;
    grafico.anchoPantalla = ancho;
    grafico.altoPantalla = alto;
    grafico.anchoInterno = ancho;
    grafico.altoInterno = alto;
    grafico.escala = 1.0f;
    grafico.vsync = true;

    // ⚠️ En versión completa: crear contexto Vulkan/OpenGL aquí
    // En versión real: cargar extensiones, crear ventana, crear swapchain

    // ✅ Simulación de inicialización exitosa
    grafico.inicializado = true;
    grafico.mensajeError.clear();

    // Reiniciar contador de FPS
    cuadrosContados = 0;
    tiempoInicio = std::chrono::steady_clock::now();

    return true;
}

// 🗑️ APAGAR — liberar todo al cerrar
void apagarGrafico() {
    if (!grafico.inicializado) return;

    // ⚠️ En versión completa: destruir texturas, shaders, buffers, swapchain
    grafico.inicializado = false;
    grafico.mensajeError = "Sistema gráfico apagado";
}

// 🎬 EMPEZAR CUADRO — preparar para dibujar
void empezarCuadro() {
    if (!grafico.inicializado) return;

    // ⚠️ En versión completa: iniciar comando buffer, limpiar pantalla, establecer viewport
}

// 🖼️ TERMINAR CUADRO — mostrar en pantalla
void terminarCuadro() {
    if (!grafico.inicializado) return;

    // ⚠️ En versión completa: finalizar comando buffer, presentar imagen, swapchain

    // ⏱️ Contar cuadro y calcular FPS
    cuadrosContados++;
    grafico.cuadrosRenderizados++;

    auto tiempoActual = std::chrono::steady_clock::now();
    double segundosTranscurridos =
        std::chrono::duration<double>(tiempoActual - tiempoInicio).count();

    if (segundosTranscurridos >= 1.0) {
        grafico.fpsActual = static_cast<uint32_t>(
            static_cast<double>(cuadrosContados) / segundosTranscurridos);
        cuadrosContados = 0;
        tiempoInicio = tiempoActual;
    }
}

// 🧹 LIMPIAR PANTALLA — color de fondo
void limpiarPantalla(float r, float g, float b, float a) {
    if (!grafico.inicializado) return;

    // ⚠️ En versión completa: glClear / vkCmdClearColorImage con el color RGBA
}

// 📦 CREAR TEXTURA — cargar imagen desde memoria
uint32_t crearTextura(uint64_t dirMemoria, uint32_t ancho, uint32_t alto,
                      FormatoTextura formato, const std::string& nombre) {
    if (!grafico.inicializado) return 0;

    // ⚠️ En versión completa:
    // 1. Leer datos desde memoria de PS5 (MemoriaRPCS5)
    // 2. Crear imagen en Vulkan/OpenGL
    // 3. Subir píxeles a GPU
    // 4. Generar ID único

    static uint32_t proximoId = 1;
    uint32_t id = proximoId++;

    return id;
}

// 🗑️ LIBERAR TEXTURA
void liberarTextura(uint32_t idTextura) {
    if (!grafico.inicializado) return;

    // ⚠️ En versión completa: destruir imagen y vista en GPU
}

// 🎨 USAR TEXTURA — activar para dibujar
void usarTextura(uint32_t idTextura) {
    if (!grafico.inicializado) return;

    // ⚠️ En versión completa: bind de la textura al pipeline
}

// 🔲 DIBUJAR VÉRTICES — modelos 3D
void dibujarVertices(const Vertice* vertices, size_t cantidad) {
    if (!grafico.inicializado) return;

    // ⚠️ En versión completa:
    // 1. Copiar vértices a buffer de GPU
    // 2. Enlazar atributos (posición, color, textura, normal)
    // 3. Llamar a draw
}

// 🖼️ DIBUJAR TEXTURA EN PANTALLA
void dibujarTextura(uint32_t idTextura, float x, float y, float ancho, float alto) {
    if (!grafico.inicializado) return;

    // ⚠️ En versión completa:
    // Dibujar un rectángulo con la textura aplicada
    // Útil para UI, menús, pantalla completa
}

// 📐 CAMBIAR RESOLUCIÓN
bool cambiarResolucion(uint32_t ancho, uint32_t alto) {
    if (!grafico.inicializado) return false;
    if (ancho == 0 || alto == 0) return false;

    grafico.anchoPantalla = ancho;
    grafico.altoPantalla = alto;
    grafico.anchoInterno = static_cast<uint32_t>(ancho * grafico.escala);
    grafico.altoInterno = static_cast<uint32_t>(alto * grafico.escala);

    // ⚠️ En versión completa: recrear swapchain con nueva resolución

    return true;
}

// 🔄 CAMBIAR MODO DE RENDER
bool cambiarModoRender(ModoRender modo) {
    if (!grafico.inicializado) return false;
    if (grafico.modo == modo) return true;

    // ⚠️ En versión completa:
    // 1. Apagar contexto actual
    // 2. Crear nuevo contexto con el modo solicitado
    // 3. Restaurar texturas y estado

    grafico.modo = modo;
    return true;
}

// ⏱️ SINCRONIZACIÓN VERTICAL
void setVSync(bool activar) {
    grafico.vsync = activar;

    // ⚠️ En versión completa: establecer presente mode en swapchain
}

// 🔍 ESCALA DE RESOLUCIÓN
void setEscala(float escala) {
    if (escala < 0.25f) escala = 0.25f;
    if (escala > 2.0f) escala = 2.0f;

    grafico.escala = escala;
    grafico.anchoInterno = static_cast<uint32_t>(grafico.anchoPantalla * escala);
    grafico.altoInterno = static_cast<uint32_t>(grafico.altoPantalla * escala);

    // ⚠️ En versión completa: actualizar viewport y resolución interna
}

// 📊 OBTENER ESTADO Y FPS
EstadoGrafico obtenerEstadoGrafico() {
    return grafico;
}

uint32_t obtenerFPS() {
    return grafico.fpsActual;
}
