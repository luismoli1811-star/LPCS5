#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

// 📐 RESOLUCIONES ESTÁNDAR DE PS5
constexpr uint32_t RES_ANCHO_BASE = 3840;   // 4K — resolución máxima
constexpr uint32_t RES_ALTO_BASE  = 2160;
constexpr uint32_t RES_ANCHO_1080 = 1920;   // 1080p — modo rendimiento
constexpr uint32_t RES_ALTO_1080  = 1080;
constexpr uint32_t RES_ANCHO_720  = 1280;   // 720p — celular
constexpr uint32_t RES_ALTO_720   = 720;

// 🎨 FORMATOS DE TEXTURA
enum class FormatoTextura : uint32_t {
    DESCONOCIDO = 0,
    RGBA8888,   // 32 bits — estándar
    BGRA8888,   // 32 bits — ordenado diferente
    RGB565,     // 16 bits — más ligero
    DXT1,       // Compresión 4:1
    DXT3,       // Compresión con alfa
    DXT5,       // Compresión con alfa interpolada
    BC7,        // Mejor compresión — PS5 lo usa mucho
    R8,         // 8 bits — escala de grises
    RG88        // 16 bits — dos canales
};

// 📦 TEXTURA — imagen que se dibuja en pantalla
struct Textura {
    uint64_t direccionMemoria;  // Dónde está en VRAM
    uint32_t ancho;
    uint32_t alto;
    uint32_t mipmaps;          // Niveles de detalle
    FormatoTextura formato;
    uint32_t tamanoBytes;
    bool cargada;
    std::string nombre;
};

// 🔲 VÉRTICE — punto del modelo 3D
struct Vertice {
    float x, y, z;          // Posición en el espacio
    float r, g, b, a;       // Color
    float u, v;             // Coordenadas de textura
    float nx, ny, nz;       // Normal (iluminación)
};

// 🎯 TIPO DE RENDERIZADO
enum class ModoRender {
    SOFTWARE,   // 🖥️ Solo CPU — lento, pero funciona en todo
    OPENGL,     // 🟢 OpenGL — compatible con casi todo
    VULKAN,     // ⚡ Vulkan — máximo rendimiento, nativo de PS5
    METAL,      // 🍎 Metal — Apple (no usado en Android)
    DIRECT3D    // 🪟 Direct3D — Windows (no en Android)
};

// ⚙️ ESTADO DEL SISTEMA GRÁFICO
struct EstadoGrafico {
    bool inicializado = false;
    ModoRender modo = ModoRender::VULKAN;
    uint32_t anchoPantalla = RES_ANCHO_1080;
    uint32_t altoPantalla = RES_ALTO_1080;
    uint32_t anchoInterno = RES_ANCHO_1080; // Resolución interna del juego
    uint32_t altoInterno = RES_ALTO_1080;
    float escala = 1.0f;
    bool vsync = true;
    uint32_t fpsActual = 0;
    uint64_t cuadrosRenderizados = 0;
    std::string mensajeError;
};

// 🎯 FUNCIONES PRINCIPALES
// Inicializar el sistema gráfico — crear ventana y contexto de render
bool inicializarGrafico(ModoRender modo, uint32_t ancho, uint32_t alto);

// Cerrar todo y liberar recursos
void apagarGrafico();

// 🖼️ CICLO DE PANTALLA
// Empezar a dibujar un cuadro nuevo
void empezarCuadro();
// Terminar y mostrar el cuadro en pantalla
void terminarCuadro();
// Limpiar pantalla con un color
void limpiarPantalla(float r, float g, float b, float a = 1.0f);

// 📦 TEXTURAS
// Cargar una textura desde memoria
uint32_t crearTextura(uint64_t dirMemoria, uint32_t ancho, uint32_t alto,
                      FormatoTextura formato, const std::string& nombre = "");
// Liberar una textura
void liberarTextura(uint32_t idTextura);
// Activar una textura para dibujar
void usarTextura(uint32_t idTextura);

// 🔲 DIBUJAR
// Dibujar un triángulo o polígono con vértices
void dibujarVertices(const Vertice* vertices, size_t cantidad);
// Dibujar una textura en pantalla completa o en una región
void dibujarTextura(uint32_t idTextura, float x, float y, float ancho, float alto);

// ⚙️ CONFIGURACIÓN
// Cambiar resolución
bool cambiarResolucion(uint32_t ancho, uint32_t alto);
// Cambiar modo de renderizado
bool cambiarModoRender(ModoRender modo);
// Activar/desactivar sincronización vertical
void setVSync(bool activar);
// Escalar la resolución interna
void setEscala(float escala);

// 📊 INFORMACIÓN
EstadoGrafico obtenerEstadoGrafico();
uint32_t obtenerFPS();
std::string nombreModoRender(ModoRender modo);

// 🌍 Variable global
extern EstadoGrafico grafico;
