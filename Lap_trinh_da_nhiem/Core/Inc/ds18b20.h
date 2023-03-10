#ifndef __DS18B20_H
#define __DS18B20_H
#include "main.h"
#include "stm32f1xx_hal.h"

//Khoi tao cac ham 
void delay(uint32_t us); // khoi tao ham delay us
void ds18b20_read(void); // doc du lieu tu chan data cua cam bien
void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin); // dat chan GPIO la Output
void Set_Pin_Intput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin); // dat chan GPIO la Input
int8_t DS18B20_Start(void); // Khoi dong doc gia tri tu DS18B20, tra ve 1 neu co cam bien, -1 neu khong co
void DS18B20_Write (uint8_t data); // viet du lieu len cam bien qua chan data de dat cac chuc nang cho cam bien
uint8_t DS18B20_Read (void); // doc 8 bit nhan duoc tu cam bien qua OneWire
void read_temp_ds18b20(float* temperature); // doc gia tri cam bien tac dong vao gia tri ma con tro temperature tra ve
void change_alarm (void); // thay doi nguong nhiet canh bao
void set_alarm_led(void); // bat tat led canh bao

#endif

