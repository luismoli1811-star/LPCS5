#pragma once

#include <cstdint>

// 🎮 BOTONES DEL MANDO DUALSENSE DE PS5
enum class BotonPS5 : uint32_t {
    // Botones principales
    TRIANGULO    = 1 << 0,  // ▲
    CIRCULO      = 1 << 1,  // ●
    CRUZ         = 1 << 2,  // ✕
    CUADRADO     = 1 << 3,  // □

    // Botones superiores / gatillos
    L1           = 1 << 4,
    R1           = 1 << 5,
    L2           = 1 << 6,
    R2           = 1 << 7,

    // Botones del centro
    COMPARTIR    = 1 << 8,
    OPCIONES     = 1 << 9,
    L3           = 1 << 10,  // Stick izquierdo presionado
    R3           = 1 << 11,  // Stick derecho presionado
    PS           = 1 << 12,  // Botón PlayStation
    CREAR        = 1 << 13,  // Botón "Crear" / Captura
    MUTE         = 1 << 14,  // Silenciar micrófono

    // Cruceta (direcciones)
    ARRIBA       = 1 << 16,
    ABAJO        = 1 << 17,
    IZQUIERDA    = 1 << 18,
    DERECHA      = 1 << 19
};

// 🎮 VALORES DE LOS STICKS ANALÓGICOS
struct Stick {
    float x = 0.0f;  // -1.0 = izquierda, +1.0 = derecha
    float y = 0.0f;  // -1.0 = abajo,   +1.0 = arriba
};

// 🔫 VALORES DE GATILLOS ANALÓGICOS (L2 y R2 tienen recorrido variable)
struct Gatillo {
    float valor = 0.0f;  // 0.0 = sin presionar, 1.0 = presionado al máximo
};

// 📱 ESTRUCTURA COMPLETA DEL ESTADO DEL MANDO
struct EstadoMando {
    uint32_t botones = 0;  // Almacena todos los botones presionados

    Stick stickIzquierdo;
    Stick stickDerecho;

    Gatillo l2;
    Gatillo r2;

    // 📱 Controles táctiles en pantalla (solo para Android)
    bool mostrarTactiles = true;
    float posicionStickIzquierdoX = 0.15f;   // Posición en pantalla (0.0 - 1.0)
    float posicionStickIzquierdoY = 0.80f;
    float posicionStickDerechoX = 0.85f;
    float posicionStickDerechoY = 0.80f;
    float tamanoBotones = 0.12f;             // Tamaño de los botones táctiles
    float zonaMuerta = 0.10f;                // Sensibilidad mínima del stick
};

// 🎯 FUNCIONES BÁSICAS
bool botonPresionado(EstadoMando& estado, BotonPS5 boton);
void establecerBoton(EstadoMando& estado, BotonPS5 boton, bool presionado);
void reiniciarMando(EstadoMando& estado);

// 🌍 Variable global para el mando principal
extern EstadoMando mandoPrincipal;
