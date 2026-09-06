#pragma once

#include <cstdint>
#include <atomic>
#include <thread>
#include <vector>

// 🧠 ARQUITECTURA DE LA CPU DE PS5 — AMD Zen 2 (x86-64)
// Para Android/ARM64 → traduciremos las instrucciones dinámicamente

// 📊 REGISTROS DE LA CPU — estado completo del procesador
struct RegistrosCPU {
    // 📦 Registros de propósito general (x0 - x31) — 64 bits
    uint64_t x[32];

    // 🔬 Registros especiales
    uint64_t pc;        // Contador de programa — dirección actual
    uint64_t sp;        // Puntero de pila
    uint64_t lr;        // Registro de enlace (retorno de funciones)
    uint64_t fp;        // Puntero de marco

    // ⚙️ Registro de estado (flags)
    uint32_t cpsr;      // Registro de estado del programa
    bool z_flag;        // Flag Cero
    bool n_flag;        // Flag Negativo
    bool c_flag;        // Flag Acarreo
    bool v_flag;        // Flag Desbordamiento

    // 🎮 SIMD / Vector — gráficos y cálculos rápidos (128 bits cada uno)
    alignas(16) uint8_t v[32][16];  // Registros vectoriales V0 - V31
};

// ⚡ ESTADO DEL PROCESADOR
struct EstadoProcesador {
    RegistrosCPU registros;
    std::atomic<bool> ejecutando{false};
    std::atomic<bool> pausado{false};
    uint64_t ciclosEjecutados = 0;
    uint32_t velocidadMHz = 3500;  // Frecuencia base de PS5: 3.5 GHz
    int nucleosActivos = 8;         // 8 núcleos Zen 2
};

// 🔧 MODOS DE EJECUCIÓN
enum class ModoEjecucion {
    NATIVO,         // ⚡ Código ARM64 nativo — rápido
    EMULADO,        // 🧩 Traducción dinámica — compatible
    INTERPRETADO    // 📖 Lectura paso a paso — lento pero seguro
};

// 🎯 FUNCIONES PRINCIPALES
// Inicializar el procesador y poner todos los registros en cero
bool inicializarProcesador();

// Reiniciar el procesador — igual que al encender la consola
void reiniciarProcesador();

// Cargar el código del juego en memoria y empezar a ejecutar
bool cargarEjecutable(const char* rutaArchivo, uint64_t direccionCarga);

// Bucle principal de emulación — corre en un hilo aparte
void bucleEjecucion();

// Detener la ejecución de forma segura
void detenerProcesador();

// Pausar / reanudar
void pausarProcesador();
void reanudarProcesador();

// Cambiar modo de ejecución
void setModoEjecucion(ModoEjecucion modo);
ModoEjecucion getModoEjecucion();

// Leer/escribir registros
uint64_t getRegistro(int indice);
void setRegistro(int indice, uint64_t valor);
uint64_t getContadorPrograma();
void setContadorPrograma(uint64_t direccion);

// 🌍 Variable global
extern EstadoProcesador cpu;
extern ModoEjecucion modoEjecucion;
