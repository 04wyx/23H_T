#ifndef _INTIMDAC_H_
#define _INTIMDAC_H_

#include "main.h"
#include "tim.h"
#include "math.h"
#include "usart.h"
#include "dac.h"
#include "Config.h"

extern uint16_t SinArray[MAX_WAVE_LENGTH];  // 给 DAC1 (TIM2) 用
extern uint16_t SinArray2[MAX_WAVE_LENGTH]; // 给 DAC2 (TIM4) 用 

void Set_DAC_Waveform_AutoHighRes(uint32_t target_freq, float phase_deg, WaveformType wave_type);
void Set_DAC2_Waveform_AutoHighRes(uint32_t target_freq, float phase_deg, WaveformType wave_type);

#endif
