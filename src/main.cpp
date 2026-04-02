#include <Arduino.h>
#include "SineGenerator.h"

#define PWM_CHANNEL 0
#define PWM_PIN 4
#define PWM_FREQ 100000 // 100 kHz -- 1khz esantionat in 100 de puncte = 100 khz - exact la alarma de 10 us a timerului
#define PWM_RES 8     // 0-255 -- ca sa fie nevoie de mai putin de 40Mhz/100khz pasi



void setup()
{
  Serial.begin(115200);
  while(!Serial) delay(10);
  Serial.println("-Serial initialized");

  if (generateSineWave(PWM_PIN, PWM_FREQ, PWM_RES, PWM_CHANNEL)) {
    Serial.println("-PWM generation started on pin " + String(PWM_PIN) + " with frequency " + String(PWM_FREQ) + " Hz and resolution " + String(PWM_RES) + " bits");
  } else {
    Serial.println("-ERROR: Failed to generate sine wave");
}
}

void loop()
{
  Serial.println("merge");
  delay(5000);
}
