#include <Arduino.h>
#include "SineGenerator.h"

#define PWM_CHANNEL 0
#define PWM_PIN 4
#define PWM_FREQ 150000 // 150 kHz -- schimbare la 6.7 microsecunde ca sa fie mai mic decat durata alarmei (10 microsecunde)
#define PWM_RES 8     // 0-255 -- ca sa fie nevoie de mai putin de 40Mhz/150khz pasi



void setup()
{
  Serial.begin(115200);
  while(!Serial) delay(10);
  Serial.println("-Serial initialized");

  generateSineWave(PWM_PIN, PWM_FREQ, PWM_RES, PWM_CHANNEL);
  Serial.println("-PWM generation started on pin " + String(PWM_PIN) + " with frequency " + String(PWM_FREQ) + " Hz and resolution " + String(PWM_RES) + " bits");
}

void loop()
{
  Serial.println("merge");
  delay(5000);
}
