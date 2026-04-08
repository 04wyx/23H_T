/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "math.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "FFT.h"
#include "Config.h"
#include "intimDAC.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

void DataSolve_Same(void);
void DataSolve_Different(void);

uint8_t adcflag = 0,wave_flag = 0;

uint16_t ADC_Value[NPT]	= {2359, 2346, 2341, 2662, 2607, 2736, 3011, 3255, 2949, 2656, 2391, 1897, 1570, 1348, 1705, 1620, 1751, 2277, 2236, 2283, 2057, 1962, 1697, 1580, 1381, 1935, 2026, 2305, 2601, 2997, 3284, 2937, 2693, 2812, 2471, 2258, 2329, 2462, 2471, 2785, 3060, 3276, 2965, 2633, 2558, 1984, 1706, 1467, 1577, 1637, 1916, 2180, 2228, 2147, 1879, 2128, 1779, 1647, 1541, 1770, 1872, 2220, 2419, 3063, 3091, 2842, 2787, 2679, 2511, 2453, 2435, 2757, 2627, 2840, 3050, 3152, 2877, 2569, 2274, 2264, 1633, 1324, 1747, 1856, 1903, 2063, 2185, 2036, 1910, 1659, 1927, 1562, 1392, 1651, 1967, 2033, 2585, 2817, 3543, 3099, 2926, 2845, 2696, 2297, 2281, 2449, 2625, 2744, 2868, 3260, 3074, 2766, 2376, 2044, 1573, 1303, 1426, 1524, 1763, 1873, 2459, 2255, 2096, 1881, 1789, 1549, 1385, 1528, 2121, 2100, 2432, 3017, 3303, 3182, 3066, 2813, 2458, 2351, 2169, 2364, 2398, 2537, 3063, 3158, 3003, 2775, 2438, 2039, 1835, 1550, 1424, 1538, 1676, 2353, 2186, 2288, 2217, 2114, 1755, 1765, 1828, 1514, 1767, 2045, 2599, 2683, 3008, 3043, 2837, 2570, 2536, 2329, 2530, 2408, 2522, 2731, 2855, 3032, 3065, 2873, 2484, 2141, 1697, 1729, 1444, 1619, 1769, 2032, 2027, 2309, 2158, 2183, 1701, 1403, 1716, 1566, 1686, 1778, 2311, 2619, 3061, 3232, 2896, 2855, 2561, 2663, 2317, 2329, 2501, 2688, 2762, 2992, 3025, 3052, 2414, 2139, 1843, 1617, 1505, 1605, 1651, 2180, 2035, 2202, 2130, 1968, 1677, 1627, 1463, 1426, 1606, 1862, 2422, 2785, 3049, 3229, 3229, 2724, 2652, 2265, 2495, 2325, 2398, 2573, 2716, 2871, 3097, 2733, 2888, 2414, 2091, 1742, 1578, 1485, 1689, 1653, 2186, 2122, 2178, 2033, 1825, 1620, 1496, 1293, 1954, 1977, 2301, 2569, 2887, 3140, 2933, 2660, 2783, 2545, 2287, 2345, 2516, 2513, 2679, 2817, 3455, 2642, 2632, 2306, 1956, 1815, 1376, 1574, 1729, 1878, 1838, 2147, 2229, 2138, 1939, 1676, 1740, 1407, 1530, 1867, 2085, 2679, 2886, 3114, 2971, 2882, 2652, 2502, 2253, 2709, 2598, 2659, 2825, 2956, 2952, 2885, 2385, 2508, 1974, 1648, 1333, 1606, 1520, 1740, 1793, 2465, 2182, 2163, 1973, 1893, 1505, 1399, 1549, 2161, 2171, 2460, 2899, 3143, 3104, 2969, 2841, 2527, 2457, 2239, 2721, 2511, 2574, 2753, 3038, 2711, 2559, 2267, 2332, 1685, 1370, 1734, 1804, 1898, 2065, 2215, 2079, 2021, 1777, 2071, 1616, 1479, 1721, 1902, 2080, 2447, 2736, 3080, 2952, 2725, 2897, 2565, 2354, 2250, 2440, 2483, 2763, 2837, 3311, 2963, 2687, 2411, 2176, 1711, 1503, 1535, 1892, 1866, 1890, 2068, 2188, 1916, 1900, 1679, 1958, 1446, 1526, 1773, 2177, 2324, 2653, 2897, 3253, 2810, 2686, 2675, 2498, 2331, 2540, 2698, 2826, 2851, 2869, 3107, 2805, 2435, 2167, 1880, 1329, 1411, 1431, 1985, 1897, 2018, 2248, 2117, 1827, 1849, 1707, 1724, 1509, 1872, 2153, 2388, 2593, 2904, 2930, 3041, 2674, 2548, 2639, 2463, 2473, 2599, 2880, 2967, 2955, 2998, 2721, 2353, 1947, 1773, 1518, 1681, 1663, 1628, 2089, 1925, 2178, 2089, 1972, 1703, 1649, 1480, 1694, 1791, 2146, 2453, 2815, 2911, 3059, 2780, 2955, 2631, 2388, 2319, 2430, 2458, 2679, 2669, 3198, 3090, 2744, 2464, 2261, 1799, 1682, 1529, 1863, 1759, 1810, 2141, 2265, 2071, 1997, 1803, 1960, 1502, 1350, 1635, 2132, 2263, 2595, 2885, 3448, 2927, 2907, 2525, 2320, 2195, 2281, 2339, 2861, 2915, 3099, 3069, 2948, 2667, 2253, 1858, 1624, 1378, 1421, 1618, 1915, 2196, 2227, 2162, 2176, 1859, 1615, 1485, 1356, 1630, 1877, 2264, 2719, 3012, 3155, 2931, 2801, 2597, 2437, 2141, 2547, 2534, 2658, 2812, 2969, 2971, 2837, 2529, 2604, 2078, 1738, 1509, 1643, 1559, 1761, 1863, 2327, 2174, 2006, 1990, 1823, 1603, 1449, 1686, 1841, 2217, 2391, 3063, 3106, 2894, 2824, 2709, 2441, 2441, 2423, 2540, 2610, 2681, 3147, 3061, 2964, 2743, 2406, 2080, 1616, 1384, 1637, 1611, 1703, 2006, 2088, 2129, 1950, 1942, 1914, 1563, 1364, 1817, 1896, 2155, 2488, 2825, 2993, 3049, 2773, 3034, 2668, 2530, 2339, 2564, 2565, 2808, 2853, 3323, 2947, 2654, 2276, 1927, 1508, 1268, 1492, 1882, 1871, 2028, 2331, 2289, 1993, 1865, 1686, 1599, 1395, 1421, 2095, 2258, 2488, 2811, 3160, 2989, 2895, 2615, 2843, 2468, 2234, 2438, 2564, 2593, 2762, 2995, 3260, 2691, 2374, 2178, 1756, 1283, 1367, 1495, 1892, 1869, 2092, 2334, 2226, 1820, 1755, 1487, 1758, 1514, 1778, 2029, 2358, 2537, 2934, 2953, 3220, 2783, 2566, 2383, 2182, 2079, 2316, 2417, 2943, 3028, 3061, 2829, 2411, 2366, 1791, 1510, 1528, 1611, 1605, 1800, 1855, 2413, 2057, 1939, 1832, 1678, 1356, 1306, 1616, 2240, 2256, 2580, 2957, 2994, 2914, 2665, 2572, 2351, 2207, 2215, 2779, 2763, 2890, 3005, 3194, 2642, 2413, 2191, 2126, 1574, 1424, 1677, 1761, 1749, 1979, 2112, 2235, 1925, 1639, 1750, 1497, 1410, 1729, 2038, 2258, 2611, 2820, 3360, 2974, 2794, 2593, 2441, 2161, 2345, 2542, 2709, 2805, 2871, 3432, 2839, 2446, 2159, 1820, 1377, 1321, 1511, 1965, 1800, 1948, 2422, 2260, 2028, 1901, 1786, 1495, 1320, 1627, 2065, 2128, 2468, 3023, 3139, 2968, 2881, 2683, 2315, 2269, 2249, 2559, 2569, 2681, 3163, 3163, 2922, 2573, 2303, 1841, 1582, 1334, 1646, 1692, 1713, 2240, 2120, 2074, 2056, 1879, 1725, 1557, 1472, 1511, 1845, 2052, 2615, 2747, 3121, 2969, 2912, 2683, 2492, 2229, 2611, 2571, 2605, 2753, 2984, 2929, 2809, 2484, 2077, 1789, 1473, 1682, 1582, 1660, 1949, 2049, 2121, 2223, 2082, 1691, 1716, 1574, 1657, 1617, 1790, 2277, 2542, 2843, 3141, 3046, 2669, 2593, 2215, 2484, 2407, 2468, 2703, 2832, 2872, 2997, 2892, 2411, 2201, 1871, 2018, 1456, 1578, 1755, 1875, 1953, 2151, 2260, 2073, 1857, 1545, 1947, 1472, 1458, 2031, 2217, 2502, 2723, 3070, 2860, 2640, 2532, 2539, 2355, 2245, 2457, 2576, 2824, 2865, 3004, 3025, 2676, 2299, 2049, 1892, 1481, 1542, 1605, 2137, 1922, 2078, 2218, 2001, 1660, 1761, 1569, 1600, 1448, 1708, 2114, 2350, 2735, 3028, 3056, 2875, 2631, 2381, 2705, 2287, 2499, 2685, 3105, 3049, 3291, 2960, 2707, 2355, 2044, 1721, 1427, 1722, 1621, 1676, 1879, 2084, 2100, 2025, 1893, 1877, 1542, 1359, 1474, 1692, 2025, 2373, 2783, 3031, 3096, 2972, 2883, 2576, 2369, 2191, 2361, 2457, 2645, 2799, 3239, 2971, 2617, 2604, 2102, 1750, 1480, 1359, 1261, 1732, 1956, 2121, 2265, 2133, 2351, 1922, 1692, 1533, 1374, 1649, 1966, 2357, 2633, 2901, 2869, 3027, 2746, 2555, 2436, 2270, 2151, 2427, 2644, 3076, 2961, 3124, 2990, 2455, 2169, 1854, 1496, 1451, 1705, 1612, 2166, 2104, 2317, 2219, 2085, 1759, 1653, 1447, 1605, 1646, 1818, 2247, 2563, 2820, 3147, 2904, 2644, 2491
    };
float FFT_in[ NPT*2 ] = {0};
float FFT_out[ NPT/2 ] = {0};
float ADC_Float[ NPT ] = {0};
float FFT_Phase[NPT / 2] = {0}; // 存储每个频率点的相位 (单位：弧度)

float FFT_Out_wave1 = 0.0;
float FFT_Out_wave2 = 0.0;
float FFT_Out_3_wave1 = 0.0;
float FFT_Out_3_wave2 = 0.0;
float FFT_Out_5_wave1 = 0.0;
float FFT_Out_5_wave2 = 0.0;

int based_wave1_state = 0;
int based_wave2_state = 0;  //1-----正弦波；2-----三角波

int wave1_index = 1;
int wave2_index = 1;

uint32_t wave1_Freq = 0;
uint32_t wave2_Freq = 0;

float phase1 = 0;
float phase2 = 0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

uint16_t SinArray[MAX_WAVE_LENGTH];  // 给 DAC1 (TIM2) 用
uint16_t SinArray2[MAX_WAVE_LENGTH]; // 给 DAC2 (TIM4) 用 

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	LED_BLUE_Off;
	LED_GREEN_Off;
	printf("START\r\n");
	HAL_TIM_Base_Start(&htim3);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&ADC_Value,NPT);	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		// if(adcflag==1)
		// {	
			LED_BLUE_On;
       //adc标志位、波形频率位初始化
 			adcflag=0;
 			wave_flag=0;

       //波形的频率索引、类型初始化
 			wave1_index=1;
 			wave2_index=1;
 			based_wave1_state=0;
 			based_wave2_state=0;

 			HAL_TIM_Base_Stop(&htim3);
 			HAL_ADC_Stop_DMA(&hadc1);		
			
 			FFT_test(); //FFT变换并计算幅值
			
// 			for(int i=0;i < NPT / 2;i++)
// 			{
// 				printf("%.1f , ",FFT_out[i]);
// 			}					
			
 			wave_flag = find_peak();
			
			wave_test(wave_flag);
			
/*==========================================================*/			
			//1-----正弦波；2-----三角波
			wave1_Freq = wave1_index * 1000;
			wave2_Freq = wave2_index * 1000;
			
			phase1 = FFT_Phase[wave1_index];
			phase2 = FFT_Phase[wave2_index];
			
      		printf("wave1:%d    Fre:%d kHz	Phase: %.2f d\r\n",based_wave1_state,wave1_index, phase1);
			printf("wave2:%d    Fre:%d kHz	Phase: %.2f d\r\n",based_wave2_state,wave2_index, phase2);
			
			// Set_DAC_Waveform_AutoHighRes(wave1_Freq, 90.0f, based_wave1_state);
			// Set_DAC2_Waveform_AutoHighRes(wave2_Freq, 0.0f, based_wave2_state);
			// printf("DAC Init Over\r\n");
			
			HAL_Delay(10000);
			HAL_TIM_Base_Start(&htim3);
			HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&ADC_Value,NPT);
		}

//   }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc == &hadc1)
  {
    adcflag = 1; // Handle ADC conversion complete event
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
