#include "ConfiguracionRPCS5.h"
#include <fstream>
#include <iostream>

// Definición de la variable global
Configuracion config;

// Ruta del archivo de configuración
const char* RUTA_CONFIG = "/storage/emulated/0/RPCS5/config.txt";

// Cargar valores por defecto
void cargarValoresPorDefecto() {
    // 📱 Gráficos
    config.resolucionAncho = 1280;
    config.resolucionAlto = 720;
    config.escala = 100;
    config.vsync = true;
    config.limiteFPS = 60;
    config.nivelGraficos = "Medio";

    // 🔊 Audio
    config.audioHabilitado = true;
    config.volumen = 100;
    config.frecuenciaMuestreo = 48000;

    // 🎮 Controles
    config.controlesTactiles = true;
    config.vibracion = true;
    config.stickDerechoInvertido = false;
    config.stickIzquierdoInvertido = false;

    // 💾 Sistema
    config.guardarAutomatico = true;
    config.tiempoAutoGuardado = 300;
    config.rutaJuegos = "/storage/emulated/0/RPCS5/Juegos/";
    config.rutaGuardados = "/storage/emulated/0/RPCS5/Guardados/";

    // ⚙️ Avanzado
    config.modoRendimiento = false;
    config.emularPerfil = true;
    config.mostrarFPS = false;
    config.idioma = 0;
}

// Guardar configuración en archivo
bool guardarConfiguracion() {
    std::ofstream archivo(RUTA_CONFIG);
    if (!archivo.is_open()) return false;

    // 📱 Gráficos
    archivo << "resolucionAncho=" << config.resolucionAncho << "\n";
    archivo << "resolucionAlto=" << config.resolucionAlto << "\n";
    archivo << "escala=" << config.escala << "\n";
    archivo << "vsync=" << config.vsync << "\n";
    archivo << "limiteFPS=" << config.limiteFPS << "\n";
    archivo << "nivelGraficos=" << config.nivelGraficos << "\n";

    // 🔊 Audio
    archivo << "audioHabilitado=" << config.audioHabilitado << "\n";
    archivo << "volumen=" << config.volumen << "\n";
    archivo << "frecuenciaMuestreo=" << config.frecuenciaMuestreo << "\n";

    // 🎮 Controles
    archivo << "controlesTactiles=" << config.controlesTactiles << "\n";
    archivo << "vibracion=" << config.vibracion << "\n";
    archivo << "stickDerechoInvertido=" << config.stickDerechoInvertido << "\n";
    archivo << "stickIzquierdoInvertido=" << config.stickIzquierdoInvertido << "\n";

    // 💾 Sistema
    archivo << "guardarAutomatico=" << config.guardarAutomatico << "\n";
    archivo << "tiempoAutoGuardado=" << config.tiempoAutoGuardado << "\n";
    archivo << "rutaJuegos=" << config.rutaJuegos << "\n";
    archivo << "rutaGuardados=" << config.rutaGuardados << "\n";

    // ⚙️ Avanzado
    archivo << "modoRendimiento=" << config.modoRendimiento << "\n";
    archivo << "emularPerfil=" << config.emularPerfil << "\n";
    archivo << "mostrarFPS=" << config.mostrarFPS << "\n";
    archivo << "idioma=" << config.idioma << "\n";

    archivo.close();
    return true;
}

// Cargar configuración desde archivo
bool cargarConfiguracion() {
    std::ifstream archivo(RUTA_CONFIG);
    if (!archivo.is_open()) {
        // Si no existe el archivo → cargar valores por defecto
        cargarValoresPorDefecto();
        guardarConfiguracion();
        return true;
    }

    std::string clave, valor;
    while (archivo.good() && std::getline(archivo, clave, '=') && std::getline(archivo, valor)) {
        if (clave == "resolucionAncho") config.resolucionAncho = std::stoi(valor);
        else if (clave == "resolucionAlto") config.resolucionAlto = std::stoi(valor);
        else if (clave == "escala") config.escala = std::stoi(valor);
        else if (clave == "vsync") config.vsync = (valor == "1");
        else if (clave == "limiteFPS") config.limiteFPS = std::stoi(valor);
        else if (clave == "nivelGraficos") config.nivelGraficos = valor;
        else if (clave == "audioHabilitado") config.audioHabilitado = (valor == "1");
        else if (clave == "volumen") config.volumen = std::stoi(valor);
        else if (clave == "frecuenciaMuestreo") config.frecuenciaMuestreo = std::stoi(valor);
        else if (clave == "controlesTactiles") config.controlesTactiles = (valor == "1");
        else if (clave == "vibracion") config.vibracion = (valor == "1");
        else if (clave == "stickDerechoInvertido") config.stickDerechoInvertido = (valor == "1");
        else if (clave == "stickIzquierdoInvertido") config.stickIzquierdoInvertido = (valor == "1");
        else if (clave == "guardarAutomatico") config.guardarAutomatico = (valor == "1");
        else if (clave == "tiempoAutoGuardado") config.tiempoAutoGuardado = std::stoi(valor);
        else if (clave == "rutaJuegos") config.rutaJuegos = valor;
        else if (clave == "rutaGuardados") config.rutaGuardados = valor;
        else if (clave == "modoRendimiento") config.modoRendimiento = (valor == "1");
        else if (clave == "emularPerfil") config.emularPerfil = (valor == "1");
        else if (clave == "mostrarFPS") config.mostrarFPS = (valor == "1");
        else if (clave == "idioma") config.idioma = std::stoi(valor);
    }

    archivo.close();
    return true;
}
