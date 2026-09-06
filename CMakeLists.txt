# ==============================================================
# 🏗️  CMAKE CONFIGURACIÓN — LPCS5 Emulador de PS5 para Android
# ==============================================================
cmake_minimum_required(VERSION 3.22)

# 📱 PLATAFORMA — Solo Android
if(NOT ANDROID)
    message(FATAL_ERROR "⚠️ Este proyecto está diseñado para Android. Usa la cadena de herramientas NDK de Android.")
endif()

# 🚀 NOMBRE Y LENGUAJE DEL PROYECTO
project(RPCS5
    VERSION 1.0.0
    DESCRIPTION "Emulador de PlayStation 5 para Android"
    LANGUAGES C CXX
)

# 📦 ESTÁNDARES DE LENGUAJE
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================
# 📱 CONFIGURACIÓN DE ANDROID / ARM64
# ==============================================================
# Arquitectura objetivo — ARM64 (todos los celulares modernos)
if(ANDROID_ABI STREQUAL "arm64-v8a")
    message(STATUS "✅ Compilando para ARM64 (arm64-v8a) — optimizado para tu celular")
    set(ARCH_ARM64 ON)
    set(USE_NEON ON) # 🚀 Instrucciones rápidas SIMD
else()
    message(FATAL_ERROR "⚠️ Solo se soporta arm64-v8a. Cambia la arquitectura de compilación.")
endif()

# Versión mínima de Android — Android 10 (API 29)
set(ANDROID_PLATFORM 29)

# ==============================================================
# ⚙️ OPTIMIZACIONES DE COMPILADOR
# ==============================================================
# Modo de compilación — Release = máximo rendimiento
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Tipo de compilación" FORCE)
endif()

message(STATUS "🔧 Modo de compilación: ${CMAKE_BUILD_TYPE}")

# 🚀 BANDERAS DE OPTIMIZACIÓN — ARM64 + NEON
add_compile_options(
    -fPIC
    -fvisibility=hidden
    -ffunction-sections
    -fdata-sections
    -fomit-frame-pointer

    # 🧠 Optimizaciones de velocidad
    -O3
    -DNDEBUG

    # 📱 ARM NEON — aceleración nativa
    -march=armv8-a
    -mtune=cortex-a76
    -mfpu=neon-fp-armv8
    -mfloat-abi=softfp

    # ⚠️ Advertencias
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
)

# 🔗 Opciones del enlazador
add_link_options(
    -Wl,--gc-sections
    -Wl,--as-needed
    -Wl,-z,relro
    -Wl,-z,now
)

# ==============================================================
# 📂 ARCHIVOS FUENTE — TODOS LOS MÓDULOS DEL EMULADOR
# ==============================================================
set(SOURCES_CORE
    # 🚀 Arranque principal
    rpcs5_main.cpp

    # 🧠 Núcleo del emulador
    ProcesadorRPCS5.cpp
    MemoriaRPCS5.cpp
    GraficasRPCS5.cpp
    AudioRPCS5.cpp
    ControlesRPCS5.cpp
    InterfazUsuarioRPCS5.cpp

    # ⚡ Sistemas adicionales
    OptimizacionesRendimientoRPCS5.cpp
    SistemaGuardadosRPCS5.cpp
)

# 📄 ARCHIVOS DE CABECERA (.h)
set(HEADERS_CORE
    ProcesadorRPCS5.h
    MemoriaRPCS5.h
    GraficasRPCS5.h
    AudioRPCS5.h
    ControlesRPCS5.h
    InterfazUsuarioRPCS5.h
    OptimizacionesRendimientoRPCS5.h
    SistemaGuardadosRPCS5.h
)

# ==============================================================
# 📚 LIBRERÍAS NECESARIAS DE ANDROID
# ==============================================================
find_library(ANDROID_LIB android)
find_library(LOG_LIB log)
find_library(EGL_LIB EGL)
find_library(GLESv3_LIB GLESv3)
find_library(OPENSL_LIB OpenSLES)

# 📦 Librerías externas opcionales (comprimir, etc.)
# zlib viene incluida en el NDK de Android
find_package(ZLIB REQUIRED)

# ==============================================================
# 🏗️  OBJETIVO — BIBLIOTECA COMPARTIDA (.so)
# ==============================================================
add_library(rpcs5 SHARED
    ${SOURCES_CORE}
    ${HEADERS_CORE}
)

# 📂 RUTAS DE INCLUSIÓN
target_include_directories(rpcs5 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern
)

# 🔗 ENLAZAR LIBRERÍAS
target_link_libraries(rpcs5
    PRIVATE
        ${ANDROID_LIB}    # 📱 Android nativo
        ${LOG_LIB}        # 📝 Logs del sistema
        ${EGL_LIB}        # 🎨 Gráficos
        ${GLESv3_LIB}     # 🎨 OpenGL ES 3.0+
        ${OPENSL_LIB}     # 🎧 Audio de baja latencia
        ZLIB::ZLIB        # 📦 Compresión de archivos
)

# ==============================================================
# 📱 CONFIGURACIÓN ESPECIAL DE ANDROID
# ==============================================================
# Definiciones de preprocesador para plataforma
target_compile_definitions(rpcs5 PRIVATE
    ANDROID_PLATFORM
    RPCS5_ARM64
    RPCS5_NEON_ENABLED
    $<$<CONFIG:Release>:NDEBUG>
)

# ==============================================================
# ✅ MENSAJE FINAL
# ==============================================================
message("")
message("================================================")
message("  🎮 RPCS5 — Emulador de PS5 para Android")
message("  📦 Versión: ${PROJECT_VERSION}")
message("  📱 Plataforma: Android ARM64 (API ${ANDROID_PLATFORM})")
message("  🔧 Compilador: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message("  ⚙️ Modo: ${CMAKE_BUILD_TYPE}")
message("================================================")
message("")
message("✅ ¡Listo para compilar! Ejecuta:")
message("   cd build && cmake --build . --parallel")
message("")
