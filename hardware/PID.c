/* pid.c */
#include "pid.h"

// 实例化 PID 结构体变量（真正分配内存的地方）
PhasePID_TypeDef DAC_Phase_PID1;  // 用于DAC通道1 (wave1)
PhasePID_TypeDef DAC_Phase_PID2;  // 用于DAC通道2 (wave2)

/**
  * @brief  初始化 PID 参数
  * @param  None
  * @retval None
  */
void Phase_PID_Init(void) {
    // 初始化PID1 (DAC通道1)
    DAC_Phase_PID1.Kp = 1.0f;   // 比例系数 (增大以加快响应)
    DAC_Phase_PID1.Ki = 0.02f;  // 积分系数 (适度增大以消除稳态误差)
    DAC_Phase_PID1.Kd = 0.1f;   // 微分系数 (增大以抑制超调)

    DAC_Phase_PID1.target_phase = 0.0f;
    DAC_Phase_PID1.actual_phase = 0.0f;
    DAC_Phase_PID1.error = 0.0f;
    DAC_Phase_PID1.error_last = 0.0f;
    DAC_Phase_PID1.integral = 0.0f;
    DAC_Phase_PID1.out = 0.0f;

    // 初始化PID2 (DAC通道2) - 可以使用相同或不同的参数
    DAC_Phase_PID2.Kp = 1.0f;
    DAC_Phase_PID2.Ki = 0.02f;
    DAC_Phase_PID2.Kd = 0.1f;

    DAC_Phase_PID2.target_phase = 0.0f;
    DAC_Phase_PID2.actual_phase = 0.0f;
    DAC_Phase_PID2.error = 0.0f;
    DAC_Phase_PID2.error_last = 0.0f;
    DAC_Phase_PID2.integral = 0.0f;
    DAC_Phase_PID2.out = 0.0f;
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

    // 1. 计算误差并标准化 (-180° to 180°)
    pid->error = Normalize_Phase(pid->target_phase - pid->actual_phase);

    // 2. 积分累加及积分限幅 (防止积分饱和)
    pid->integral += pid->error;
    if(pid->integral > 180.0f)  pid->integral = 180.0f;
    if(pid->integral < -180.0f) pid->integral = -180.0f;

    // 3. 微分项
    float derivative = pid->error - pid->error_last;

    // 4. PID 计算: 输出 = 实际相位 + Kp*error + Ki*integral + Kd*derivative
    pid->out = pid->actual_phase +
               pid->Kp * pid->error +
               pid->Ki * pid->integral +
               pid->Kd * derivative;

    // 5. 限制输出相位在 0-360 度范围内
    while(pid->out >= 360.0f) pid->out -= 360.0f;
    while(pid->out < 0.0f)    pid->out += 360.0f;

    // 6. 保存本次误差，用于下次计算微分
    pid->error_last = pid->error;

    return pid->out;
}
