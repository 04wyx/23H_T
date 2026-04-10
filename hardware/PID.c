/* pid.c */
#include "pid.h"

// 实例化 PID 结构体变量（真正分配内存的地方）
PhasePID_TypeDef DAC_Phase_PID;

/**
  * @brief  初始化 PID 参数
  * @param  None
  * @retval None
  */
void Phase_PID_Init(void) {
    // 这里的参数需要根据你的实际系统响应速度进行整定
    DAC_Phase_PID.Kp = 0.5f;   // 比例系数 
    DAC_Phase_PID.Ki = 0.01f;  // 积分系数
    DAC_Phase_PID.Kd = 0.05f;  // 微分系数
    
    DAC_Phase_PID.target_phase = 0.0f;
    DAC_Phase_PID.actual_phase = 0.0f;
    DAC_Phase_PID.error = 0.0f;
    DAC_Phase_PID.error_last = 0.0f;
    DAC_Phase_PID.integral = 0.0f;
    DAC_Phase_PID.out = 0.0f;
}

/**
  * @brief  相位标准化（内部函数，仅在 pid.c 中使用）
  * @note   将相位差限制在 -180 到 180 度之间，防止跨越360度时误差激增
  */
static float Normalize_Phase(float phase_diff) {
    while (phase_diff > 180.0f)  phase_diff -= 360.0f;
    while (phase_diff < -180.0f) phase_diff += 360.0f;
    return phase_diff;
}

/**
  * @brief  PID 计算函数
  * @param  pid: 指向PID结构体的指针
  * @param  target: 目标相位 (角度)
  * @param  actual: 实际读取到的DAC相位 (角度)
  * @retval 计算后需要输出的补偿相位
  */
float Phase_PID_Calc(PhasePID_TypeDef *pid, float target, float actual) {
    pid->target_phase = target;
    pid->actual_phase = actual;
    
    // 1. 计算误差并标准化
    pid->error = Normalize_Phase(pid->target_phase - pid->actual_phase);
    
    // 2. 积分累加及积分限幅 (防止积分饱和)
    pid->integral += pid->error;
    if(pid->integral > 360.0f)  pid->integral = 360.0f;
    if(pid->integral < -360.0f) pid->integral = -360.0f;
    
    // 3. 位置式 PID 计算
    pid->out += (pid->Kp * pid->error) + 
                (pid->Ki * pid->integral) + 
                (pid->Kd * (pid->error - pid->error_last));
                
    // 4. 限制最终输出相位在 0-360 度范围内
    while(pid->out >= 360.0f) pid->out -= 360.0f;
    while(pid->out < 0.0f)    pid->out += 360.0f;
    
    // 5. 保存本次误差，用于下次计算微分
    pid->error_last = pid->error;
    
    return pid->out;
}