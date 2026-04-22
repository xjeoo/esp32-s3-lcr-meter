#include <Arduino.h>
#include "SineGenerator.h"
#include "AdcSampler.h"

#define PWM_CHANNEL 0
#define PWM_PIN     4
#define PWM_FREQ    100000
#define PWM_RES     8

// Buffere locale in care copiem datele din AdcSampler pentru procesare
static int vinLocal[ADC_BUF_SIZE];
static int voutLocal[ADC_BUF_SIZE];

static uint32_t sampleCounter = 0;

void setup() {
  Serial.begin(921600);   // 921600 necesar pentru Teleplot la 10kHz sampling
  while (!Serial) delay(10);
  Serial.println("-Serial initialized");

  if (generateSineWave(PWM_PIN, PWM_FREQ, PWM_RES, PWM_CHANNEL)) {
    Serial.println("-PWM generation started on pin " + String(PWM_PIN) +
                   " at " + String(PWM_FREQ) + " Hz, " + String(PWM_RES) + " bits");
  } else {
    Serial.println("-ERROR: Failed to generate sine wave");
  }

  adcSamplerInit();
  Serial.println("-ADC sampler initialized (10kHz, buf=" + String(ADC_BUF_SIZE) + ")");
}
void loop() {
  int readyBuf = adcGetReadyBufIndex();
  if (readyBuf == -1) return;  // niciun buffer gata, revenim imediat

  adcGetBuffer(readyBuf, vinLocal, voutLocal);
  adcReleaseBuffer(readyBuf);

  for (int i = 0; i < ADC_BUF_SIZE; i++) {
    
    Serial.print(">Vin:");
    Serial.print(sampleCounter);
    Serial.print(":");
    Serial.println(vinLocal[i]);
    Serial.print(">Vout:");
    Serial.print(sampleCounter);
    Serial.print(":");
    Serial.println(voutLocal[i]);
    sampleCounter++;
  }
}