/*Includes*/
#include "stm32f4xx_hal.h"

extern void Error_Handler(void);
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{


  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt initialization*/
  /* MemoryManagement_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  /* BusFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  /* UsageFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  /* SVCall_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  /* DebugMonitor_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM3)
  {

    /* Peripheral clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
    /* Peripheral interrupt initialization */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

  }
  if(htim_base->Instance==TIM2)
    {

      /* Peripheral clock enable */
      __HAL_RCC_TIM2_CLK_ENABLE();
      /* Peripheral interrupt initialization */
      HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
      HAL_NVIC_EnableIRQ(TIM2_IRQn);

    }
  if(htim_base->Instance==TIM4)
      {
        /* Peripheral clock enable */
	  __HAL_RCC_TIM4_CLK_ENABLE();
	  	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);

      }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM3)
  {

    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /* Peripheral interrupt DeInit*/
    HAL_NVIC_DisableIRQ(TIM3_IRQn);

  }


}

