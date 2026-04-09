#include <Arduino.h>
#include "SineGenerator.h"
#include "AdcSampler.h"

#define PWM_CHANNEL 0
#define PWM_PIN 4
#define PWM_FREQ 100000 // 100 kHz -- 1khz esantionat in 100 de puncte = 100 khz - exact la alarma de 10 us a timerului
#define PWM_RES 8     // 0-255 -- ca sa fie nevoie de mai putin de 40Mhz/100khz pasi

#define ADC_PIN 6
#define ADC_SAMPLE_RATE 20000 // 20 kHz sampling rate per architecture.md
#define ADC_BUFFER_SIZE 200   // 200 samples per architecture.md



void setup()
{
  Serial.begin(115200);
  while(!Serial) delay(10);
  Serial.println("-Serial initialized");

  // Initialize PWM generation
  if (generateSineWave(PWM_PIN, PWM_FREQ, PWM_RES, PWM_CHANNEL)) {
    Serial.println("-PWM generation started on pin " + String(PWM_PIN) + " with frequency " + String(PWM_FREQ) + " Hz and resolution " + String(PWM_RES) + " bits");
  } else {
    Serial.println("-ERROR: Failed to generate sine wave");
  }

  // Initialize ADC sampler
  if (initAdcSampler(ADC_PIN, ADC_SAMPLE_RATE, ADC_BUFFER_SIZE)) {
    Serial.println("-ADC sampler initialized on pin " + String(ADC_PIN) + " with sample rate " + String(ADC_SAMPLE_RATE) + " Hz and buffer size " + String(ADC_BUFFER_SIZE));
    // Start sampling automatically
    startSampling();
    Serial.println("-Sampling started automatically.");
  } else {
    Serial.println("-ERROR: Failed to initialize ADC sampler");
  }

  Serial.println("-System ready. Continuous sampling active.");
}

void loop()
{
  // Handle ADC sampling in loop (checks flag and performs analogRead)
  handleAdcSampling();

  // Check if buffer is full and print data
  if (isBufferFull()) {
    Serial.println("-Buffer full. Printing CSV data:");
    printBufferCsv();
    Serial.println("-Resetting and continuing...");
    resetBuffer();
    startSampling(); // Automatically continue sampling
  }
 // Delay to avoid flooding the serial output, adjust as needed
}
