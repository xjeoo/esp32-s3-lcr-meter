#ifndef ADC_SAMPLER_H
#define ADC_SAMPLER_H

#include <Arduino.h>

// Initialize ADC sampler with specified parameters
bool initAdcSampler(uint8_t pin, uint32_t sampleRate, uint16_t bufferSize);

// Start sampling (enables timer interrupt)
void startSampling();

// Stop sampling (disables timer interrupt)
void stopSampling();

// Check if buffer is full
bool isBufferFull();

// Print buffer contents in CSV format
void printBufferCsv();

// Reset buffer index and clear full flag
void resetBuffer();

// Get pointer to buffer (for advanced processing)
volatile uint16_t* getBuffer();

// Get current buffer index
uint16_t getBufferIndex();

// Handle ADC sampling in loop() - must be called repeatedly
void handleAdcSampling();

#endif
