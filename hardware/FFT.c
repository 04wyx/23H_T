#include "FFT.h"


/*判断两个信号频率是否一样*/
int find_peak(void)
{
	int count=0;
	for(int i = 20 - range;i <= 100 + range;i++)
	{
		if( (FFT_out[i] > FFT_out[i-1] ) && (FFT_out[i] > FFT_out[i+1] ) && FFT_out[i] > 100000 )
		{
			count++;
//			printf("count = %d\r\n",i);
		}
	}
  printf("wave_flag = %d\r\n",count);

	if(count == 1){ return 1; }  //特殊情况两者频率相同
	else{ return 2; }					//普遍情况
}
/*fft——test*/
void FFT_test(void)
{
	printf("ADC \r\n");
	for(int i = 0;i < NPT;i++)
	{
		ADC_Float[i] = (float)ADC_Value[i];
//		printf("%d ,",ADC_Value[i]);
	}			
	printf("\r\nADC Over\r\n");
	for(int j = 0;j < NPT; j++)
	{
		FFT_in[j*2] = ADC_Float[j];
		FFT_in[j*2+1] = 0;				
	}			
	arm_cfft_f32(&arm_cfft_sR_f32_len1024,FFT_in,0,1);		 
	arm_cmplx_mag_f32(FFT_in,FFT_out,NPT/2);	
}
//查找FFT结果误差范围内最大值
float Find_Wave_Amp(int index, int step)
{	
	int WAVE_INDEX = index * step;
	int step_change = step;
	int wave_change = 0;
	float FFT_Index = FFT_out[ WAVE_INDEX];
	float FFT_Max = FFT_Index;
	if (WAVE_INDEX <= 100)
	{
		if (step == 3)
		{
			step_change = 4;
		}
  		for(int i = index * step - range * (step_change -2); i < index * step + range * (step_change -2); i++){
			printf("i = %d, FFT_out[i] = %.1f\r\n", i, FFT_out[i]);
  		  if(FFT_out[i] >= FFT_Max)
    		{
    	 	 FFT_Max = FFT_out[i];
			 wave_change = i;
   			}
 		}
		printf("IN100:wave_index = %d, i = %d, FFT_out[i] = %.1f , FFT_Max = %.1f\r\n", WAVE_INDEX, wave_change, FFT_out[WAVE_INDEX], FFT_Max);	
	  return FFT_Max;
	}
	else
	{
  		for(int i = (index - range) * step; i < (index + range) * step; i++){
  		  if(FFT_out[i] >= FFT_Max)
    		{
    	 	 FFT_Max = FFT_out[i];
			 wave_change = i; 
   			}
 		}	
		// printf("OUT100:wave_index = %d, i = %d, FFT_out[i] = %.1f , FFT_Max = %.1f\r\n", WAVE_INDEX, wave_change, FFT_out[WAVE_INDEX], FFT_Max);
	  return FFT_Max;
	}

}
//修正wave_index的偏差
int wave_set(int index)
{
//  if(index < 20)
//  { 
//    index = 20; 
//  }
//  else if(index > 100)
//  { 
//    index = 100; 
//  }
  int corrected_index = index + range;
  int num = corrected_index / 5;
  int index_new = num * 5;
//	printf("%d,%d,%d\r\n",index,num,index_new);
  if(index_new >= (index - range) && index_new <= (index + range))
  {
    return index_new;
  }else{
		return index;
	}
}
