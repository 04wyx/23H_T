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
/*去除直流偏移+汉宁窗*/
void remove_dc_and_window(void)
{
    float mean = 0.0f;
    /*================ 1. ADC转float + 求平均 =================*/
    for(int i = 0; i < NPT; i++)
    {
        ADC_Float[i] = (float)ADC_Value[i];
        mean += ADC_Float[i];
    }
    mean /= NPT;
    printf("mean = %.3f\r\n", mean);
    /*================ 2. 去直流 =================*/
    for(int i = 0; i < NPT; i++)
    {
        ADC_Float[i] -= mean;
    }
    /*================ 3. 加窗（汉宁窗） =================*/
    for(int j = 0; j < NPT; j++)
    {
        float w = 0.5f - 0.5f * cosf(2 * PI * j / (NPT - 1));
        FFT_in[2*j]     = ADC_Float[j] * w;  // 实部
        FFT_in[2*j + 1] = 0.0f;              // 虚部
    }
}
/*fft——test*/
void FFT_test(void)
{
	printf("\r\n===== FFT START =====\r\n");	//FFT开始
	printf("\r\n===== ADC START =====\r\n");

	remove_dc_and_window();		

	printf("\r\n===== ADC OVER =====\r\n");
  	/*================ 1. FFT =================*/
	arm_cfft_f32(&arm_cfft_sR_f32_len1024, FFT_in, 0, 1);	
	/*================ 2. 幅值 =================*/	 
	arm_cmplx_mag_f32(FFT_in, FFT_out, NPT/2);
	
    /*================ 3. 相位 =================*/
	for(int i = 0; i < NPT/2; i++)
    {
        float re = FFT_in[2*i];
        float im = FFT_in[2*i + 1];

        // 防止 atan2(0,0)
        if(fabsf(re) < 1e-6f && fabsf(im) < 1e-6f)
        {
            FFT_Phase[i] = 0.0f;
        }
        else
        {
            FFT_Phase[i] = atan2f(im, re);  // 弧度
        }
		FFT_Phase[i] = FFT_Phase[i] * (180.0f / PI);  // 转换为角度
    }

	printf("===== FFT END =====\r\n");
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
//			printf("i = %d, FFT_out[i] = %.1f\r\n", i, FFT_out[i]);
  		  if(FFT_out[i] >= FFT_Max)
    		{
    	 	 FFT_Max = FFT_out[i];
			 wave_change = i;
   			}
 		}
//		printf("IN100:wave_index = %d, i = %d, FFT_out[i] = %.1f , FFT_Max = %.1f\r\n", WAVE_INDEX, wave_change, FFT_out[WAVE_INDEX], FFT_Max);	
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
void wave_test(int wave_flag)		
{
	/*=============================两者频率相同================================*/
 			if( wave_flag == 1)     
 			{
 				DataSolve_Same();
 				wave2_index = wave1_index;
				
 				if(20 - range <= wave1_index && wave1_index <= 100 + range)  //****************频率大于20Khz且小于100kHz*************
 				{
          //查看三倍频点位置的幅值
          FFT_Out_3_wave1 = Find_Wave_Amp(wave1_index, 3);
          printf("FFT_Out_3_wave1 = %.1f\r\n", FFT_Out_3_wave1);
 					if(FFT_Out_3_wave1 > 30000)  //FFT_out[wave1_index * 5] > 20000  //三角+三角
 					{
 						based_wave1_state = based_wave2_state=2; 
 					}
 					else if(FFT_Out_3_wave1 > 15000) //FFT_out[wave1_index * 5] > 9000 //正弦+三角
 					{
 						based_wave1_state = 1;
 						based_wave2_state = 2;
 					}
 					else
 					{
 						based_wave1_state = based_wave2_state = 1;  //正弦+正弦
 					}
 				}
 				else //***********************频率大于100kHz或者频率小于20kHz*************************
 				{
 					printf("wave_same wrong \r\n");
 				}
 			}
/*=============================普遍情况，两者频率不相同================================*/
			else     
			{
				DataSolve_Different();
				if(20 - range <= wave1_index && wave1_index <= 100 + range && 20 - range <= wave2_index &&  wave2_index <= 100 + range )  //********************频率均大于20kHz且小于100kHz并在误差范围内*******************
				{
          FFT_Out_3_wave1 = Find_Wave_Amp(wave1_index, 3);
          FFT_Out_3_wave2 = Find_Wave_Amp(wave2_index, 3);
          FFT_Out_5_wave1 = Find_Wave_Amp(wave1_index, 5);
          FFT_Out_5_wave2 = Find_Wave_Amp(wave2_index, 5);
          
          printf("FFT_Out_wave1 = %.1f, FFT_Out_wave2 = %.1f\r\n", FFT_Out_wave1,FFT_Out_wave2);
          printf("FFT_Out_3_wave1 = %.1f, FFT_Out_3_wave2 = %.1f\r\n", FFT_Out_3_wave1, FFT_Out_3_wave2);
          printf("FFT_Out_5_wave1 = %.1f, FFT_Out_5_wave2 = %.1f\r\n", FFT_Out_5_wave1, FFT_Out_5_wave2);

					if(wave2_index != 3 * wave1_index)//普遍情况 不是恰好三倍 
					{
            printf("not 3\r\n");
						if( fabs(FFT_Out_wave1 / 9.0f - FFT_Out_3_wave1 ) < 15000 && FFT_Out_wave1 > FFT_Out_3_wave1)
						{
							based_wave1_state = 2;  //wave1三角
						}
						else{based_wave1_state = 1;}  //wave1正弦
						
						if( fabs(FFT_Out_wave2 /9.0f - FFT_Out_3_wave2 ) < 15000 && FFT_Out_wave2 > FFT_Out_3_wave2 &&  FFT_Out_3_wave2 > FFT_Out_5_wave2)
						{
							based_wave2_state = 2;  //wave2三角
						}
						else{based_wave2_state = 1;}  //wave2正弦
					}
					else//恰好三倍 
					{
						if( fabs(FFT_Out_wave2 / 9.0f - FFT_Out_3_wave2 ) < 10000 && FFT_Out_wave2 > FFT_Out_3_wave2 &&  FFT_Out_3_wave2 > FFT_Out_5_wave2)
						{
							based_wave2_state = 2;//wave2三角
						}
						else{based_wave2_state = 1;}//wave2正弦
						
						if( fabs(FFT_Out_wave1 / 25.0f - FFT_Out_5_wave1) < 5000 )
						{
							based_wave1_state = 2;//wave1三角
						}
						else{based_wave1_state = 1;}//wave1正弦
					}
				}
        else //***********************频率大于100kHz或者频率小于20kHz*************************
 				{
 					printf("wave_different wrong \r\n");
 				}
			}
}
/*处理相同频率的信号*/
void DataSolve_Same(void)
{
	printf("/*==========same=============*/\r\n");
	for(int j = 20 - range;j <= 100 + range; j++)
	{
		if(FFT_out[wave1_index] < FFT_out[j]) 
		{
			wave1_index = j;            // 更新最大值索引
		} 
	}
	FFT_Out_wave1 = FFT_out[wave1_index];
	int wave_now_index = wave1_index;
	wave1_index = wave_set(wave_now_index);
	printf("wave_index %d to %d\r\n",wave_now_index,wave1_index);
}
/*处理不同频率信号*/
void DataSolve_Different(void)
{
	printf("/*=========different==========*/\r\n");
	for(int j = 20 - range; j <= 100 + range; j++) 
	{
		if(FFT_out[wave1_index] < FFT_out[j]) 
		{
			wave2_index = wave1_index;  // 原最大值变成次大值
			wave1_index = j;            // 更新最大值索引
		} 
		else if(FFT_out[wave2_index] < FFT_out[j] && j != wave1_index) 
		{
			wave2_index = j;  // 更新次大值索引
		}	 
	}
	if(wave2_index < wave1_index) //保证wave1索引小于wave2索引
	{
		int t;
		t = wave1_index;
		wave1_index = wave2_index;
		wave2_index = t;
	}
  FFT_Out_wave1 = FFT_out[wave1_index];
  FFT_Out_wave2 = FFT_out[wave2_index];
  int wave1_now_index = wave1_index;
	int wave2_now_index = wave2_index;
	wave1_index = wave_set(wave1_now_index);
	wave2_index = wave_set(wave2_now_index);
  printf("wave1_index %d to %d,wave2_index %d to %d\r\n",wave1_now_index,wave1_index,wave2_now_index,wave2_index);
}
