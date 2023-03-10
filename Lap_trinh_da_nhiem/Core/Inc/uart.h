#ifndef __UART_H
#define __UART_H

#include "main.h"

// khai bao extern de co the su dung bien tai cac file .c khac
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

void MX_USART2_UART_Init(void);
void MX_DMA_Init(void);
void MX_GPIO_Init(void);
void RX_handle(void);
int fputc(int ch, FILE * f);
#endif

