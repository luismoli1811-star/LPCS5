#pragma once

#include <string>
#include <vector>
#include <cstdint>

// 🎮 FORMATOS DE JUEGOS SOPORTADOS
enum class FormatoJuego {
    DESCONOCIDO,
    PKG,        // 📦 Paquete oficial de PS5 — formato estándar
    ISO,        // 📀 Imagen de disco
    CARPETA,    // 📂 Juego extraído — archivos sueltos
    PS5         // Archivo propio o formato comprimido
};

// 📋 INFORMACIÓN COMPLETA DE UN JUEGO
struct JuegoInfo {
    std::string ruta;              // Ruta completa del archivo/carpeta
    std::string nombre;            // Nombre del juego
    std::string id;                // ID único del juego (ej: PPSA01234)
    std::string titulo;            // Título oficial
    std::string version;           // Versión del juego
    std::string region;            // Región: USA, EUR, JPN, etc.
    std::string descripcion;       // Descripción breve
    std::string portadaRuta;       // Ruta de la imagen de portada
    uint64_t tamano = 0;           // Tamaño total en bytes
    uint32_t codigoBloqueo = 0;    // Si requiere descifrado
    FormatoJuego formato = FormatoJuego::DESCONOCIDO;
    bool esValido = false;         // Si el archivo es válido
    bool estaCifrado = false;      // Si necesita clave para leerlo
    bool favorito = false;         // Marcado como favorito
    int vecesJugado = 0;           // Contador de partidas
    uint64_t ultimaPartida = 0;    // Última vez jugado (timestamp)
};

// 📊 ESTADO DEL CARGADOR
struct EstadoCarga {
    bool cargando = false;
    int progreso = 0;              // 0 - 100%
    std::string mensaje;           // Qué está haciendo
    bool cancelado = false;
};

// 🎯 FUNCIONES PRINCIPALES
// Escanear carpeta de juegos y devolver lista completa
std::vector<JuegoInfo> escanearCarpetaJuegos(const std::string& ruta);

// Identificar el formato de un archivo
FormatoJuego detectarFormato(const std::string& ruta);

// Leer información básica del juego (nombre, ID, versión, etc.)
bool leerInfoJuego(JuegoInfo& juego);

// Cargar el juego en memoria — preparar para emulación
bool cargarJuego(const JuegoInfo& juego);

// Descargar/extraer archivos si es necesario
bool prepararArchivosJuego(const JuegoInfo& juego);

// Obtener extensión como cadena
std::string formatoAString(FormatoJuego formato);

// Buscar portada automáticamente en carpeta de media
std::string buscarPortada(const JuegoInfo& juego);

// 🌍 Variables globales
extern std::vector<JuegoInfo> listaJuegos;
extern EstadoCarga estadoCarga;
extern JuegoInfo juegoActual;
