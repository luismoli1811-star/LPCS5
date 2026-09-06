#include "MemoriaRPCS5.h"
#include <cstring>
#include <algorithm>
#include <cstdio>

// 🌍 Variable global del gestor de memoria
EstadoMemoria memoria;

// ✅ INICIALIZAR — crear los buffers reales de memoria
bool inicializarMemoria() {
    // Limpiar estado
    std::memset(&memoria, 0, sizeof(EstadoMemoria));
    memoria.inicializada = false;

    // 🧠 Asignar RAM principal — 14 GB (memoria compartida)
    memoria.ramPrincipal = new uint8_t[MEM_TAMANO_RAM];
    if (!memoria.ramPrincipal) {
        std::fprintf(stderr, "❌ No se pudo asignar RAM principal\n");
        return false;
    }
    std::memset(memoria.ramPrincipal, 0, MEM_TAMANO_RAM);

    // 🎮 Asignar memoria de vídeo — 2 GB (GPU)
    memoria.memoriaVideo = new uint8_t[MEM_TAMANO_VRAM];
    if (!memoria.memoriaVideo) {
        std::fprintf(stderr, "❌ No se pudo asignar memoria de vídeo\n");
        delete[] memoria.ramPrincipal;
        memoria.ramPrincipal = nullptr;
        return false;
    }
    std::memset(memoria.memoriaVideo, 0, MEM_TAMANO_VRAM);

    memoria.usadaBytes = 0;
    memoria.inicializada = true;

    return true;
}

// 🗑️ LIBERAR — devolver toda la memoria al sistema
void liberarMemoria() {
    if (memoria.ramPrincipal) {
        delete[] memoria.ramPrincipal;
        memoria.ramPrincipal = nullptr;
    }
    if (memoria.memoriaVideo) {
        delete[] memoria.memoriaVideo;
        memoria.memoriaVideo = nullptr;
    }
    memoria.bloques.clear();
    memoria.usadaBytes = 0;
    memoria.inicializada = false;
}

// 🔍 TRADUCIR — dirección virtual → puntero real en el celular
uint8_t* traducirDireccion(uint64_t direccion, uint64_t* tamanoDisponible) {
    if (!memoria.inicializada) return nullptr;

    // 🧠 RAM Principal — 0x0000000000000000 a 0x000000037FFFFFFF
    if (direccion >= MEM_BASE_RAM && direccion < MEM_BASE_RAM + MEM_TAMANO_RAM) {
        uint64_t desplazamiento = direccion - MEM_BASE_RAM;
        if (tamanoDisponible) *tamanoDisponible = MEM_TAMANO_RAM - desplazamiento;
        return memoria.ramPrincipal + desplazamiento;
    }

    // 🎮 Memoria de Vídeo — 0x0000000380000000 a 0x00000003FFFFFFFF
    if (direccion >= MEM_BASE_VRAM && direccion < MEM_BASE_VRAM + MEM_TAMANO_VRAM) {
        uint64_t desplazamiento = direccion - MEM_BASE_VRAM;
        if (tamanoDisponible) *tamanoDisponible = MEM_TAMANO_VRAM - desplazamiento;
        return memoria.memoriaVideo + desplazamiento;
    }

    // ⚠️ Hardware / Registros — no hay buffer directo, se maneja por separado
    if (direccion >= MEM_BASE_HARDWARE && direccion < MEM_BASE_HARDWARE + MEM_TAMANO_HARDWARE) {
        if (tamanoDisponible) *tamanoDisponible = 0;
        return nullptr;
    }

    // ❌ Dirección fuera de rango
    return nullptr;
}

// 📦 ASIGNAR BLOQUE — reservar una región con permisos
uint64_t asignarBloque(uint64_t direccion, uint64_t tamano,
                        uint32_t permisos, const std::string& etiqueta) {
    if (!memoria.inicializada) return 0;

    // Si no se especifica dirección, buscar espacio libre
    uint64_t direccionAsignar = direccion;
    if (direccionAsignar == 0) {
        direccionAsignar = MEM_BASE_RAM + 0x100000; // Empezar después de zonas reservadas
        for (const auto& bloque : memoria.bloques) {
            uint64_t finBloque = bloque.direccionBase + bloque.tamano;
            if (finBloque > direccionAsignar) {
                direccionAsignar = finBloque;
            }
        }
    }

    // Crear y registrar el bloque
    BloqueMemoria nuevo;
    nuevo.direccionBase = direccionAsignar;
    nuevo.tamano = tamano;
    nuevo.permisos = permisos;
    nuevo.etiqueta = etiqueta;
    nuevo.datos = traducirDireccion(direccionAsignar);
    nuevo.asignado = true;

    memoria.bloques.push_back(nuevo);
    memoria.usadaBytes += tamano;

    return direccionAsignar;
}

// 🗑️ LIBERAR BLOQUE
bool liberarBloque(uint64_t direccion) {
    for (auto it = memoria.bloques.begin(); it != memoria.bloques.end(); ++it) {
        if (it->direccionBase == direccion && it->asignado) {
            memoria.usadaBytes -= it->tamano;
            memoria.bloques.erase(it);
            return true;
        }
    }
    return false;
}

// 📖 LEER — 8 bits (1 byte)
uint8_t leer8(uint64_t direccion) {
    uint8_t* ptr = traducirDireccion(direccion);
    if (!ptr) return 0;
    return *ptr;
}

// 📖 LEER — 16 bits (2 bytes)
uint16_t leer16(uint64_t direccion) {
    uint8_t* ptr = traducirDireccion(direccion);
    if (!ptr) return 0;
    return static_cast<uint16_t>(ptr[0]) |
           (static_cast<uint16_t>(ptr[1]) << 8);
}

// 📖 LEER — 32 bits (4 bytes)
uint32_t leer32(uint64_t direccion) {
    uint8_t* ptr = traducirDireccion(direccion);
    if (!ptr) return 0;
    return static_cast<uint32_t>(ptr[0]) |
           (static_cast<uint32_t>(ptr[1]) << 8) |
           (static_cast<uint32_t>(ptr[2]) << 16) |
           (static_cast<uint32_t>(ptr[3]) << 24);
}

// 📖 LEER — 64 bits (8 bytes)
uint64_t leer64(uint64_t direccion) {
    uint8_t* ptr = traducirDireccion(direccion);
    if (!ptr) return 0;
    return static_cast<uint64_t>(ptr[0]) |
           (static_cast<uint64_t>(ptr[1]) << 8) |
           (static_cast<uint64_t>(ptr[2]) << 16) |
           (static_cast<uint64_t>(ptr[3]) << 24) |
           (static_cast<uint64_t>(ptr[4]) << 32) |
           (static_cast<uint64_t>(ptr[5]) << 40) |
           (static_cast<uint64_t>(ptr[6]) << 48) |
           (static_cast<uint64_t>(ptr[7]) << 56);
}

// 📖 LEER BLOQUE — copiar varios bytes
size_t leerBloque(uint64_t direccion, void* destino, size_t tamano) {
    uint64_t disponible = 0;
    uint8_t* ptr = traducirDireccion(direccion, &disponible);
    if (!ptr || disponible == 0) return 0;

    size_t copiar = std::min(tamano, static_cast<size_t>(disponible));
    std::memcpy(destino, ptr, copiar);
    return copiar;
}

// ✏️ ESCRIBIR — 8 bits
void escribir8(uint64_t direccion, uint8_t valor) {
    uint8_t* ptr = traducirDireccion(direccion);
    if (!ptr) return;
    *ptr = valor;
}

// ✏️ ESCRIBIR — 16 bits
void escribir16(uint64_t direccion, uint16_t valor) {
    uint8_t* ptr = traducirDireccion(direccion);
    if (!ptr) return;
    ptr[0] = static_cast<uint8_t>(valor & 0xFF);
    ptr[1] = static_cast<uint8_t>((valor >> 8) & 0xFF);
}

// ✏️ ESCRIBIR — 32 bits
void escribir32(uint64_t direccion, uint32_t valor) {
    uint8_t* ptr = traducirDireccion(direccion);
    if (!ptr) return;
    ptr[0] = static_cast<uint8_t>(valor & 0xFF);
    ptr[1] = static_cast<uint8_t>((valor >> 8) & 0xFF);
    ptr[2] = static_cast<uint8_t>((valor >> 16) & 0xFF);
    ptr[3] = static_cast<uint8_t>((valor >> 24) & 0xFF);
}

// ✏️ ESCRIBIR — 64 bits
void escribir64(uint64_t direccion, uint64_t valor) {
    uint8_t* ptr = traducirDireccion(direccion);
    if (!ptr) return;
    ptr[0] = static_cast<uint8_t>(valor & 0xFF);
    ptr[1] = static_cast<uint8_t>((valor >> 8) & 0xFF);
    ptr[2] = static_cast<uint8_t>((valor >> 16) & 0xFF);
    ptr[3] = static_cast<uint8_t>((valor >> 24) & 0xFF);
    ptr[4] = static_cast<uint8_t>((valor >> 32) & 0xFF);
    ptr[5] = static_cast<uint8_t>((valor >> 40) & 0xFF);
    ptr[6] = static_cast<uint8_t>((valor >> 48) & 0xFF);
    ptr[7] = static_cast<uint8_t>((valor >> 56) & 0xFF);
}

// ✏️ ESCRIBIR BLOQUE — copiar varios bytes
size_t escribirBloque(uint64_t direccion, const void* origen, size_t tamano) {
    uint64_t disponible = 0;
    uint8_t* ptr = traducirDireccion(direccion, &disponible);
    if (!ptr || disponible == 0) return 0;

    size_t copiar = std::min(tamano, static_cast<size_t>(disponible));
    std::memcpy(ptr, origen, copiar);
    return copiar;
}

// 📊 ESTADÍSTICAS
EstadoMemoria obtenerEstadoMemoria() {
    return memoria;
}

uint64_t memoriaLibre() {
    if (!memoria.inicializada) return 0;
    return (MEM_TAMANO_RAM + MEM_TAMANO_VRAM) - memoria.usadaBytes;
}

uint64_t memoriaUsada() {
    return memoria.usadaBytes;
}
