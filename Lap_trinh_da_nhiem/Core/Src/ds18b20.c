#include "ds18b20.h"

int8_t Presence  = 0;

uint8_t Temp_byte1;
uint8_t Temp_byte2;
uint16_t TEMP;
void delay(uint32_t us)
{
  uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000);
  uint32_t start = SysTick->VAL;
  while (ticks > (start - SysTick->VAL)) { }
	
}

void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_IntStruct ={0};
	GPIO_IntStruct.Pin = GPIO_Pin;
	GPIO_IntStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_IntStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_IntStruct);
	
}

void Set_Pin_Intput(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_IntStruct ={0};
	GPIO_IntStruct.Pin = GPIO_Pin;
	GPIO_IntStruct.Mode = GPIO_MODE_INPUT;
	GPIO_IntStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_IntStruct);
}


int8_t DS18B20_Start(void)
{
    int8_t response = 0;
    
    // Dat DS18B20 la Output va keo ve 0 trong 480us
    Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
    delay(480);
    
    // Set the DS18B20 pin as input and wait for the presence pulse
    Set_Pin_Intput(DS18B20_PORT, DS18B20_PIN);
    delay(80);
    if (!HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)) {
        // Presence pulse detected, DS18B20 is present
        response = 1;
    } else {
        // No presence pulse detected, DS18B20 is not present or has an error
        response = -1;
    }
	printf("  ");
    delay(400);
    
    // Return the response
    return response;
}


void DS18B20_Write (uint8_t data)
{
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);  // set as output

	for (int i=0; i<8; i++)
	{

		if ((data & (1<<i))!=0)  // if the bit is high
		{
			// write 1

			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);  // set as output
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, (GPIO_PinState)0);  // pull the pin LOW
			delay(1);  // wait for 1 us

			Set_Pin_Intput(DS18B20_PORT, DS18B20_PIN);  // set as input
			delay(50);  // wait for 60 us
		}

		else  // if the bit is low
		{
			// write 0

			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN,(GPIO_PinState)0);  // pull the pin LOW
			delay(50);  // wait for 60 us

			Set_Pin_Intput(DS18B20_PORT, DS18B20_PIN);
		}
	}
}

uint8_t DS18B20_Read (void)
{
	uint8_t value=0;
	Set_Pin_Intput (DS18B20_PORT,DS18B20_PIN);

	for (int i=0;i<8;i++)
	{
		Set_Pin_Output(DS18B20_PORT,DS18B20_PIN);   // set as output

		HAL_GPIO_WritePin (DS18B20_PORT,DS18B20_PIN, (GPIO_PinState)0);  // pull the data pin LOW
		delay(2);  // wait for 2 us

		Set_Pin_Intput(DS18B20_PORT,DS18B20_PIN);  // set as input
		
		if (HAL_GPIO_ReadPin (DS18B20_PORT,DS18B20_PIN))  // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1
			
		}
		delay(60);  // wait for 60 us
	}
	return value;
}

void read_temp_ds18b20(float* temperature)
{
    Presence = DS18B20_Start();
    delay(1);
    DS18B20_Write(0xCC);  // skip ROM
    DS18B20_Write(0x44);  // convert t
    delay(800);

    Presence = DS18B20_Start();
    delay(1);
    DS18B20_Write(0xCC);  // skip ROM
    DS18B20_Write(0xBE);  // Read Scratch-pad

    Temp_byte1 = DS18B20_Read();
    Temp_byte2 = DS18B20_Read();
    TEMP = (Temp_byte2 << 8) | Temp_byte1;
    *temperature = (float)TEMP / 16;
}
void change_alarm (void)
{
	 if (alarm_change)
    	{
    		uint8_t temp_change = 0;
			for (i = strlen((char*)rxBuff)-1; i>=ex_strlen; i--)
			{	
				if (rxBuff[i] >= '0' && rxBuff[i] <= '9')
					temp_change = temp_change+(rxBuff[i]-'0')*pow(10,strlen((char*)rxBuff)-i-1);
				else
					break;
			}
			ex_strlen = strlen((char*)rxBuff);
			if ((temp_change != alarm_temp)&&(temp_change != 0))
				alarm_temp = temp_change;
    	}
}

void set_alarm_led(void)
{
	if (temp > alarm_temp) 
		{
			printf("Qua nguong nhiet do\n");
			HAL_GPIO_WritePin(Alarm_led_GPIO_Port, Alarm_led_Pin, (GPIO_PinState)0);
		}
	else 
		{
			HAL_GPIO_WritePin(Alarm_led_GPIO_Port, Alarm_led_Pin, (GPIO_PinState)1);
		}
}
