


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "usb_device.h"

/* USER CODE BEGIN Includes */
#include "Marlin_export.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef   TimerHeaterAndPwm,
					TimerStepper,
					Timer_usDelay;
ADC_HandleTypeDef adc1Handler;
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void timerHeaterAndPwmInit(void);
static void timer_usInit(void);
static void timerStepperInit(void);
static void ADC1_INIT(void);

/* Private function prototypes -----------------------------------------------*/

USB_OTG_DeviceTypeDef *usbHdl;
int main(void)
{

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  timerHeaterAndPwmInit();
  timerStepperInit();
  timer_usInit();
  ADC1_INIT();

  HAL_ADC_Start(&adc1Handler);
  HAL_TIM_Base_Start_IT(&TimerHeaterAndPwm);
  HAL_TIM_Base_Start_IT(&TimerStepper);
  HAL_TIM_Base_Start(&Timer_usDelay);

  while(hUsbDeviceFS.dev_state!=3)
  HAL_Delay(500);
  setup();
  while (1)
  {
	  loop();
  }


}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
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

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
/*Initialize ADC*/
static void ADC1_INIT(){
	__HAL_RCC_ADC1_CLK_ENABLE();
	GPIO_InitTypeDef initStr;
	initStr.Pin = GPIO_PIN_0;
	initStr.Mode = GPIO_MODE_ANALOG;
	initStr.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &initStr);
	ADC_ChannelConfTypeDef adcChannel;


	adc1Handler.Instance = ADC1;
	adc1Handler.Init.ClockPrescaler= ADC_CLOCKPRESCALER_PCLK_DIV4;
	adc1Handler.Init.Resolution = ADC_RESOLUTION10b;
	adc1Handler.Init.ScanConvMode = DISABLE;
	adc1Handler.Init.ContinuousConvMode = ENABLE;
	adc1Handler.Init.DiscontinuousConvMode = DISABLE;
	adc1Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	adc1Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adc1Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc1Handler.Init.NbrOfConversion = 1;
	adc1Handler.Init.DMAContinuousRequests = DISABLE;
	adc1Handler.Init.EOCSelection = DISABLE;
	if (HAL_ADC_Init(&adc1Handler) != HAL_OK)
	  {
	    Error_Handler();
	  }
	adcChannel.Channel=ADC_CHANNEL_0;
	adcChannel.Rank = 1;
	adcChannel.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	  if (HAL_ADC_ConfigChannel(&adc1Handler, &adcChannel) != HAL_OK)
	  {
	    Error_Handler();
	  }

}
/*Initialize stepperTimer*/
static void timerStepperInit(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  TimerStepper.Instance = TIM2;
  TimerStepper.Init.Prescaler = 42;
  TimerStepper.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimerStepper.Init.Period = 0x4000;
  TimerStepper.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&TimerStepper) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&TimerStepper, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&TimerStepper, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
/* Initialize TimerHeaterAndPwmInit */
static void timerHeaterAndPwmInit(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  TimerHeaterAndPwm.Instance = TIM3;
  TimerHeaterAndPwm.Init.Prescaler = 84;
  TimerHeaterAndPwm.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimerHeaterAndPwm.Init.Period = 2000;
  TimerHeaterAndPwm.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&TimerHeaterAndPwm) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&TimerHeaterAndPwm, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&TimerHeaterAndPwm, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/*Initialize timer_usInit*/
static void timer_usInit(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  Timer_usDelay.Instance = TIM4;
  Timer_usDelay.Init.Prescaler = 84;
  Timer_usDelay.Init.CounterMode = TIM_COUNTERMODE_UP;
  Timer_usDelay.Init.Period = 0xFFFF;
  Timer_usDelay.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&Timer_usDelay) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&Timer_usDelay, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&Timer_usDelay, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{
	   GPIO_InitTypeDef initStruct;

	   	__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOH_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();
		__HAL_RCC_GPIOE_CLK_ENABLE();

	   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

	   initStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	   initStruct.Mode = GPIO_MODE_OUTPUT_PP;

	   initStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
	   HAL_GPIO_Init(GPIOG, &initStruct);

	   initStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
	   HAL_GPIO_Init(GPIOD, &initStruct);

	   initStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
	   HAL_GPIO_Init(GPIOE, &initStruct);

	   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);


}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{

  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{

  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

}

#endif

