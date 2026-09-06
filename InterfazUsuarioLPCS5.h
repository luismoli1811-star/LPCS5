#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

// 📐 DIMENSIONES DE PANTALLA — orientación vertical (celular)
constexpr uint32_t UI_ANCHO_MAX = 1080;
constexpr uint32_t UI_ALTO_MAX  = 2400;
constexpr uint32_t UI_MARGEN    = 32;
constexpr uint32_t UI_ALTO_BARRA_SUPERIOR = 80;
constexpr uint32_t UI_ALTO_BARRA_INFERIOR = 96;

// 🎨 COLORES PRINCIPALES — tema oscuro moderno
struct ColorUI {
    uint8_t r, g, b, a;

    static constexpr ColorUI FONDO        () { return { 18,  18,  24, 255 }; }
    static constexpr ColorUI FONDO_ALTO   () { return { 30,  30,  40, 255 }; }
    static constexpr ColorUI DESTACADO    () { return { 66, 133, 244, 255 }; } // Azul PS
    static constexpr ColorUI DESTACADO_OSCURO() { return { 41,  98, 184, 255 }; }
    static constexpr ColorUI TEXTO_PRIMARIO() { return {255, 255, 255, 255 }; }
    static constexpr ColorUI TEXTO_SECUNDARIO() { return {170, 170, 180, 255 }; }
    static constexpr ColorUI BORDE        () { return { 50,  50,  65, 255 }; }
    static constexpr ColorUI EXITO       () { return { 76, 175,  80, 255 }; }
    static constexpr ColorUI ADVERTENCIA  () { return {255, 193,   7, 255 }; }
    static constexpr ColorUI ERROR       () { return {244,  67,  54, 255 }; }
    static constexpr ColorUI TRANSPARENTE() { return {  0,   0,   0,   0 }; }
};

// 📍 POSICIÓN Y TAMAÑO DE UN ELEMENTO
struct RectUI {
    float x, y;
    float ancho, alto;

    bool contiene(float px, float py) const {
        return px >= x && px <= x + ancho && py >= y && py <= y + alto;
    }
};

// 🔘 TIPOS DE ELEMENTOS
enum class TipoElementoUI : uint32_t {
    NINGUNO = 0,
    BOTON,
    ETIQUETA,
    LISTA,
    DESLIZADOR,
    INTERRUPTOR,
    ENTRADA_TEXTO,
    IMAGEN,
    SEPARADOR,
    TARJETA_JUEGO
};

// 🔘 ESTADO VISUAL DEL ELEMENTO
enum class EstadoElementoUI : uint8_t {
    NORMAL = 0,
    PRESIONADO,
    SELECCIONADO,
    DESACTIVADO,
    ENFOQUE
};

// 🧩 ELEMENTO BASE DE LA INTERFAZ
struct ElementoUI {
    std::string id;
    TipoElementoUI tipo;
    RectUI rect;
    EstadoElementoUI estado;
    bool visible;
    bool interactuable;

    std::string texto;
    std::string icono;
    ColorUI colorFondo;
    ColorUI colorTexto;

    // 📞 Función que se ejecuta al tocar/activar
    std::function<void()> alActivar;
    std::function<void(float)> alCambiarValor; // Para deslizadores/interruptores

    float valor;      // 0.0 a 1.0 — deslizador, interruptor
    size_t indice;    // Para listas
};

// 📱 PANTALLAS / VISTAS DE LA APLICACIÓN
enum class PantallaActiva : uint32_t {
    CARGA = 0,
    INICIO,
    SELECCION_JUEGO,
    JUEGO_CORRIENDO,
    AJUSTES,
    FIRMWARE,
    CONTROLES,
    INFORMACION,
    SALIR
};

// ⚙️ ESTADO COMPLETO DE LA INTERFAZ
struct EstadoInterfaz {
    bool inicializada = false;
    bool visible = true;
    PantallaActiva pantallaActual = PantallaActiva::CARGA;
    PantallaActiva pantallaAnterior = PantallaActiva::CARGA;

    uint32_t anchoPantalla = 1080;
    uint32_t altoPantalla = 2400;
    float escalaDensidad = 1.0f;

    std::vector<ElementoUI> elementos;
    std::string elementoEnfocado;
    std::string elementoPresionado;

    bool mostrarBarraEstado = true;
    bool mostrarControlesTactiles = true;
    bool pantallaCompleta = false;

    std::string mensajeEstado;
    bool mostrarMensajeTemporal = false;
    float tiempoMensajeTemporal = 0.0f;
};

// 🎯 FUNCIONES PRINCIPALES
// Inicializar todo el sistema de interfaz
bool inicializarInterfaz(uint32_t ancho, uint32_t alto, float escalaDensidad = 1.0f);

// Liberar recursos y cerrar
void apagarInterfaz();

// 🔄 CICLO DE RENDER
void empezarDibujadoUI();
void terminarDibujadoUI();
void dibujarInterfazCompleto();

// 📱 NAVEGACIÓN DE PANTALLAS
void cambiarPantalla(PantallaActiva nuevaPantalla);
void volverPantallaAnterior();
PantallaActiva obtenerPantallaActual();

// 🧩 GESTIÓN DE ELEMENTOS
ElementoUI* crearBoton(const std::string& id, const RectUI& rect,
                        const std::string& texto, std::function<void()> callback);
ElementoUI* crearEtiqueta(const std::string& id, const RectUI& rect,
                           const std::string& texto, ColorUI color = ColorUI::TEXTO_PRIMARIO());
ElementoUI* crearInterruptor(const std::string& id, const RectUI& rect,
                              bool valorInicial, std::function<void(bool)> callback);
ElementoUI* crearDeslizador(const std::string& id, const RectUI& rect,
                             float valorMin, float valorMax, float inicial,
                             std::function<void(float)> callback);
ElementoUI* crearLista(const std::string& id, const RectUI& rect,
                       const std::vector<std::string>& items);
ElementoUI* crearTarjetaJuego(const std::string& id, const RectUI& rect,
                              const std::string& titulo, const std::string& rutaPortada,
                              std::function<void()> alSeleccionar);
void eliminarElemento(const std::string& id);
void ocultarElemento(const std::string& id);
void mostrarElemento(const std::string& id);
ElementoUI* buscarElemento(const std::string& id);

// 👆 ENTRADA TÁCTIL / TECLADO
void procesarToque(float x, float y, bool presionado);
void procesarDesplazamiento(float deltaY);
void procesarTecla(int codigoTecla, bool presionada);

// 📊 BARRA DE ESTADO Y MENSAJES
void actualizarBarraEstado(const std::string& estado, uint32_t fps = 0,
                           uint64_t memoriaUsada = 0);
void mostrarMensajeTemporal(const std::string& mensaje, float duracionSegundos = 2.0f);
void ocultarMensajeTemporal();

// 🎨 ESTILO Y CONFIGURACIÓN
void setTemaOscuro();
void setTemaClaro();
void setEscalaInterfaz(float escala);
void alternarControlesTactiles();
void alternarPantallaCompleta();

// 📦 ESTADO GLOBAL
EstadoInterfaz& obtenerEstadoInterfaz();

// 🌍 Variable global
extern EstadoInterfaz interfaz;
