#ifndef _FFT_H_
#define _FFT_H_

#include "main.h"
#include "tim.h"
#include "math.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "arm_math.h"
#include "arm_const_structs.h"
#include "Config.h"

extern float FFT_in[ NPT * 2 ];
extern float FFT_out[ NPT / 2 ];
extern float ADC_Float[ NPT ];
extern float FFT_Phase[ NPT / 2];
extern uint16_t ADC_Value[ NPT ];

extern float FFT_Out_wave1;
extern float FFT_Out_wave2;
extern float FFT_Out_3_wave1;
extern float FFT_Out_3_wave2;
extern float FFT_Out_5_wave1;
extern float FFT_Out_5_wave2;

extern int based_wave1_state;
extern int based_wave2_state;  //1-----正弦波；2-----三角波

extern int wave1_index ;
extern int wave2_index ;

int find_peak(void);
void FFT_test(void);
float Find_Wave_Amp(int index, int step);
int wave_set(int index);
void wave_test(int wave_flag);
void DataSolve_Same(void);
void DataSolve_Different(void);
void remove_dc_and_window(void);

#endif
