#pragma once

#include <string>

// Configuración general del emulador
struct Configuracion {
    // 📱 Gráficos y rendimiento
    int resolucionAncho = 1280;
    int resolucionAlto = 720;
    int escala = 100;
    bool vsync = true;
    int limiteFPS = 60;
    std::string nivelGraficos = "Medio";  // Bajo / Medio / Alto

    // 🔊 Audio
    bool audioHabilitado = true;
    int volumen = 100;
    int frecuenciaMuestreo = 48000;

    // 🎮 Controles
    bool controlesTactiles = true;
    bool vibracion = true;
    bool stickDerechoInvertido = false;
    bool stickIzquierdoInvertido = false;

    // 💾 Sistema
    bool guardarAutomatico = true;
    int tiempoAutoGuardado = 300;  // segundos
    std::string rutaJuegos = "/storage/emulated/0/RPCS5/Juegos/";
    std::string rutaGuardados = "/storage/emulated/0/RPCS5/Guardados/";

    // ⚙️ Avanzado
    bool modoRendimiento = false;
    bool emularPerfil = true;
    bool mostrarFPS = false;
    int idioma = 0;  // 0=Español, 1=Inglés, etc.
};

// Declaración global para acceder desde todo el emulador
extern Configuracion config;
