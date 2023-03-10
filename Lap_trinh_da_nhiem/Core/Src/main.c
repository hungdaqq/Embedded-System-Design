/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ds18b20.h"
#include "sg90.h"
#include "i2c-lcd.h"
#include "uart.h"
#include "timer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId ReadTempTaskHandle;
uint32_t ReadTempTaskBuffer[ 128 ];
osStaticThreadDef_t ReadTempTaskControlBlock;
osThreadId UartTaskHandle;
uint32_t UartTaskBuffer[ 128 ];
osStaticThreadDef_t UartTaskControlBlock;
osThreadId ControlSg90TaskHandle;
uint32_t ControlSg90TaskBuffer[ 128 ];
osStaticThreadDef_t ControlSg90TaskControlBlock;
osThreadId AlarmChangeTaskHandle;
uint32_t AlarmChangeTaskBuffer[ 128 ];
osStaticThreadDef_t AlarmChangeTaskControlBlock;
osThreadId SendAlarmTaskHandle;
uint32_t SendAlarmTaskBuffer[ 128 ];
osStaticThreadDef_t SendAlarmTaskControlBlock;
osThreadId DisplayTempTaskHandle;
uint32_t DisplayTempTaskBuffer[ 128 ];
osStaticThreadDef_t DisplayTempTaskControlBlock;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void ReadTemp(void const * argument);
void Uart(void const * argument);
void ControlSg90(void const * argument);
void AlarmChange(void const * argument);
void SendAlarm(void const * argument);
void DisplayTemp(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t rxBuff[BF_SIZE]; 

int8_t ex_strlen; 

uint16_t pwm_value, pwm_value_pre;
float temp = 0;
bool check = true;
bool alarm_change = false;

int i = 0; 
uint8_t alarm_temp = 30;
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
  MX_I2C1_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	lcd_init();
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of ReadTempTask */
  osThreadStaticDef(ReadTempTask, ReadTemp, osPriorityHigh, 0, 128, ReadTempTaskBuffer, &ReadTempTaskControlBlock);
  ReadTempTaskHandle = osThreadCreate(osThread(ReadTempTask), NULL);

  /* definition and creation of UartTask */
  osThreadStaticDef(UartTask, Uart, osPriorityAboveNormal, 0, 128, UartTaskBuffer, &UartTaskControlBlock);
  UartTaskHandle = osThreadCreate(osThread(UartTask), NULL);

  /* definition and creation of ControlSg90Task */
  osThreadStaticDef(ControlSg90Task, ControlSg90, osPriorityNormal, 0, 128, ControlSg90TaskBuffer, &ControlSg90TaskControlBlock);
  ControlSg90TaskHandle = osThreadCreate(osThread(ControlSg90Task), NULL);

  /* definition and creation of AlarmChangeTask */
  osThreadStaticDef(AlarmChangeTask, AlarmChange, osPriorityNormal, 0, 128, AlarmChangeTaskBuffer, &AlarmChangeTaskControlBlock);
  AlarmChangeTaskHandle = osThreadCreate(osThread(AlarmChangeTask), NULL);

  /* definition and creation of SendAlarmTask */
  osThreadStaticDef(SendAlarmTask, SendAlarm, osPriorityNormal, 0, 128, SendAlarmTaskBuffer, &SendAlarmTaskControlBlock);
  SendAlarmTaskHandle = osThreadCreate(osThread(SendAlarmTask), NULL);

  /* definition and creation of DisplayTempTask */
  osThreadStaticDef(DisplayTempTask, DisplayTemp, osPriorityNormal, 0, 128, DisplayTempTaskBuffer, &DisplayTempTaskControlBlock);
  DisplayTempTaskHandle = osThreadCreate(osThread(DisplayTempTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_ReadTemp */
/**
  * @brief  Function implementing the ReadTempTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ReadTemp */
void ReadTemp(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
		read_temp_ds18b20(&temp);
    osDelay(1000);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Uart */
/**
* @brief Function implementing the UartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Uart */
void Uart(void const * argument)
{
  /* USER CODE BEGIN Uart */
  /* Infinite loop */
  for(;;)
  {
		RX_handle();
		if (alarm_change)
			{
				printf("Nhap nguong nhiet: \n");
			}
		else
			if (!check)
				{
					printf("Nhap goc quay: \n");
				}
    osDelay(500);
  }
  /* USER CODE END Uart */
}

/* USER CODE BEGIN Header_ControlSg90 */
/**
* @brief Function implementing the ControlSg90Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ControlSg90 */
void ControlSg90(void const * argument)
{
  /* USER CODE BEGIN ControlSg90 */
  /* Infinite loop */
  for(;;)
  {
		change_sg90();
    osDelay(500);
  }
  /* USER CODE END ControlSg90 */
}

/* USER CODE BEGIN Header_AlarmChange */
/**
* @brief Function implementing the AlarmChangeTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AlarmChange */
void AlarmChange(void const * argument)
{
  /* USER CODE BEGIN AlarmChange */
  /* Infinite loop */
  for(;;)
  {
		change_alarm();
    osDelay(500);
  }
  /* USER CODE END AlarmChange */
}

/* USER CODE BEGIN Header_SendAlarm */
/**
* @brief Function implementing the SendAlarmTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SendAlarm */
void SendAlarm(void const * argument)
{
  /* USER CODE BEGIN SendAlarm */
  /* Infinite loop */
  for(;;)
  {
		set_alarm_led();
    osDelay(500);
  }
  /* USER CODE END SendAlarm */
}

/* USER CODE BEGIN Header_DisplayTemp */
/**
* @brief Function implementing the DisplayTempTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DisplayTemp */
void DisplayTemp(void const * argument)
{
  /* USER CODE BEGIN DisplayTemp */
  /* Infinite loop */
  for(;;)
  {
		lcd_display_temp();
    osDelay(1000);
  }
  /* USER CODE END DisplayTemp */
}

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

#ifdef  USE_FULL_ASSERT
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
