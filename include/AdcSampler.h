#pragma once
#include <Arduino.h>

// ─── Config ────────────────────────────────────────────────
#define ADC_BUF_SIZE   200      // 200 samples = 20 perioade la 10kHz sampling
#define ADC_SAMPLE_US  100      // 100us intre samples = 10kHz frecventa de esantionare
#define ADC_VIN_PIN    5        // unused
#define ADC_VOUT_PIN   6        // unused
// ───────────────────────────────────────────────────────────

void adcSamplerInit();

// returns index of ready buffer (0 or 1), -1 if neither are ready
int  adcGetReadyBufIndex();

// Copies ADC buffers in specified arrays
// vinOut and voutOut must have at least ADC_BUF_SIZE elements
void adcGetBuffer(int bufIdx, int* vinOut, int* voutOut);

//Frees buffer after processing. ISR can write in it again
void adcReleaseBuffer(int bufIdx);