#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>

// 📋 VERSIONES DEL FIRMWARE DE PS5
struct VersionFirmware {
    uint32_t mayor;
    uint32_t menor;
    uint32_t parche;
    std::string etiqueta;       // Ej: "24.01-08.50.00"
    std::string fecha;          // Fecha de lanzamiento
    uint64_t tamanoTotal;       // Tamaño del firmware en MB
};

// 📦 ARCHIVOS PRINCIPALES DEL FIRMWARE
struct ArchivoFirmware {
    std::string nombre;         // Nombre del archivo
    std::string ruta;          // Ruta dentro de la carpeta Firmware
    uint64_t tamano;           // Tamaño en bytes
    uint32_t hashCRC32;        // Verificación de integridad
    bool presente;             // Si existe en el disco
    bool valido;               // Si el hash coincide
    bool esCifrado;            // Si requiere descifrado
    std::string descripcion;   // Qué hace este archivo
};

// ⚙️ ESTADO DEL FIRMWARE
struct EstadoFirmware {
    bool cargado = false;
    bool inicializado = false;
    VersionFirmware version;
    std::string rutaCarpeta;
    std::vector<ArchivoFirmware> archivos;
    uint32_t archivosValidos = 0;
    uint32_t archivosTotales = 0;
    std::string mensajeError;
};

// 🔧 COMPONENTES DEL SISTEMA
enum class ComponenteSistema {
    KERNEL,         // 🧠 Núcleo principal — gestiona memoria, procesos, hardware
    RTOS,           // ⏰ Sistema en tiempo real — controla el procesador y gráficos
    SECURITY,       // 🔒 Módulo de seguridad — cifrado, firmas, claves
    IO_MANAGER,     // 💾 Entrada/salida — discos, USB, red, controladores
    AUDIO_SYSTEM,   // 🔊 Sistema de audio
    VIDEO_DRIVER,   // 🖼️ Controlador de gráficos
    NETWORK,        // 🌐 Red y conexiones
    INPUT_MGR,      // 🎮 Manejo de mandos y periféricos
    SETTINGS,       // ⚙️ Configuración del sistema
    BOOTLOADER      // 🚀 Primer programa que arranca la consola
};

// 🎯 FUNCIONES PRINCIPALES
// Verificar si la carpeta de firmware existe y tiene todos los archivos
EstadoFirmware verificarFirmware(const std::string& rutaCarpeta);

// Cargar el firmware en memoria y preparar todos los componentes
bool cargarFirmware(const std::string& rutaCarpeta);

// Inicializar cada componente del sistema (kernel, audio, vídeo, etc.)
bool inicializarComponentes();

// Obtener información de la versión instalada
VersionFirmware obtenerVersionFirmware();

// Buscar y listar todos los archivos del firmware
std::vector<ArchivoFirmware> escanearArchivosFirmware(const std::string& ruta);

// Verificar integridad de cada archivo (comprobar hashes)
bool verificarIntegridad(ArchivoFirmware& archivo);

// Descifrar archivos protegidos si es necesario
bool descifrarArchivo(ArchivoFirmware& archivo);

// Reiniciar el sistema completo (como apagar y prender la consola)
void reiniciarSistema();

// Obtener estado actual
EstadoFirmware obtenerEstadoFirmware();

// 🌍 Variable global
extern EstadoFirmware firmware;
