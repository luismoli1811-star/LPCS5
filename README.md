# 🎮 LPCS5 — Emulador de PlayStation 5 para Android

> ⚠️ **Estado del proyecto:** En desarrollo activo — Versión 0.1.0
> 📱 **Plataforma:** Android 10+ (ARM64-v8a)
> ⚡ **Lenguaje:** C++ Nativo — Alto rendimiento
> 📄 **Licencia:** BSD 3-Clause

---

## 📖 Descripción

**RPCS5** es un emulador experimental y de código abierto de consola PlayStation 5, diseñado **específicamente para dispositivos móviles Android**. Su objetivo es permitir jugar títulos de PS5 en teléfonos y tabletas, aprovechando aceleración por hardware, optimizaciones de rendimiento y una interfaz adaptada a pantallas táctiles.

---

## ✨ Características Principales

### 🧠 Emulación
- ✅ **Núcleo de CPU x86_64 → ARM64** — traducción dinámica de instrucciones
- ✅ **Emulación de GPU** — renderizado gráfico con Vulkan/OpenGL ES 3.2
- ✅ **Sistema de memoria** — gestión de memoria de consola adaptada a dispositivos móviles
- ✅ **Audio de baja latencia** — AAudio / OpenSL ES, sincronizado con video
- ✅ **Compatibilidad con formatos:** `.pkg`, `.iso`, carpetas descomprimidas

### ⚡ Rendimiento
- ✅ **5 niveles de optimización** — desde Depuración hasta Máximo Rendimiento
- ✅ **Ajuste automático en tiempo real** — baja resolución, salto de cuadros, reduce efectos si va lento
- ✅ **Vectorización NEON** — instrucciones rápidas nativas de ARM
- ✅ **Multihilo adaptativo** — usa todos los núcleos disponibles
- ✅ **VSync adaptativo** — sincronización dinámica para fluidez

### 📱 Interfaz y Controles
- ✅ **Controles táctiles en pantalla** — sticks, gatillos, botones con tamaño y posición personalizable
- ✅ **Soporte de mandos físicos** — Bluetooth, USB, Xbox, DualSense, DualShock
- ✅ **Escala de resolución** — 0.5x → 1.5x, ajustable por juego
- ✅ **Idiomas múltiples** — Español, Inglés, Francés, Alemán, Japonés, Chino
- ✅ **Modo oscuro / claro** — adaptable al sistema

### 💾 Sistema de Guardados
- ✅ **Guardados manuales (ranuras 0–9)** — compatibles con formato nativo
- ✅ **Estados rápidos (Savestates)** — guarda y carga TODO el estado al instante ⚡
- ✅ **Auto-guardado configurable** — cada 5 minutos, al salir del juego
- ✅ **Capturas de pantalla previsualizadas** — ves tu partida de un vistazo 📸
- ✅ **Compresión de estados** — ahorra hasta 50% de espacio
- ✅ **Exportar / Importar** — respalda y transfiere tus partidas ☁️

---

## 📱 Requisitos del Sistema

| Requisito | Mínimo | Recomendado |
|---|---|---|
| **Sistema Operativo** | Android 10 (API 29) | Android 13+ (API 33+) |
| **Procesador** | ARM64-v8a, 4 núcleos | Snapdragon 8 Gen 1+ / Dimensity 9000+ |
| **RAM** | 6 GB | 12 GB o más |
| **Almacenamiento** | 2 GB libres | Tarjeta SD rápida o UFS 3.0+ |
| **Gráficos** | Vulkan 1.1 / OpenGL ES 3.2 | Vulkan 1.3+ con controladores actualizados |

---

## 📂 Estructura del Proyecto
RPCS5/
├── main.cpp
├── Sistema.h / Sistema.cpp
├── Procesador.h / Procesador.cpp
├── Memoria.h / Memoria.cpp
├── Grafica.h / Grafica.cpp
├── InterfazUsuario.h / InterfazUsuario.cpp
├── OptimizacionesRendimiento.h / OptimizacionesRendimiento.cpp
├── SistemaGuardados.h / SistemaGuardados.cpp
├── README.md          ← ✅ El tuyo, tal como lo vi en la foto
├── LICENSE
└── CMakeLists.txt     ← ✅ Configurado solo con tus archivos

