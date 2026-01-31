# Microcontroladores-y-optimizacion
# Optimización de Algoritmos DSP en Arduino: Punto Flotante vs. Punto Fijo

## 📋 Descripción del Proyecto

Este proyecto implementa y compara dos métodos de cálculo de Transformada Discreta de Fourier (DFT) en Arduino Uno:

- **DFT en punto flotante**: Usa funciones `cos()` y `sin()` en tiempo de ejecución
- **DFT optimizado en punto fijo**: Emplea tablas precalculadas y aritmética entera

**Objetivo**: Demostrar mejoras de rendimiento en microcontroladores con recursos limitados.

## 🖼️ Diagrama de Conexiones

![Diagrama Arduino-HC05](deepseek_mermaid_20260131_35e5ca.png)

**Figura 1**: Esquema de conexión entre Arduino Uno y módulo HC-05, mostrando alimentación, tierra y conexiones seriales.

## 📊 Comparación de Resultados

| Método | Tiempo Promedio | Memoria Flash | RAM | Precisión | Consumo CPU |
|--------|----------------|---------------|-----|-----------|-------------|
| **Punto Flotante** | 1850 μs | 1800 bytes | 300 bytes | Exacta (float) | Alto |
| **Punto Fijo** | 720 μs | 2200 bytes | 400 bytes | Aprox. (<0.1% error) | Bajo |
| **Mejora** | **61% más rápido** | +22% (tablas) | +33% (buffers) | - | -60% uso CPU |

## 🔌 Conexiones Hardware

### Componentes Requeridos
- Arduino Uno Rev3 (ATmega328P @ 16MHz)
- Módulo Bluetooth HC-05
- Resistencias: 1kΩ y 2kΩ (divisor de tensión)
- Protoboard y cables dupont

### Resultados
![Monitor Serial - Configuración y Datos](Captura%20de%20pantalla%202026-01-25%20151346.png)
🎯 FUNCIÓN PRINCIPAL
loop() - Bucle Principal
Código que ejecuta la comparación continua:


  
# CONSECUENCIAS
1. CONSECUENCIA: RENDIMIENTO
Resultado: 61% más rápido
Punto flotante: 1850 μs
Punto fijo: 720 μs
Reducción: 1130 μs por ciclo

# CONSECUENCIA: MEMORIA
Aumento de uso:
Flash: +400 bytes (22% más)

RAM: +100 bytes (33% más)

Trade-off: memoria por velocidad

# CONSECUENCIA: PRECISIÓN
Error introducido:

Punto flotante: precisión completa

Punto fijo: error < 0.1%

Aceptable para audio/sensores

# CONSECUENCIA: ENERGÍA
Reducción de consumo:

Menos ciclos CPU = menos energía

Importante para baterías

Operaciones enteras más eficientes

# CONSECUENCIA: ESTABILIDAD
Mejora en consistencia:

Punto flotante: ±10% variación

Punto fijo: ±2% variación

Comportamiento predecible
# CONSECUENCIA: APLICACIONES
Usos recomendados:

Tiempo real: SÍ (punto fijo)

Alta precisión: NO (usar float)

Batería: SÍ (punto fijo)

Audio: SÍ (error inaudible)

licencia: Mit


