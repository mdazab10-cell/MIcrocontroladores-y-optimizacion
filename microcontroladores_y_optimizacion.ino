/*
  microcontroladores_y_optimizacion.ino
  Versión optimizada del DFT (64 puntos) comparando punto flotante y
  una aproximación en punto fijo con tablas precalculadas.

  - Usa SoftwareSerial para enviar tiempos por HC-05.
  - Precalcula cos/sin (factor de escala SCALE).
  - Código comentado en español.

  Hardware:
  - HC-05 conectado a 5V, GND, TXD/RXD (usar divisor de tensión para RX a 3.3V)
  - SoftwareSerial en pines 10 (RX), 11 (TX).
*/

#include <SoftwareSerial.h>
#include <math.h>
#include <Arduino.h>

SoftwareSerial BTSerial(10, 11); // RX, TX

const int N = 64;               // número de muestras (DFT tamaño)
float signal[N];                // señal de entrada (float para generar muestras)
float spectrum_float[N];        // espectro en punto flotante
int spectrum_fixed[N];          // espectro aproximado en punto fijo

// Tabla de coseno y seno precalculadas (escala SCALE)
const int SCALE = 1000;
int cosTable[N];
int sinTable[N];

void precalcTables() {
  for (int k = 0; k < N; k++) {
    float angle = -2.0 * PI * k / N;
    cosTable[k] = (int)round(SCALE * cos(angle));
    sinTable[k] = (int)round(SCALE * sin(angle));
  }
}

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  // Generar señal compuesta (ejemplo): seno fundamental + 0.5 * segundo armónico
  for (int n = 0; n < N; n++) {
    signal[n] = sin(2.0 * PI * n / N) + 0.5 * sin(4.0 * PI * n / N);
  }

  precalcTables();
}

void loop() {
  unsigned long t1, t2;

  // --- Procesamiento en punto flotante (DFT directo) ---
  t1 = micros();
  for (int k = 0; k < N; k++) {
    float real = 0.0f, imag = 0.0f;
    for (int n = 0; n < N; n++) {
      float angle = -2.0f * PI * k * n / N;
      real += signal[n] * cos(angle);
      imag += signal[n] * sin(angle);
    }
    spectrum_float[k] = sqrt(real * real + imag * imag);
  }
  t2 = micros();
  unsigned long tiempo_float = t2 - t1;

  // --- Procesamiento aproximado en punto fijo (uso de tablas precalculadas) ---
  t1 = micros();
  for (int k = 0; k < N; k++) {
    long accReal = 0;
    long accImag = 0;
    for (int n = 0; n < N; n++) {
      int angleIndex = (k * n) % N;
      int cosVal = cosTable[angleIndex]; // escalado por SCALE
      int sinVal = sinTable[angleIndex];
      int sScaled = (int)round(signal[n] * SCALE);

      // Multiplicación (sScaled * cosVal) -> factor SCALE^2
      accReal += (long)sScaled * (long)cosVal;
      accImag += (long)sScaled * (long)sinVal;
    }
    // Convertimos a float y normalizamos por SCALE^2 para calcular magnitud
    float r = (float)accReal / (SCALE * SCALE);
    float i = (float)accImag / (SCALE * SCALE);
    float mag = sqrt(r * r + i * i);
    spectrum_fixed[k] = (int)round(mag * 1000.0f); // magnitud escalada *1000
  }
  t2 = micros();
  unsigned long tiempo_fixed = t2 - t1;

  // --- Envío de tiempos al Serial Plotter (dos series separadas por tab) ---
  Serial.print(tiempo_float);
  Serial.print("\t");
  Serial.println(tiempo_fixed);

  // --- Envío por Bluetooth HC-05 (texto) ---
  BTSerial.print("Tiempo flotante: ");
  BTSerial.print(tiempo_float);
  BTSerial.println(" us");

  BTSerial.print("Tiempo fijo (esc. *1000): ");
  BTSerial.print(tiempo_fixed);
  BTSerial.println(" us");

  delay(1000); // espera 1s entre iteraciones
}