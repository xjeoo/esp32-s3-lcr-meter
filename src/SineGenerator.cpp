#include <Arduino.h>
#include "SineGenerator.h"

#define SAMPLES 100


int sinTable[SAMPLES];
volatile int sampleIndex = 0; 

hw_timer_t *timer = NULL;

// valoare provizorie inaite de a primi canalul ca parametru
uint8_t activePwmChannel = 0;

// punem in IRAM functia de intrerupere ca sa fie rapida
void IRAM_ATTR onTimer(){  
  ledcWrite(activePwmChannel, sinTable[sampleIndex]);
  sampleIndex++;
  
  if (sampleIndex >= SAMPLES){
    sampleIndex = 0;
  }
}

bool generateSineWave(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t channel) {
  

  // input validation starting

  if (resolution < 1 || resolution > 16) {
    return false;
  }
  
  // ESP32-S3 low-speed LEDC uses 40 MHz XTAL clock
  // Max frequency = clock_freq / (2^resolution)
  const uint32_t LEDC_CLOCK_FREQ = 40000000;
  uint32_t maxFreq = LEDC_CLOCK_FREQ / (1 << resolution);
  
  if (freq > maxFreq) {
    return false;
  }
  
  if (freq == 0) {
    return false;
  }
  
  // input validation ending

  activePwmChannel = channel;

  for (int i = 0; i < SAMPLES; i++) {
    float angle = (2.0 * PI * i) / SAMPLES;  // [0, 2*PI] normat la sample ca sa fie intre 0-255
    sinTable[i] = (int)(127.5 + 127.5 * sin(angle));
  }

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(pin, channel);

  
  //-----------------------------------
  // -------TIMER INITIALIZATION-------
  //-----------------------------------
  // semnal 1khz (1000 perioade pe secunda)
  // => esantionam in 100 de puncte
  // => un esantion la fiecare 1/(1000*100) secunde (10 microsecunde/esantion)
  // aici setam alarma timerului
  //-----------------------------------


  // ceasul default este de 80 Mhz -- divider 80 ca sa ramana 1Mhz (1 microsecunda)
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10, true);
  timerAlarmEnable(timer);
  
  return true;
}