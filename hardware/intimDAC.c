#include "intimDAC.h"

/**
 * @brief[DAC通道1] 全自动高点数模式 (TIM2触发)
 * @param  target_freq: 目标频率 (单位: Hz)
 * @param  phase_deg:   目标相位 (单位: 度，仅正弦波生效，三角波自动忽略)
 * @param  wave_type:   波形类型 (WAVE_SINE 或 WAVE_TRIANGLE)
 */
void Set_DAC_Waveform_AutoHighRes(uint32_t target_freq, float phase_deg, WaveformType wave_type)
{
    HAL_TIM_Base_Stop(&htim2);
    HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
    
    uint32_t best_N = 10;
    uint32_t best_ARR = 1;
    uint32_t min_error = 0xFFFFFFFF;
    
    uint32_t max_N = 1000000 / target_freq;
    if (max_N < 10) max_N = 10; 
    if (max_N > MAX_WAVE_LENGTH) max_N = MAX_WAVE_LENGTH;
    
    for (int32_t n = max_N; n >= 10; n--) 
    {
        uint32_t ideal_arr = 84000000 / (n * target_freq);
        if (ideal_arr == 0) continue;
        
        uint32_t freq1 = 84000000 / (n * ideal_arr);
        uint32_t err1 = (freq1 > target_freq) ? (freq1 - target_freq) : (target_freq - freq1);
        if (err1 < min_error) { 
            min_error = err1; best_N = n; best_ARR = ideal_arr;
        }
        
        uint32_t freq2 = 84000000 / (n * (ideal_arr + 1));
        uint32_t err2 = (freq2 > target_freq) ? (freq2 - target_freq) : (target_freq - freq2);
        if (err2 < min_error) {
            min_error = err2; best_N = n; best_ARR = ideal_arr + 1;
        }
    }
    
    // 生成波形数组
    float phase_rad = phase_deg * 3.1415926535f / 180.0f;
    for(uint32_t i = 0; i < best_N; i++) 
    {
        float val = 0.0f;
        
        if (wave_type == WAVE_SINE) 
        {
            // 正弦波：包含相位控制
            val = sin(2.0f * 3.1415926535f * i / best_N + phase_rad);
        }
        else if (wave_type == WAVE_TRIANGLE)
        {
            // 三角波：不需要相位，直接根据当前点在周期内的比例生成
            float x = (float)i / best_N; 
            
            if (x < 0.25f)       val = 4.0f * x;
            else if (x < 0.75f)  val = 2.0f - 4.0f * x;
            else                 val = 4.0f * x - 4.0f;
        }
        
        SinArray[i] = (uint16_t)(520.0f + 400.0f * val); 
    }
    
    __HAL_TIM_SET_PRESCALER(&htim2, 0);
    __HAL_TIM_SET_AUTORELOAD(&htim2, best_ARR - 1);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)SinArray, best_N, DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start(&htim2);
}


/**
 * @brief[DAC通道2] 全自动高点数模式 (TIM4触发)
 * @param  target_freq: 目标频率 (单位: Hz)
 * @param  phase_deg:   目标相位 (单位: 度，仅正弦波生效，三角波自动忽略)
 * @param  wave_type:   波形类型 (WAVE_SINE 或 WAVE_TRIANGLE)
 */
void Set_DAC2_Waveform_AutoHighRes(uint32_t target_freq, float phase_deg, WaveformType wave_type)
{
    HAL_TIM_Base_Stop(&htim4);
    HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_2);
    
    uint32_t best_N = 10;
    uint32_t best_ARR = 1;
    uint32_t min_error = 0xFFFFFFFF;
    
    uint32_t max_N = 1000000 / target_freq;
    if (max_N < 10) max_N = 10; 
    if (max_N > MAX_WAVE_LENGTH) max_N = MAX_WAVE_LENGTH;
    
    for (int32_t n = max_N; n >= 10; n--) 
    {
        uint32_t ideal_arr = 84000000 / (n * target_freq);
        if (ideal_arr == 0) continue;
        
        uint32_t freq1 = 84000000 / (n * ideal_arr);
        uint32_t err1 = (freq1 > target_freq) ? (freq1 - target_freq) : (target_freq - freq1);
        if (err1 < min_error) { 
            min_error = err1; best_N = n; best_ARR = ideal_arr;
        }
        
        uint32_t freq2 = 84000000 / (n * (ideal_arr + 1));
        uint32_t err2 = (freq2 > target_freq) ? (freq2 - target_freq) : (target_freq - freq2);
        if (err2 < min_error) {
            min_error = err2; best_N = n; best_ARR = ideal_arr + 1;
        }
    }
    
    // 生成波形数组
    float phase_rad = phase_deg * 3.1415926535f / 180.0f;
    for(uint32_t i = 0; i < best_N; i++) 
    {
        float val = 0.0f;
        
        if (wave_type == WAVE_SINE) 
        {
            // 正弦波：包含相位控制
            val = sin(2.0f * 3.1415926535f * i / best_N + phase_rad);
        }
        else if (wave_type == WAVE_TRIANGLE)
        {
            // 三角波：不需要相位，直接根据当前点在周期内的比例生成
            float x = (float)i / best_N; 
            
            if (x < 0.25f)       val = 4.0f * x;
            else if (x < 0.75f)  val = 2.0f - 4.0f * x;
            else                 val = 4.0f * x - 4.0f;
        }
        
        SinArray2[i] = (uint16_t)(520.0f + 400.0f * val); 
    }
    
    __HAL_TIM_SET_PRESCALER(&htim4, 0);
    __HAL_TIM_SET_AUTORELOAD(&htim4, best_ARR - 1);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)SinArray2, best_N, DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start(&htim4);
}