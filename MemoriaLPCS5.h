#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

// 📊 MAPA DE MEMORIA DE PS5 — 16 GB GDDR6 total (compartida entre CPU y GPU)
// Direcciones virtuales → lo que ve el procesador del juego

// 📍 LÍMITES DE MEMORIA (en direcciones de 64 bits)
constexpr uint64_t MEM_TAMANO_TOTAL   = 0x400000000;  // 16 GB exactos
constexpr uint64_t MEM_BASE_RAM       = 0x0000000000000000; // Memoria principal
constexpr uint64_t MEM_TAMANO_RAM     = 0x380000000;  // 14 GB para juegos y sistema
constexpr uint64_t MEM_BASE_VRAM      = 0x0000000380000000; // Gráficos (GPU)
constexpr uint64_t MEM_TAMANO_VRAM    = 0x080000000;  // 2 GB exclusivos para GPU
constexpr uint64_t MEM_BASE_HARDWARE  = 0x0000000F00000000; // Registros de hardware
constexpr uint64_t MEM_TAMANO_HARDWARE= 0x000010000000; // 256 MB
constexpr uint64_t MEM_BASE_STACK     = 0x00000001F0000000; // Pila del sistema
constexpr uint64_t MEM_TAMANO_STACK   = 0x00100000;   // 16 MB

// 🔧 PERMISOS DE PÁGINA
enum class PermisosMemoria : uint32_t {
    NINGUNO   = 0b000,
    LECTURA   = 0b001,
    ESCRITURA = 0b010,
    EJECUCION = 0b100,
    LECTURA_ESCRITURA = LECTURA | ESCRITURA,
    LECTURA_EJECUCION  = LECTURA | EJECUCION,
    TODOS     = LECTURA | ESCRITURA | EJECUCION
};

// 📦 BLOQUE DE MEMORIA — cada región asignada
struct BloqueMemoria {
    uint64_t direccionBase;
    uint64_t tamano;
    uint32_t permisos;
    std::string etiqueta;    // Nombre para depurar
    uint8_t* datos;          // Puntero al buffer real
    bool asignado;
};

// ⚙️ ESTADO COMPLETO DEL GESTOR DE MEMORIA
struct EstadoMemoria {
    uint8_t* ramPrincipal;    // Buffer de 14 GB — memoria principal
    uint8_t* memoriaVideo;    // Buffer de 2 GB — memoria de gráficos
    std::vector<BloqueMemoria> bloques; // Lista de regiones asignadas
    uint64_t usadaBytes;      // Cuánta memoria está ocupada
    bool inicializada;
};

// 🎯 FUNCIONES PRINCIPALES
// Inicializar todo el espacio de memoria — crear los buffers reales
bool inicializarMemoria();

// Liberar toda la memoria al cerrar
void liberarMemoria();

// Asignar un bloque de memoria con permisos
uint64_t asignarBloque(uint64_t direccion, uint64_t tamano,
                        uint32_t permisos, const std::string& etiqueta = "");

// Liberar un bloque asignado
bool liberarBloque(uint64_t direccion);

// 📖 LEER — 8, 16, 32 y 64 bits + bloques
uint8_t  leer8(uint64_t direccion);
uint16_t leer16(uint64_t direccion);
uint32_t leer32(uint64_t direccion);
uint64_t leer64(uint64_t direccion);
size_t   leerBloque(uint64_t direccion, void* destino, size_t tamano);

// ✏️ ESCRIBIR — 8, 16, 32 y 64 bits + bloques
void escribir8(uint64_t direccion, uint8_t valor);
void escribir16(uint64_t direccion, uint16_t valor);
void escribir32(uint64_t direccion, uint32_t valor);
void escribir64(uint64_t direccion, uint64_t valor);
size_t escribirBloque(uint64_t direccion, const void* origen, size_t tamano);

// 🔍 Traducir dirección virtual → puntero real en el celular
uint8_t* traducirDireccion(uint64_t direccion, uint64_t* tamanoDisponible = nullptr);

// 📊 Obtener información y estadísticas
EstadoMemoria obtenerEstadoMemoria();
uint64_t memoriaLibre();
uint64_t memoriaUsada();

// 🌍 Variable global
extern EstadoMemoria memoria;
