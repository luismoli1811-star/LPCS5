#pragma once

#include <string>
#include <vector>

// 📁 ESTRUCTURA COMPLETA DE RUTAS DEL EMULADOR
struct RutasSistema {
    // 📱 Carpeta principal del emulador en el celular
    std::string raiz = "/storage/emulated/0/RPCS5/";

    // 🎮 Juegos — archivos .pkg, .iso, .ps5
    std::string juegos = "/storage/emulated/0/RPCS5/Juegos/";

    // 💾 Partidas guardadas
    std::string guardados = "/storage/emulated/0/RPCS5/Guardados/";

    // ⚙️ Configuración del emulador
    std::string config = "/storage/emulated/0/RPCS5/Config/";

    // 📄 Archivos de sistema — firmware, caché, shaders
    std::string sistema = "/storage/emulated/0/RPCS5/Sistema/";
    std::string firmware = "/storage/emulated/0/RPCS5/Sistema/Firmware/";
    std::string shaders = "/storage/emulated/0/RPCS5/Sistema/Shaders/";
    std::string cache = "/storage/emulated/0/RPCS5/Sistema/Cache/";

    // 🖼️ Portadas, íconos, imágenes de juegos
    std::string media = "/storage/emulated/0/RPCS5/Media/";
    std::string portadas = "/storage/emulated/0/RPCS5/Media/Portadas/";
    std::string capturas = "/storage/emulated/0/RPCS5/Media/Capturas/";

    // 📂 Carpeta temporal — archivos que se borran al cerrar
    std::string temporal = "/storage/emulated/0/RPCS5/Temporal/";
};

// 📂 INFORMACIÓN DE CADA CARPETA
struct CarpetaInfo {
    std::string ruta;
    std::string nombre;
    std::string descripcion;
    bool existe = false;
    long long tamano = 0;       // Tamaño en bytes
    int cantidadArchivos = 0;
};

// 🎯 FUNCIONES PRINCIPALES
// Crear TODAS las carpetas si no existen
bool crearEstructuraCarpetas();

// Verificar si una carpeta existe
bool carpetaExiste(const std::string& ruta);

// Crear una sola carpeta
bool crearCarpeta(const std::string& ruta);

// Obtener información de una carpeta (tamaño, archivos, etc.)
CarpetaInfo obtenerInfoCarpeta(const std::string& ruta);

// Calcular espacio total usado por el emulador
long long obtenerEspacioTotalUsado();

// Borrar carpeta temporal
bool limpiarTemporal();

// Obtener lista de todas las carpetas del sistema
std::vector<CarpetaInfo> listarTodasCarpetas();

// 🌍 Variable global con todas las rutas
extern RutasSistema rutas;
