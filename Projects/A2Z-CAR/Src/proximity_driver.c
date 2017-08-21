/*
 * proximity_driver.c
 *
 *  Created on: 2017. aug. 15.
 *      Author: AND
 */
#include "proximity_driver.h"

static void EXTI15_10_IRQHandler_Config(void);
uint32_t proxim1_cntr = 0;
int8_t proxim1_up = 0;
uint32_t tickstart;
uint32_t tickstop;
uint32_t cm_cntr = 0;

int8_t proximity_driver_init()
{
	proximity_sensor_trigger_init();
	proximity1_exti_init();
	proximity_timer_init();

	return 0;
}



int8_t proximity_sensor_trigger_init()
{
	//init D3 (PB0) as trigger for proximity sensors
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_0;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_Init);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

	printf("Proxim trigger init done.\n");

	return 0;
}

void proximity_send_trigger()
{
	//printf("Proxim trigger sending started.\n");

	//init trigger pin to D3 (PB0)

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_Delay(1500);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	printf("Proxim trigger sent.\n");
	HAL_Delay(250);
}

int8_t proximity1_exti_init()
{
	//init D2 (PD14) EXTI mode
	EXTI15_10_IRQHandler_Config();

	printf("Proxim sensor2 init done.\n");

	return 0;

}

int8_t proximity_control_thread()
{

	while (1){

		proxim1_cntr = 0;
		proxim1_up = 0;
		proximity_send_trigger();

		while (proxim1_up == 1){

		}
		proxim1_cntr = cm_cntr;
		printf("proxim1_cntr: %lu\n\n", proxim1_cntr);
		cm_cntr = 0;
	}
	return 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if ((GPIO_Pin == GPIO_PIN_14) && (proxim1_up == 0)) {
	  proxim1_up = 1;
	  HAL_TIM_Base_Start_IT(&proxim_timer_handle);
	  printf("proxim1_up: %d\n", proxim1_up);

  } else if ((GPIO_Pin == GPIO_PIN_14) && (proxim1_up == 1)) {
	  proxim1_up = 0;
	  printf("proxim1_up: %d\n", proxim1_up);
	  HAL_TIM_Base_Stop_IT(&proxim_timer_handle);

  }
}

static void EXTI15_10_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /* Configure PD.14 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_14;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Enable and set EXTI lines 10 to 15 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

int8_t proximity_timer_init()
{
	//init TIM4_CH3 10 kHz x 0,58 = 1 periode / 1 cm
	proxim_timer_handle.Instance = TIM4;
	proxim_timer_handle.State = HAL_TIM_STATE_RESET;
	proxim_timer_handle.Channel = HAL_TIM_ACTIVE_CHANNEL_3;
	proxim_timer_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	proxim_timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	proxim_timer_handle.Init.Period = 9280;
	proxim_timer_handle.Init.Prescaler = 4;
	HAL_TIM_Base_Init(&proxim_timer_handle);
	HAL_TIM_Base_Start_IT(&proxim_timer_handle);

	printf("TIM4 init done.\n");

	HAL_NVIC_DisableIRQ(TIM3_IRQn);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
	cm_cntr++;
 }

