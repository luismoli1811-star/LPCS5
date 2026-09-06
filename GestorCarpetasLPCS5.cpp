#include "GestorCarpetas RPCS5.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <cstring>

// 🌍 Definición de las rutas globales
RutasSistema rutas;

// ✅ Verificar si una carpeta ya existe
bool carpetaExiste(const std::string& ruta) {
    struct stat info;
    if (stat(ruta.c_str(), &info) != 0) return false;
    return S_ISDIR(info.st_mode);
}

// ✅ Crear una sola carpeta
bool crearCarpeta(const std::string& ruta) {
    if (carpetaExiste(ruta)) return true;
    // Permisos: lectura, escritura, ejecución para el usuario
    int resultado = mkdir(ruta.c_str(), 0755);
    return resultado == 0;
}

// 🚀 CREAR TODA LA ESTRUCTURA DE UNA VEZ
bool crearEstructuraCarpetas() {
    bool exito = true;

    // 📁 Carpeta raíz
    if (!crearCarpeta(rutas.raiz)) exito = false;

    // 🎮 Juegos
    if (!crearCarpeta(rutas.juegos)) exito = false;

    // 💾 Guardados
    if (!crearCarpeta(rutas.guardados)) exito = false;

    // ⚙️ Configuración
    if (!crearCarpeta(rutas.config)) exito = false;

    // 📄 Sistema y subcarpetas
    if (!crearCarpeta(rutas.sistema)) exito = false;
    if (!crearCarpeta(rutas.firmware)) exito = false;
    if (!crearCarpeta(rutas.shaders)) exito = false;
    if (!crearCarpeta(rutas.cache)) exito = false;

    // 🖼️ Media
    if (!crearCarpeta(rutas.media)) exito = false;
    if (!crearCarpeta(rutas.portadas)) exito = false;
    if (!crearCarpeta(rutas.capturas)) exito = false;

    // 📂 Temporal
    if (!crearCarpeta(rutas.temporal)) exito = false;

    return exito;
}

// 📊 Contar archivos y calcular tamaño de una carpeta
static void escanearCarpeta(const std::string& ruta, long long& tamano, int& cantidadArchivos) {
    DIR* dir = opendir(ruta.c_str());
    if (!dir) return;

    struct dirent* entrada;
    while ((entrada = readdir(dir)) != nullptr) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
            continue;

        std::string rutaCompleta = ruta + "/" + entrada->d_name;
        struct stat info;
        if (stat(rutaCompleta.c_str(), &info) != 0) continue;

        if (S_ISDIR(info.st_mode)) {
            escanearCarpeta(rutaCompleta, tamano, cantidadArchivos);
        } else {
            tamano += info.st_size;
            cantidadArchivos++;
        }
    }
    closedir(dir);
}

// 📋 Obtener información completa de una carpeta
CarpetaInfo obtenerInfoCarpeta(const std::string& ruta) {
    CarpetaInfo info;
    info.ruta = ruta;
    info.existe = carpetaExiste(ruta);

    if (!info.existe) return info;

    // Extraer nombre de la carpeta
    size_t pos = ruta.find_last_of('/');
    info.nombre = (pos != std::string::npos) ? ruta.substr(pos + 1) : ruta;

    // Calcular tamaño y cantidad de archivos
    long long tamano = 0;
    int archivos = 0;
    escanearCarpeta(ruta, tamano, archivos);
    info.tamano = tamano;
    info.cantidadArchivos = archivos;

    // Descripción según el tipo
    if (ruta == rutas.juegos) info.descripcion = "Juegos de PS5 (.pkg, .iso)";
    else if (ruta == rutas.guardados) info.descripcion = "Partidas guardadas";
    else if (ruta == rutas.config) info.descripcion = "Ajustes y configuración";
    else if (ruta == rutas.firmware) info.descripcion = "Firmware del sistema";
    else if (ruta == rutas.shaders) info.descripcion = "Caché de gráficos";
    else if (ruta == rutas.temporal) info.descripcion = "Archivos temporales";
    else info.descripcion = "Carpeta de archivos";

    return info;
}

// 📈 Espacio total usado por el emulador
long long obtenerEspacioTotalUsado() {
    long long total = 0;
    int archivos = 0;
    escanearCarpeta(rutas.raiz, total, archivos);
    return total;
}

// 🧹 Borrar todo lo que hay en la carpeta temporal
bool limpiarTemporal() {
    DIR* dir = opendir(rutas.temporal.c_str());
    if (!dir) return false;

    struct dirent* entrada;
    bool todoBien = true;
    while ((entrada = readdir(dir)) != nullptr) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
            continue;

        std::string rutaCompleta = rutas.temporal + "/" + entrada->d_name;
        struct stat info;
        if (stat(rutaCompleta.c_str(), &info) != 0) continue;

        if (S_ISDIR(info.st_mode)) {
            // Borrar carpeta recursivamente
            // (Simplificado — en versión completa sería recursivo)
        } else {
            if (unlink(rutaCompleta.c_str()) != 0) todoBien = false;
        }
    }
    closedir(dir);
    return todoBien;
}

// 📂 Lista con información de todas las carpetas principales
std::vector<CarpetaInfo> listarTodasCarpetas() {
    std::vector<CarpetaInfo> lista;
    lista.push_back(obtenerInfoCarpeta(rutas.juegos));
    lista.push_back(obtenerInfoCarpeta(rutas.guardados));
    lista.push_back(obtenerInfoCarpeta(rutas.config));
    lista.push_back(obtenerInfoCarpeta(rutas.sistema));
    lista.push_back(obtenerInfoCarpeta(rutas.media));
    lista.push_back(obtenerInfoCarpeta(rutas.temporal));
    return lista;
}
