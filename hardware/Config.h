#ifndef CONFIG_H
#define CONFIG_H

#define NPT 1024
#define range 1
#define MAX_WAVE_LENGTH 100 //最大的支持点数
#define PI 3.1415926535f

typedef enum {
    WAVE_SINE = 1,    // 正弦波
    WAVE_TRIANGLE = 2 // 三角波
} WaveformType;

#endif
