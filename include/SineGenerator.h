#ifndef SINE_GENERATOR_H
#define SINE_GENERATOR_H

#include <Arduino.h>

bool generateSineWave(uint8_t pin, uint32_t frequency, uint8_t resolution, uint8_t channel = 0);

#endif