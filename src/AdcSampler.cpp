#include "AdcSampler.h"
#include "driver/adc.h"
// ─── Buffere duble ───────────────────────────────────────────────
// ISR scrie in activeBuf. Cand e plin, marcheaza ready si trece la celalalt.
// main.cpp citeste din bufferul marcat ready fara conflict cu ISR-ul.
// ────────────────────────────────────────────────────────────────
static volatile int  vinBuf[2][ADC_BUF_SIZE];
static volatile int  voutBuf[2][ADC_BUF_SIZE];
static volatile bool bufReady[2]  = {false, false};
static volatile int  activeBuf    = 0;    // bufferul in care scrie ISR-ul acum
static volatile int  writeIdx     = 0;    // pozitia curenta in activeBuf

static hw_timer_t*   adcTimer     = NULL;

//
void IRAM_ATTR onAdcTimer() {
  if (bufReady[activeBuf]) return;

  vinBuf[activeBuf][writeIdx]  = adc1_get_raw(ADC1_CHANNEL_4); // GPIO5
  voutBuf[activeBuf][writeIdx] = adc1_get_raw(ADC1_CHANNEL_5); // GPIO6

  writeIdx++;
  if (writeIdx >= ADC_BUF_SIZE) {
    bufReady[activeBuf] = true;
    activeBuf = 1 - activeBuf;
    writeIdx  = 0;
  }
}
//

void adcSamplerInit() {
  adc1_config_width(ADC_WIDTH_BIT_12); //seteaza rezolutia de 12 biti
  adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_12); // GPIO5
  adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_12); // GPIO6

  adcTimer = timerBegin(1, 80, true);
  timerAttachInterrupt(adcTimer, &onAdcTimer, true);
  timerAlarmWrite(adcTimer, ADC_SAMPLE_US, true);
  timerAlarmEnable(adcTimer);
}
int adcGetReadyBufIndex() {
  if (bufReady[0]) return 0;
  if (bufReady[1]) return 1;
  return -1;
}

void adcGetBuffer(int bufIdx, int* vinOut, int* voutOut) {
  for (int i = 0; i < ADC_BUF_SIZE; i++) {
    vinOut[i]  = vinBuf[bufIdx][i];
    voutOut[i] = voutBuf[bufIdx][i];
  }
}

void adcReleaseBuffer(int bufIdx) {
  bufReady[bufIdx] = false;
}



// varianta cu analogRead() mai jos -- era prea inceata

// // ─── ISR ─────────────────────────────────────────────────────────
// void IRAM_ATTR onAdcTimer() {
//   // Daca bufferul activ nu a fost inca eliberat de main, sarim sample-ul
//   // (mai bine pierdem un sample decat sa scriem peste date necitite)
//   if (bufReady[activeBuf]) return;

//   vinBuf[activeBuf][writeIdx]  = analogRead(ADC_VIN_PIN);
//   voutBuf[activeBuf][writeIdx] = analogRead(ADC_VOUT_PIN);
//   // NOTA: Δt intre cele doua analogRead este ~15µs → eroare de faza ~5.4°
//   // Aceasta valoare fixa va fi compensata software in faza de calcul

//   writeIdx++;

//   if (writeIdx >= ADC_BUF_SIZE) {
//     bufReady[activeBuf] = true;
//     activeBuf = 1 - activeBuf;   // flip: 0→1 sau 1→0
//     writeIdx  = 0;
//   }
// }