/* pid.h */
#ifndef __PID_H
#define __PID_H

#include "main.h"  // 包含基础的数据类型和HAL库头文件
#include "Config.h"

/* ==================== PID 结构体定义 ==================== */
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    
    float target_phase;  // 目标相位
    float actual_phase;  // 实际相位
    
    float error;         // 当前误差
    float error_last;    // 上次误差
    float integral;      // 积分项
    
    float out;           // PID输出
} PhasePID_TypeDef;

/* ==================== 外部变量声明 ==================== */
// 使用 extern 声明，让包含此头文件的其他 .c 文件都能使用这个全局变量
extern PhasePID_TypeDef DAC_Phase_PID;

/* ==================== 函数声明 ==================== */
void Phase_PID_Init(void);
float Phase_PID_Calc(PhasePID_TypeDef *pid, float target, float actual);

#endif /* __PID_H */