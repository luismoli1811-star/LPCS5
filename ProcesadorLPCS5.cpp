#include "ProcesadorRPCS5.h"
#include <cstring>
#include <algorithm>

// 🌍 Variables globales
EstadoProcesador cpu;
ModoEjecucion modoEjecucion = ModoEjecucion::EMULADO;

// ✅ INICIALIZAR — poner todo en cero al encender
bool inicializarProcesador() {
    // Limpiar todos los registros
    std::memset(&cpu.registros, 0, sizeof(RegistrosCPU));

    // Valores iniciales realistas de la consola al encender
    cpu.registros.pc = 0x00000000;  // Empieza en la dirección de arranque
    cpu.registros.sp = 0x1FFFFFFF;  // Pila al final de la memoria
    cpu.registros.cpsr = 0x40000000; // Estado inicial
    cpu.registros.z_flag = false;
    cpu.registros.n_flag = false;
    cpu.registros.c_flag = false;
    cpu.registros.v_flag = false;

    // Limpiar registros vectoriales
    for (int i = 0; i < 32; i++) {
        std::memset(cpu.registros.v[i], 0, 16);
    }

    // Estado del procesador
    cpu.ejecutando = false;
    cpu.pausado = false;
    cpu.ciclosEjecutados = 0;
    cpu.velocidadMHz = 3500;
    cpu.nucleosActivos = 8;

    modoEjecucion = ModoEjecucion::EMULADO;

    return true;
}

// 🔄 REINICIAR — igual que apagar y prender
void reiniciarProcesador() {
    detenerProcesador();
    inicializarProcesador();
}

// 📂 CARGAR EJECUTABLE — leer el código del juego en memoria
bool cargarEjecutable(const char* rutaArchivo, uint64_t direccionCarga) {
    // ⚠️ Aquí se leería el archivo .pkg/.elf/.so y cargaría cada sección
    // En versión completa: analizar encabezados, mapear memoria, aplicar reubicaciones

    // Por ahora: simular carga exitosa
    cpu.registros.pc = direccionCarga; // El contador de programa apunta al inicio del juego
    return true;
}

// ▶️ BUCLE PRINCIPAL — el corazón que corre todo el tiempo
void bucleEjecucion() {
    cpu.ejecutando = true;
    cpu.pausado = false;

    while (cpu.ejecutando) {
        // ⏸️ Si está pausado, esperar sin gastar CPU
        while (cpu.pausado && cpu.ejecutando) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        if (!cpu.ejecutando) break;

        // ⚡ Ejecutar según el modo
        if (modoEjecucion == ModoEjecucion::INTERPRETADO) {
            // 📖 Modo seguro: leer y ejecutar instrucción por instrucción
            // (aquí iría el intérprete completo en versión final)
            cpu.ciclosEjecutados += 1;
            cpu.registros.pc += 4; // Avanzar 1 instrucción (4 bytes)
        }
        else if (modoEjecucion == ModoEjecucion::EMULADO) {
            // 🧩 Modo rápido: traducción dinámica JIT
            // (aquí se llamaría al código traducido a ARM64)
            cpu.ciclosEjecutados += 8; // Más ciclos por iteración = más rápido
            cpu.registros.pc += 4;
        }
        else if (modoEjecucion == ModoEjecucion::NATIVO) {
            // ⚡ Modo máximo: código nativo ARM64 compilado
            cpu.ciclosEjecutados += 32; // ¡El más rápido de todos!
            cpu.registros.pc += 4;
        }

        // 🧠 Limitar velocidad para no sobrecargar el celular
        // ~3.5 GHz emulado → controlamos el tiempo por ciclo
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

// ⏹️ DETENER — parar la emulación de forma segura
void detenerProcesador() {
    cpu.ejecutando = false;
    cpu.pausado = false;
}

// ⏸️ PAUSAR / ▶️ REANUDAR
void pausarProcesador() {
    cpu.pausado = true;
}

void reanudarProcesador() {
    cpu.pausado = false;
}

// 🔄 CAMBIAR MODO DE EJECUCIÓN
void setModoEjecucion(ModoEjecucion modo) {
    modoEjecucion = modo;
}

ModoEjecucion getModoEjecucion() {
    return modoEjecucion;
}

// 📥 LEER REGISTRO
uint64_t getRegistro(int indice) {
    if (indice >= 0 && indice < 32) {
        return cpu.registros.x[indice];
    }
    return 0;
}

// 📤 ESCRIBIR REGISTRO
void setRegistro(int indice, uint64_t valor) {
    if (indice >= 0 && indice < 32) {
        cpu.registros.x[indice] = valor;
    }
}

// 📍 CONTADOR DE PROGRAMA — dirección actual de ejecución
uint64_t getContadorPrograma() {
    return cpu.registros.pc;
}

void setContadorPrograma(uint64_t direccion) {
    cpu.registros.pc = direccion;
}
