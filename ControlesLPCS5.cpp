#include "ControlesRPCS5.h"
#include <cmath>

// 🎮 Definición del mando principal (variable global)
EstadoMando mandoPrincipal;

// ⚙️ Verificar si un botón está presionado
bool botonPresionado(EstadoMando& estado, BotonPS5 boton) {
    return (estado.botones & static_cast<uint32_t>(boton)) != 0;
}

// ⚙️ Presionar o soltar un botón
void establecerBoton(EstadoMando& estado, BotonPS5 boton, bool presionado) {
    if (presionado) {
        estado.botones |= static_cast<uint32_t>(boton);   // Poner a 1 → presionar
    } else {
        estado.botones &= ~static_cast<uint32_t>(boton);  // Poner a 0 → soltar
    }
}

// 🔄 Poner todo en cero — soltar todos los botones, centrar sticks
void reiniciarMando(EstadoMando& estado) {
    estado.botones = 0;

    // Sticks al centro
    estado.stickIzquierdo.x = 0.0f;
    estado.stickIzquierdo.y = 0.0f;
    estado.stickDerecho.x = 0.0f;
    estado.stickDerecho.y = 0.0f;

    // Gatillos sin presionar
    estado.l2.valor = 0.0f;
    estado.r2.valor = 0.0f;
}

// 📱 PROCESAR TOQUES EN PANTALLA — para controles táctiles
void procesarToquePantalla(EstadoMando& estado, float x, float y, int id, bool presionando) {
    if (!estado.mostrarTactiles) return;

    // Tamaño de pantalla normalizado (0.0 a 1.0)
    const float zona = estado.tamanoBotones;

    // 🕹️ Stick Izquierdo
    float sx = estado.posicionStickIzquierdoX;
    float sy = estado.posicionStickIzquierdoY;
    if (std::abs(x - sx) < zona && std::abs(y - sy) < zona) {
        if (presionando) {
            // Convertir posición del dedo a valor de stick (-1.0 a +1.0)
            estado.stickIzquierdo.x = (x - sx) / zona * 2.0f;
            estado.stickIzquierdo.y = (sy - y) / zona * 2.0f;  // Invertir eje Y
        } else {
            // Soltar → volver al centro
            estado.stickIzquierdo.x = 0.0f;
            estado.stickIzquierdo.y = 0.0f;
        }
        return;
    }

    // 🕹️ Stick Derecho
    float sdx = estado.posicionStickDerechoX;
    float sdy = estado.posicionStickDerechoY;
    if (std::abs(x - sdx) < zona && std::abs(y - sdy) < zona) {
        if (presionando) {
            estado.stickDerecho.x = (x - sdx) / zona * 2.0f;
            estado.stickDerecho.y = (sdy - y) / zona * 2.0f;
        } else {
            estado.stickDerecho.x = 0.0f;
            estado.stickDerecho.y = 0.0f;
        }
        return;
    }

    // 🔘 Botones de acción (Triángulo, Círculo, Cruz, Cuadrado)
    float botonDistancia = zona * 0.6f;
    // Triángulo — arriba-derecha
    if (std::abs(x - (sdx + botonDistancia)) < botonDistancia &&
        std::abs(y - (sdy - botonDistancia * 1.5f)) < botonDistancia) {
        establecerBoton(estado, BotonPS5::TRIANGULO, presionando);
        return;
    }
    // Círculo
    if (std::abs(x - (sdx + botonDistancia * 2.0f)) < botonDistancia &&
        std::abs(y - sdy) < botonDistancia) {
        establecerBoton(estado, BotonPS5::CIRCULO, presionando);
        return;
    }
    // Cruz
    if (std::abs(x - sdx) < botonDistancia &&
        std::abs(y - (sdy + botonDistancia * 1.5f)) < botonDistancia) {
        establecerBoton(estado, BotonPS5::CRUZ, presionando);
        return;
    }
    // Cuadrado
    if (std::abs(x - (sdx - botonDistancia)) < botonDistancia &&
        std::abs(y - sdy) < botonDistancia) {
        establecerBoton(estado, BotonPS5::CUADRADO, presionando);
        return;
    }
}

// 🧹 Aplicar zona muerta a los sticks — evita movimiento por toques leves
void aplicarZonaMuerta(Stick& stick, float zonaMuerta) {
    if (std::abs(stick.x) < zonaMuerta) stick.x = 0.0f;
    if (std::abs(stick.y) < zonaMuerta) stick.y = 0.0f;

    // Normalizar para que llegue al 100% fuera de la zona muerta
    float r = std::sqrt(stick.x * stick.x + stick.y * stick.y);
    if (r > zonaMuerta && r > 0.01f) {
        float factor = (r - zonaMuerta) / (1.0f - zonaMuerta);
        stick.x = std::clamp(stick.x / r * factor, -1.0f, 1.0f);
        stick.y = std::clamp(stick.y / r * factor, -1.0f, 1.0f);
    } else {
        stick.x = 0.0f;
        stick.y = 0.0f;
    }
}

// 🔄 ACTUALIZAR TODO — llamar cada cuadro antes de enviar al emulador
void actualizarControles(EstadoMando& estado) {
    aplicarZonaMuerta(estado.stickIzquierdo, estado.zonaMuerta);
    aplicarZonaMuerta(estado.stickDerecho, estado.zonaMuerta);
}
