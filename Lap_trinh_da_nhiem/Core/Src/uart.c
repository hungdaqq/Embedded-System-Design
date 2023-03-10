#include "uart.h"

UART_HandleTypeDef huart2; //khai bao uart2
DMA_HandleTypeDef hdma_usart2_rx; //khai bao nhan du lieu tu uart2 bang DMA

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  HAL_UART_Receive_DMA(&huart2, rxBuff, sizeof(rxBuff));
  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}

void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Alarm_led_GPIO_Port, Alarm_led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Alarm_led_Pin */
  GPIO_InitStruct.Pin = Alarm_led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Alarm_led_GPIO_Port, &GPIO_InitStruct);

}
void RX_handle(void)
{
	char s1[8];
	strncpy(s1, (char*)(rxBuff + strlen((char*)rxBuff) - 7), 7);
	s1[7] = '\0';
	if (strlen((char*)rxBuff) > BF_SIZE-7)
	{
		//reset lai DMA va rxBuff
			memset(rxBuff, NULL, strlen((char*)rxBuff));
			__HAL_DMA_DISABLE(&hdma_usart2_rx); 
			hdma_usart2_rx.Instance->CNDTR = BF_SIZE; 
			__HAL_DMA_ENABLE(&hdma_usart2_rx); 
			ex_strlen = 0;
	}
	if (!strcmp(s1,"maytinh"))
		{
			printf("yes\n");
			check = false; // dieu khien dong co qua may tinh
			alarm_change = false; // khong the doi nhiet do nguong
			//reset lai DMA va rxBuff
			ex_strlen = 0;
			memset(rxBuff, NULL, strlen((char*)rxBuff));
			__HAL_DMA_DISABLE(&hdma_usart2_rx); 
			hdma_usart2_rx.Instance->CNDTR = BF_SIZE; 
			__HAL_DMA_ENABLE(&hdma_usart2_rx); 
		}
	else if (!strcmp(s1,"nhietdo"))
		{
			check = true; // dieu khien dong co qua nhiet do
			alarm_change = false; // khong the doi nhiet do nguong
			//reset lai DMA va rxBuff
			memset(rxBuff, NULL, strlen((char*)rxBuff));
			__HAL_DMA_DISABLE(&hdma_usart2_rx); 
			hdma_usart2_rx.Instance->CNDTR = BF_SIZE; 
			__HAL_DMA_ENABLE(&hdma_usart2_rx); 

		}
	else if (!strcmp(s1,"tnguong"))
		{
			alarm_change = true;// thay doi nhiet do nguong
			//reset lai DMA va rxBuff
			memset(rxBuff, NULL, strlen((char*)rxBuff));
			__HAL_DMA_DISABLE(&hdma_usart2_rx); 
			hdma_usart2_rx.Instance->CNDTR = BF_SIZE; 
			__HAL_DMA_ENABLE(&hdma_usart2_rx); 
		}
}
// khai bao de thuc hien su dung ham printf qua UART TX
struct __FILE{
	int dummy;
};
FILE __stdout;

int fputc(int ch, FILE * f) {
  HAL_UART_Transmit( & huart2, (uint8_t * ) & ch, 1, HAL_MAX_DELAY);
  return ch;
}




