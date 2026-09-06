#include "InterfazUsuarioRPCS5.h"
#include "GraficaRPCS5.h"
#include <cstring>
#include <algorithm>
#include <cmath>

// 🌍 Variable global de la interfaz
EstadoInterfaz interfaz;

// ==============================================================
// 🚀 INICIALIZAR — Preparar todo el sistema de interfaz
// ==============================================================
bool inicializarInterfaz(uint32_t ancho, uint32_t alto, float escalaDensidad) {
    std::memset(&interfaz, 0, sizeof(EstadoInterfaz));

    interfaz.anchoPantalla = ancho;
    interfaz.altoPantalla = alto;
    interfaz.escalaDensidad = escalaDensidad;
    interfaz.pantallaActual = PantallaActiva::CARGA;
    interfaz.visible = true;
    interfaz.mostrarBarraEstado = true;
    interfaz.mostrarControlesTactiles = true;
    interfaz.inicializada = true;

    // Crear elementos de la pantalla de carga
    ElementoUI etiquetaCarga;
    etiquetaCarga.id = "etq_carga";
    etiquetaCarga.tipo = TipoElementoUI::ETIQUETA;
    etiquetaCarga.visible = true;
    etiquetaCarga.interactuable = false;
    etiquetaCarga.texto = "Inicializando RPCS5...";
    etiquetaCarga.colorTexto = ColorUI::TEXTO_PRIMARIO();
    etiquetaCarga.rect = { ancho/2.0f - 150.0f, alto/2.0f, 300.0f, 50.0f };
    interfaz.elementos.push_back(etiquetaCarga);

    return true;
}

// ==============================================================
// 🛑 APAGAR — Liberar recursos
// ==============================================================
void apagarInterfaz() {
    interfaz.elementos.clear();
    interfaz.inicializada = false;
}

// ==============================================================
// 🔄 CAMBIAR PANTALLA — Navegación principal
// ==============================================================
void cambiarPantalla(PantallaActiva nuevaPantalla) {
    if (!interfaz.inicializada) return;

    interfaz.pantallaAnterior = interfaz.pantallaActual;
    interfaz.pantallaActual = nuevaPantalla;

    // Limpiar elementos de la pantalla anterior
    interfaz.elementos.clear();
    interfaz.elementoEnfocado.clear();
    interfaz.elementoPresionado.clear();

    // Cargar elementos según la pantalla nueva
    switch (nuevaPantalla) {
        case PantallaActiva::CARGA:
            crearEtiqueta("etq_carga",
                { interfaz.anchoPantalla/2 - 150, interfaz.altoPantalla/2, 300, 50 },
                "Inicializando...", ColorUI::TEXTO_PRIMARIO());
            break;

        case PantallaActiva::INICIO: {
            float centroX = interfaz.anchoPantalla / 2.0f;
            float botonAncho = 320 * interfaz.escalaDensidad;
            float botonAlto = 72 * interfaz.escalaDensidad;
            float espaciado = 32 * interfaz.escalaDensidad;
            float inicioY = interfaz.altoPantalla * 0.25f;

            crearEtiqueta("titulo",
                { centroX - 150, 80, 300, 60 },
                "RPCS5", ColorUI::DESTACADO());

            crearBoton("btn_juegos",
                { centroX - botonAncho/2, inicioY, botonAncho, botonAlto },
                "🎮 Mis Juegos", [](){ cambiarPantalla(PantallaActiva::SELECCION_JUEGO); });

            crearBoton("btn_firmware",
                { centroX - botonAncho/2, inicioY + botonAlto + espaciado, botonAncho, botonAlto },
                "📄 Firmware", [](){ cambiarPantalla(PantallaActiva::FIRMWARE); });

            crearBoton("btn_ajustes",
                { centroX - botonAncho/2, inicioY + (botonAlto + espaciado) * 2, botonAncho, botonAlto },
                "⚙️ Ajustes", [](){ cambiarPantalla(PantallaActiva::AJUSTES); });

            crearBoton("btn_salir",
                { centroX - botonAncho/2, inicioY + (botonAlto + espaciado) * 3, botonAncho, botonAlto },
                "🚪 Salir", [](){ cambiarPantalla(PantallaActiva::SALIR); });
            break;
        }

        case PantallaActiva::SELECCION_JUEGO: {
            crearEtiqueta("titulo_juegos",
                { UI_MARGEN, UI_MARGEN, 300, 50 },
                "Mis Juegos", ColorUI::TEXTO_PRIMARIO());

            crearBoton("btn_atras_juegos",
                { interfaz.anchoPantalla - 140, 30, 110, 60 },
                "← Atrás", [](){ volverPantallaAnterior(); });

            // Lista de juegos — se llena dinámicamente
            float tarjetaAncho = (interfaz.anchoPantalla - UI_MARGEN * 3) / 2.0f;
            float tarjetaAlto = 220 * interfaz.escalaDensidad;
            float tarjetaY = 120;

            crearTarjetaJuego("juego_1",
                { UI_MARGEN, tarjetaY, tarjetaAncho, tarjetaAlto },
                "Astro Bot", "", [](){ /* cargar juego */ });

            crearTarjetaJuego("juego_2",
                { UI_MARGEN * 2 + tarjetaAncho, tarjetaY, tarjetaAncho, tarjetaAlto },
                "God of War", "", [](){ /* cargar juego */ });
            break;
        }

        case PantallaActiva::AJUSTES: {
            crearEtiqueta("titulo_ajustes",
                { UI_MARGEN, UI_MARGEN, 300, 50 },
                "Ajustes", ColorUI::TEXTO_PRIMARIO());

            crearBoton("btn_atras_ajustes",
                { interfaz.anchoPantalla - 140, 30, 110, 60 },
                "← Atrás", [](){ volverPantallaAnterior(); });

            float filaY = 120;
            float interruptorX = interfaz.anchoPantalla - 120;

            crearEtiqueta("etq_vsync", { UI_MARGEN, filaY + 10, 250, 40 }, "Sincronización V", ColorUI::TEXTO_PRIMARIO());
            crearInterruptor("sw_vsync", { interruptorX, filaY, 60, 40 }, true, [](bool activar){ setVSync(activar); });

            filaY += 70;
            crearEtiqueta("etq_tactil", { UI_MARGEN, filaY + 10, 250, 40 }, "Controles táctiles", ColorUI::TEXTO_PRIMARIO());
            crearInterruptor("sw_tactil", { interruptorX, filaY, 60, 40 }, true, [](bool activar){ interfaz.mostrarControlesTactiles = activar; });

            filaY += 70;
            crearEtiqueta("etq_escala", { UI_MARGEN, filaY + 10, 250, 40 }, "Escala interfaz", ColorUI::TEXTO_PRIMARIO());
            crearDeslizador("sl_escala", { UI_MARGEN, filaY + 50, interfaz.anchoPantalla - UI_MARGEN * 2, 20 }, 0.8f, 1.5f, 1.0f, [](float v){ setEscalaInterfaz(v); });
            break;
        }

        case PantallaActiva::JUEGO_CORRIENDO:
            // En juego — controles táctiles y barra de estado
            interfaz.mostrarControlesTactiles = true;
            break;

        default:
            break;
    }
}

// ==============================================================
// ↩️ VOLVER A LA PANTALLA ANTERIOR
// ==============================================================
void volverPantallaAnterior() {
    cambiarPantalla(interfaz.pantallaAnterior);
}

// ==============================================================
// 📱 OBTENER PANTALLA ACTUAL
// ==============================================================
PantallaActiva obtenerPantallaActual() {
    return interfaz.pantallaActual;
}

// ==============================================================
// 🧩 CREAR ELEMENTOS — Botones, etiquetas, listas...
// ==============================================================

ElementoUI* crearBoton(const std::string& id, const RectUI& rect,
                       const std::string& texto, std::function<void()> callback) {
    ElementoUI elem;
    elem.id = id;
    elem.tipo = TipoElementoUI::BOTON;
    elem.rect = rect;
    elem.texto = texto;
    elem.visible = true;
    elem.interactuable = true;
    elem.estado = EstadoElementoUI::NORMAL;
    elem.colorFondo = ColorUI::DESTACADO();
    elem.colorTexto = ColorUI::TEXTO_PRIMARIO();
    elem.alActivar = callback;
    interfaz.elementos.push_back(elem);
    return &interfaz.elementos.back();
}

ElementoUI* crearEtiqueta(const std::string& id, const RectUI& rect,
                          const std::string& texto, ColorUI color) {
    ElementoUI elem;
    elem.id = id;
    elem.tipo = TipoElementoUI::ETIQUETA;
    elem.rect = rect;
    elem.texto = texto;
    elem.visible = true;
    elem.interactuable = false;
    elem.colorTexto = color;
    interfaz.elementos.push_back(elem);
    return &interfaz.elementos.back();
}

ElementoUI* crearInterruptor(const std::string& id, const RectUI& rect,
                              bool valorInicial, std::function<void(bool)> callback) {
    ElementoUI elem;
    elem.id = id;
    elem.tipo = TipoElementoUI::INTERRUPTOR;
    elem.rect = rect;
    elem.visible = true;
    elem.interactuable = true;
    elem.valor = valorInicial ? 1.0f : 0.0f;
    elem.alCambiarValor = [callback](float v) { callback(v >= 0.5f); };
    interfaz.elementos.push_back(elem);
    return &interfaz.elementos.back();
}

ElementoUI* crearDeslizador(const std::string& id, const RectUI& rect,
                            float valorMin, float valorMax, float inicial,
                            std::function<void(float)> callback) {
    ElementoUI elem;
    elem.id = id;
    elem.tipo = TipoElementoUI::DESLIZADOR;
    elem.rect = rect;
    elem.visible = true;
    elem.interactuable = true;
    elem.valor = inicial;
    elem.alCambiarValor = callback;
    interfaz.elementos.push_back(elem);
    return &interfaz.elementos.back();
}

ElementoUI* crearLista(const std::string& id, const RectUI& rect,
                      const std::vector<std::string>& items) {
    ElementoUI elem;
    elem.id = id;
    elem.tipo = TipoElementoUI::LISTA;
    elem.rect = rect;
    elem.visible = true;
    elem.interactuable = true;
    interfaz.elementos.push_back(elem);
    return &interfaz.elementos.back();
}

ElementoUI* crearTarjetaJuego(const std::string& id, const RectUI& rect,
                              const std::string& titulo, const std::string& rutaPortada,
                              std::function<void()> alSeleccionar) {
    ElementoUI elem;
    elem.id = id;
    elem.tipo = TipoElementoUI::TARJETA_JUEGO;
    elem.rect = rect;
    elem.texto = titulo;
    elem.icono = rutaPortada;
    elem.visible = true;
    elem.interactuable = true;
    elem.alActivar = alSeleccionar;
    interfaz.elementos.push_back(elem);
    return &interfaz.elementos.back();
}

// ==============================================================
// 🔍 BUSCAR / OCULTAR / ELIMINAR ELEMENTOS
// ==============================================================
ElementoUI* buscarElemento(const std::string& id) {
    for (auto& elem : interfaz.elementos) {
        if (elem.id == id) return &elem;
    }
    return nullptr;
}

void ocultarElemento(const std::string& id) {
    if (auto* elem = buscarElemento(id)) elem->visible = false;
}

void mostrarElemento(const std::string& id) {
    if (auto* elem = buscarElemento(id)) elem->visible = true;
}

void eliminarElemento(const std::string& id) {
    interfaz.elementos.erase(
        std::remove_if(interfaz.elementos.begin(), interfaz.elementos.end(),
            [&](const ElementoUI& e) { return e.id == id; }),
        interfaz.elementos.end());
}

// ==============================================================
// 👆 PROCESAR TOQUES — ¡El dedo toca la pantalla!
// ==============================================================
void procesarToque(float x, float y, bool presionado) {
    if (!interfaz.inicializada || !interfaz.visible) return;

    if (presionado) {
        // Buscar qué elemento fue tocado
        for (auto it = interfaz.elementos.rbegin(); it != interfaz.elementos.rend(); ++it) {
            if (!it->visible || !it->interactuable) continue;

            if (it->rect.contiene(x, y)) {
                interfaz.elementoPresionado = it->id;
                it->estado = EstadoElementoUI::PRESIONADO;

                // Ejecutar acción según tipo
                if (it->tipo == TipoElementoUI::BOTON ||
                    it->tipo == TipoElementoUI::TARJETA_JUEGO) {
                    if (it->alActivar) it->alActivar();
                }
                else if (it->tipo == TipoElementoUI::INTERRUPTOR) {
                    it->valor = it->valor >= 0.5f ? 0.0f : 1.0f;
                    if (it->alCambiarValor) it->alCambiarValor(it->valor);
                }
                else if (it->tipo == TipoElementoUI::DESLIZADOR) {
                    float rel = (x - it->rect.x) / it->rect.ancho;
                    it->valor = std::max(0.0f, std::min(1.0f, rel));
                    if (it->alCambiarValor) it->alCambiarValor(it->valor);
                }
                break;
            }
        }
    } else {
        // Soltar el elemento
        if (!interfaz.elementoPresionado.empty()) {
            if (auto* elem = buscarElemento(interfaz.elementoPresionado)) {
                elem->estado = EstadoElementoUI::NORMAL;
            }
            interfaz.elementoPresionado.clear();
        }
    }
}

// ==============================================================
// 👆 DESPLAZAMIENTO — deslizar el dedo
// ==============================================================
void procesarDesplazamiento(float deltaY) {
    // ⚠️ En versión completa: desplazar listas, menús, scroll
}

// ==============================================================
// ⌨️ TECLADO
// ==============================================================
void procesarTecla(int codigoTecla, bool presionada) {
    // ⚠️ En versión completa: atajos de teclado, navegación con teclas
}

// ==============================================================
// 📊 BARRA DE ESTADO — FPS, memoria, estado
// ==============================================================
void actualizarBarraEstado(const std::string& estado, uint32_t fps, uint64_t memoriaUsada) {
    interfaz.mensajeEstado = estado;
    // ⚠️ En versión completa: dibujar barra superior con FPS y uso de memoria
}

// ==============================================================
// 💬 MENSAJES TEMPORALES
// ==============================================================
void mostrarMensajeTemporal(const std::string& mensaje, float duracion) {
    interfaz.mensajeEstado = mensaje;
    interfaz.mostrarMensajeTemporal = true;
    interfaz.tiempoMensajeTemporal = duracion;
}

void ocultarMensajeTemporal() {
    interfaz.mostrarMensajeTemporal = false;
}

// ==============================================================
// 🎨 ESTILO Y ESCALA
// ==============================================================
void setEscalaInterfaz(float escala) {
    escala = std::max(0.8f, std::min(1.5f, escala));
    interfaz.escalaDensidad = escala;
    // ⚠️ En versión completa: recalcula todos los tamaños y posiciones
}

void alternarControlesTactiles() {
    interfaz.mostrarControlesTactiles = !interfaz.mostrarControlesTactiles;
}

void alternarPantallaCompleta() {
    interfaz.pantallaCompleta = !interfaz.pantallaCompleta;
}

void setTemaOscuro() { /* ⚠️ Implementar cambio de colores */ }
void setTemaClaro() { /* ⚠️ Implementar cambio de colores */ }

// ==============================================================
// 📦 OBTENER ESTADO
// ==============================================================
EstadoInterfaz& obtenerEstadoInterfaz() {
    return interfaz;
}

// ==============================================================
// 🎬 CICLO DE DIBUJADO
// ==============================================================
void empezarDibujadoUI() {
    if (!interfaz.inicializada || !interfaz.visible) return;
    // ⚠️ En versión completa: preparar capa de renderizado de UI
}

void terminarDibujadoUI() {
    if (!interfaz.inicializada || !interfaz.visible) return;
    // ⚠️ En versión completa: presentar capa de UI encima del juego
}

void dibujarInterfazCompleto() {
    if (!interfaz.inicializada || !interfaz.visible) return;

    // ⚠️ En versión completa:
    // 1. Dibujar fondo de pantalla
    // 2. Dibujar cada elemento en orden (de atrás hacia adelante)
    // 3. Dibujar barra de estado superior
    // 4. Dibujar controles táctiles si está en juego
    // 5. Dibujar mensaje temporal si hay
}
