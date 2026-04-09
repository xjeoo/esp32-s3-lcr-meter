#include <Arduino.h>
#include "AdcSampler.h"

// Global variables for ADC sampling
static uint8_t adcPin = 0;
static uint16_t bufferSize = 200;
static volatile uint16_t* adcBuffer = nullptr;
static volatile uint16_t bufferIndex = 0;
static volatile bool bufferFull = false;
static volatile bool takeSampleFlag = false;

// Timer handle
static hw_timer_t* adcTimer = NULL;

// ISR for timer interrupt - must have IRAM_ATTR
void IRAM_ATTR onAdcTimer() {
    takeSampleFlag = true;
}

bool initAdcSampler(uint8_t pin, uint32_t sampleRate, uint16_t size) {
    // Validate parameters
    if (size == 0 || sampleRate == 0) {
        return false;
    }
    
    // Store parameters
    adcPin = pin;
    bufferSize = size;
    
    // Allocate buffer
    if (adcBuffer != nullptr) {
        delete[] adcBuffer;
    }
    adcBuffer = new volatile uint16_t[bufferSize];
    
    // Initialize buffer
    for (uint16_t i = 0; i < bufferSize; i++) {
        adcBuffer[i] = 0;
    }
    
    // Reset state
    bufferIndex = 0;
    bufferFull = false;
    takeSampleFlag = false;
    
    // Configure ADC pin
    pinMode(adcPin, INPUT);
    
    // Set ADC resolution to 12 bits (0-4095) - default for ESP32
    analogReadResolution(12);
    
    // Calculate timer interval in microseconds
    // sampleRate is in Hz, so interval = 1,000,000 / sampleRate
    uint32_t intervalUs = 1000000 / sampleRate;
    
    // Initialize Timer 1 (Timer 0 is used for PWM)
    // Divider 80 gives 1 microsecond resolution (80MHz / 80 = 1MHz)
    adcTimer = timerBegin(1, 80, true);
    
    if (adcTimer == NULL) {
        return false;
    }
    
    // Attach interrupt handler
    timerAttachInterrupt(adcTimer, &onAdcTimer, true);
    
    // Set alarm value (in microseconds)
    timerAlarmWrite(adcTimer, intervalUs, true);
    
    // Don't enable timer yet - wait for startSampling()
    timerAlarmEnable(adcTimer);
    
    return true;
}

void startSampling() {
    if (adcTimer != NULL) {
        // Reset buffer before starting
        resetBuffer();
        // Enable timer interrupt
        timerAlarmEnable(adcTimer);
    }
}

void stopSampling() {
    if (adcTimer != NULL) {
        // Disable timer interrupt
        timerAlarmDisable(adcTimer);
    }
}

bool isBufferFull() {
    return bufferFull;
}

void printBufferCsv() {
    Serial.println("sample_index,adc_value");
    for (uint16_t i = 0; i < bufferSize; i++) {
        Serial.print(">Vout:");
        
        Serial.println(adcBuffer[i]);
    }
}

void resetBuffer() {
    bufferIndex = 0;
    bufferFull = false;
    takeSampleFlag = false;
    
    // Clear buffer
    if (adcBuffer != nullptr) {
        for (uint16_t i = 0; i < bufferSize; i++) {
            adcBuffer[i] = 0;
        }
    }
}

volatile uint16_t* getBuffer() {
    return adcBuffer;
}

uint16_t getBufferIndex() {
    return bufferIndex;
}

// This function should be called in loop() to handle sampling
// It checks the flag set by ISR and performs the actual analogRead
void handleAdcSampling() {
    if (takeSampleFlag && !bufferFull) {
        takeSampleFlag = false;
        
        // Read ADC value (this is safe to do in loop())
        uint16_t adcValue = analogRead(adcPin);
        
        // Store in buffer
        adcBuffer[bufferIndex] = adcValue;
        bufferIndex++;
        
        // Check if buffer is full
        if (bufferIndex >= bufferSize) {
            bufferFull = true;
            stopSampling(); // Stop timer when buffer is full
        }
    }
}