// ==============================================================
// RPCS5 — Emulador de PlayStation 5 para Android
// Archivo principal: Punto de entrada de la aplicación
// ==============================================================

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

// 📦 INCLUIR TODOS LOS MÓDULOS DEL SISTEMA
#include "MemoriaRPCS5.h"
#include "ModuloFirmwareRPCS5.h"
#include "ProcesadorRPCS5.h"
#include "GraficaRPCS5.h"
#include "AudioRPCS5.h"
#include "ControlesRPCS5.h"
#include "GestorCarpetasRPCS5.h"
#include "ConfiguracionRPCS5.h"

// 📌 VERSIÓN DEL PROYECTO
const std::string NOMBRE_APP    = "RPCS5";
const std::string VERSION_APP   = "0.1.0-Experimental";
const std::string FECHA_COMPILACION = __DATE__;
const std::string HORA_COMPILACION  = __TIME__;

// ⚙️ ESTADO GLOBAL DEL EMULADOR
struct EstadoEmulador {
    bool estaCorriendo = false;
    bool firmwareCargado = false;
    bool juegoCargado = false;
    bool sistemaInicializado = false;

    std::string rutaFirmware;
    std::string rutaJuego;
    std::string mensajeEstado;

    uint32_t cuadrosPorSegundo = 0;
    uint64_t tiempoEjecucionMs = 0;
} estadoEmulador;

// 📋 PROTOTIPOS DE FUNCIONES INTERNAS
static bool inicializarSistemaCompleto();
static void apagarSistemaCompleto();
static bool cargarFirmwareDelSistema();
static bool cargarJuego(const std::string& rutaJuego);
static void buclePrincipal();
static void mostrarPantallaInicio();
static void procesarEntrada();
static void actualizarEstado();
static void mostrarAyuda();

// ==============================================================
// 🚀 FUNCIÓN PRINCIPAL — DONDE EMPIEZA TODO
// ==============================================================
int main(int argc, char* argv[]) {
    // ==============================
    // 1. SALUDO Y PRESENTACIÓN
    // ==============================
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                %s — Emulador de PS5                  ║\n", NOMBRE_APP.c_str());
    printf("║                  Versión: %s                          ║\n", VERSION_APP.c_str());
    printf("║           Compilado: %s %s                       ║\n", FECHA_COMPILACION.c_str(), HORA_COMPILACION.c_str());
    printf("║               Plataforma: Android / Vulkan               ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n\n");

    // ==============================
    // 2. PROCESAR ARGUMENTOS
    // ==============================
    std::string rutaJuegoInicial;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            mostrarAyuda();
            return EXIT_SUCCESS;
        } else if (arg == "--firmware" && i + 1 < argc) {
            estadoEmulador.rutaFirmware = argv[++i];
        } else if (rutaJuegoInicial.empty()) {
            rutaJuegoInicial = arg;
        }
    }

    // ==============================
    // 3. INICIALIZAR TODO EL SISTEMA
    // ==============================
    printf("[1/5] Inicializando subsistemas...\n");
    if (!inicializarSistemaCompleto()) {
        printf("\n❌ Error fatal: No se pudo inicializar el sistema\n");
        printf("   %s\n", estadoEmulador.mensajeEstado.c_str());
        return EXIT_FAILURE;
    }
    printf("✅ Subsistemas listos\n\n");

    // ==============================
    // 4. CARGAR FIRMWARE
    // ==============================
    printf("[2/5] Cargando firmware del sistema...\n");
    if (!cargarFirmwareDelSistema()) {
        printf("⚠️  Advertencia: Problemas con el firmware\n");
        printf("   %s\n", estadoEmulador.mensajeEstado.c_str());
    } else {
        estadoEmulador.firmwareCargado = true;
        printf("✅ Firmware cargado correctamente\n\n");
    }

    // ==============================
    // 5. CARGAR JUEGO SI SE PASÓ
    // ==============================
    if (!rutaJuegoInicial.empty()) {
        printf("[3/5] Cargando juego: %s\n", rutaJuegoInicial.c_str());
        if (cargarJuego(rutaJuegoInicial)) {
            estadoEmulador.juegoCargado = true;
            printf("✅ Juego cargado\n\n");
        } else {
            printf("❌ No se pudo cargar el juego\n\n");
        }
    } else {
        printf("[3/5] Ningún juego especificado — mostrando menú principal\n\n");
    }

    // ==============================
    // 6. MOSTRAR PANTALLA DE INICIO
    // ==============================
    mostrarPantallaInicio();

    // ==============================
    // 7. BUCLE PRINCIPAL — ¡EMPEZAR!
    // ==============================
    printf("[4/5] Iniciando emulación...\n");
    estadoEmulador.estaCorriendo = true;
    printf("[5/5] ✅ %s listo — ¡Emulación en marcha!\n\n", NOMBRE_APP.c_str());

    buclePrincipal(); // ← TODO OCURRE AQUÍ ←

    // ==============================
    // 8. AL SALIR — LIMPIAR TODO
    // ==============================
    printf("\n🛑 Deteniendo emulación...\n");
    apagarSistemaCompleto();
    printf("✅ Sistema apagado correctamente\n");
    printf("👋 ¡Gracias por usar %s!\n", NOMBRE_APP.c_str());

    return EXIT_SUCCESS;
}

// ==============================================================
// 🔧 INICIALIZAR TODOS LOS MÓDULOS
// ==============================================================
static bool inicializarSistemaCompleto() {
    estadoEmulador.mensajeEstado = "Iniciando...";

    // 1. Configuración
    printf("   ⚙️ Cargando configuración... ");
    if (!inicializarConfiguracion()) {
        printf("Falló\n");
        estadoEmulador.mensajeEstado = "Error en configuración";
        return false;
    }
    printf("OK\n");

    // 2. Carpetas y rutas
    printf("   📁 Preparando carpetas... ");
    if (!inicializarCarpetas()) {
        printf("Falló\n");
        estadoEmulador.mensajeEstado = "Error al crear carpetas";
        return false;
    }
    printf("OK\n");

    // 3. Memoria
    printf("   💾 Inicializando memoria (16 GB)... ");
    if (!inicializarMemoria()) {
        printf("Falló\n");
        estadoEmulador.mensajeEstado = "Sin memoria suficiente";
        return false;
    }
    printf("OK\n");

    // 4. Gráficos
    printf("   🖼️ Inicializando gráficos (Vulkan)... ");
    if (!inicializarGrafico(ModoRender::VULKAN, RES_ANCHO_1080, RES_ALTO_1080)) {
        printf("Fallback a OpenGL... ");
        if (!inicializarGrafico(ModoRender::OPENGL, RES_ANCHO_1080, RES_ALTO_1080)) {
            printf("Falló\n");
            estadoEmulador.mensajeEstado = "Sin aceleración gráfica";
            return false;
        }
    }
    printf("OK — %s\n", nombreModoRender(grafico.modo).c_str());

    // 5. Audio
    printf("   🔊 Inicializando audio... ");
    if (!inicializarAudio()) {
        printf("Sin audio\n");
    } else {
        printf("OK\n");
    }

    // 6. Controles
    printf("   🎮 Inicializando controles... ");
    if (!inicializarControles()) {
        printf("Falló\n");
    } else {
        printf("OK\n");
    }

    // 7. Procesador
    printf("   🧠 Inicializando procesador... ");
    if (!inicializarProcesador()) {
        printf("Falló\n");
        estadoEmulador.mensajeEstado = "Error al inicializar CPU";
        return false;
    }
    printf("OK\n");

    estadoEmulador.sistemaInicializado = true;
    estadoEmulador.mensajeEstado = "Sistema listo";
    return true;
}

// ==============================================================
// 🛑 APAGAR TODO ORDENADAMENTE
// ==============================================================
static void apagarSistemaCompleto() {
    estadoEmulador.estaCorriendo = false;

    printf("   🎮 Apagando controles...\n");
    apagarControles();

    printf("   🔊 Apagando audio...\n");
    apagarAudio();

    printf("   🖼️ Apagando gráficos...\n");
    apagarGrafico();

    printf("   💾 Liberando memoria...\n");
    liberarMemoria();

    printf("   ⚙️ Guardando configuración...\n");
    guardarConfiguracion();
}

// ==============================================================
// 📄 CARGAR FIRMWARE
// ==============================================================
static bool cargarFirmwareDelSistema() {
    if (estadoEmulador.rutaFirmware.empty()) {
        estadoEmulador.rutaFirmware = obtenerRutaCarpeta("Firmware");
    }

    EstadoFirmware info = verificarFirmware(estadoEmulador.rutaFirmware);
    if (!info.mensajeError.empty()) {
        estadoEmulador.mensajeEstado = info.mensajeError;
        return false;
    }

    return cargarFirmware(estadoEmulador.rutaFirmware) && inicializarComponentes();
}

// ==============================================================
// 🎮 CARGAR JUEGO
// ==============================================================
static bool cargarJuego(const std::string& rutaJuego) {
    // ⚠️ Aquí se carga el ejecutable del juego, verifica formato,
    // mapea en memoria, inicializa el procesador para ejecutarlo
    estadoEmulador.rutaJuego = rutaJuego;
    estadoEmulador.mensajeEstado = "Juego cargado — esperando inicio";
    return true;
}

// ==============================================================
// 🎬 BUCLE PRINCIPAL — EL CORAZÓN LATIENTE
// ==============================================================
static void buclePrincipal() {
    while (estadoEmulador.estaCorriendo) {
        // 1. Procesar teclado, pantalla táctil, mandos
        procesarEntrada();

        // 2. Ejecutar instrucciones del procesador
        if (estadoEmulador.juegoCargado) {
            ejecutarCuantiaInstrucciones(100000); // 100k instrucciones por cuadro
        }

        // 3. Actualizar estado y estadísticas
        actualizarEstado();

        // 4. Dibujar un cuadro
        empezarCuadro();
        limpiarPantalla(0.05f, 0.05f, 0.08f); // Fondo oscuro
        // ⚠️ Aquí se dibuja el juego, UI, menús...
        terminarCuadro();

        // Salir si se presionó salir
        if (debeSalirEmulador()) {
            estadoEmulador.estaCorriendo = false;
        }
    }
}

// ==============================================================
// 🖥️ PANTALLA DE INICIO
// ==============================================================
static void mostrarPantallaInicio() {
    printf("══════════════════════════════════════════════════════════\n");
    printf("  🎮  %s — Menú Principal\n", NOMBRE_APP.c_str());
    printf("══════════════════════════════════════════════════════════\n");
    printf("  Estado: %s\n", estadoEmulador.mensajeEstado.c_str());
    printf("  Firmware: %s | FPS: %d\n",
           estadoEmulador.firmwareCargado ? "✅ Cargado" : "⚠️ Sin cargar",
           estadoEmulador.cuadrosPorSegundo);
    printf("══════════════════════════════════════════════════════════\n");
    printf("  Cargar juego  |  Configuración  |  Salir\n");
    printf("══════════════════════════════════════════════════════════\n\n");
}

// ==============================================================
// 📊 ACTUALIZAR ESTADO
// ==============================================================
static void actualizarEstado() {
    estadoEmulador.cuadrosPorSegundo = obtenerFPS();
    // ⚠️ Actualizar tiempo de ejecución, estadísticas, barra de estado...
}

// ==============================================================
// 🎹 PROCESAR ENTRADA
// ==============================================================
static void procesarEntrada() {
    // ⚠️ Leer pantalla táctil, mandos, teclado
    procesarEventosControles();
}

// ==============================================================
// ❓ AYUDA
// ==============================================================
static void mostrarAyuda() {
    printf("Uso: %s [opciones] [ruta_del_juego]\n\n", NOMBRE_APP.c_str());
    printf("Opciones:\n");
    printf("  -h, --help              Muestra esta ayuda\n");
    printf("  --firmware <ruta>       Ruta a la carpeta del firmware\n");
    printf("\nEjemplo:\n");
    printf("  %s --firmware ./firmware ./games/mi_juego.pkg\n", NOMBRE_APP.c_str());
}
