#include "sg90.h"
void user_pwm_setvalue(uint16_t value)
{
    TIM_OC_InitTypeDef sConfigOC;
  
    sConfigOC.OCMode = TIM_OCMODE_PWM1; // Che do dao xung PWM
    sConfigOC.Pulse = value; // Gia tri do rong xung PWM
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // Dien ap dau ra khi ket thuc xung PWM la cao
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE; // Che do khong nhanh
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1); // Cau hinh kenh dau ra cua Timer 4
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1); // Bat dau tao xung PWM tren kenh 1 cua Timer 4
    if (HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1) != HAL_OK) {
    // Neu khong the dung Timer 4, in ra thong bao loi
    printf("Loi: Khong the dung Timer 4.\n");
}

// Cau hinh kenh dau ra cua Timer 4
if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
    // Neu khong the cau hinh kenh dau ra, in ra thong bao loi
    printf("Loi: Khong the cau hinh kenh dau ra cua Timer 4.\n");
}

// Bat dau tao xung PWM tren kenh 1 cua Timer 4
if (HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1) != HAL_OK) {
    // Neu khong the bat dau tao xung PWM, in ra thong bao loi
    printf("Loi: Khong the bat dau tao xung PWM tren kenh %d cua Timer 4.\n", TIM_CHANNEL_1);
}
}
void change_sg90 (void)
{
	if (check)
  		{
    		pwm_value = 500+(temp-20)*(2500-500)/(40-20);
  		}
	else if ((ex_strlen != strlen((char*)rxBuff))&&(!alarm_change))
		{
			uint8_t angle = 0;
			for (i = strlen((char*)rxBuff)-1; i>=ex_strlen; i--)
			{	
				if (rxBuff[i] >= '0' && rxBuff[i] <= '9')
					angle = angle+(rxBuff[i]-'0')*pow(10,strlen((char*)rxBuff)-i-1);
				else
					break;
			}
			ex_strlen = strlen((char*)rxBuff);
		if (angle >180)
			angle = 180;
		printf("goc: %d",angle);
		pwm_value = 500+ angle *(2500-500)/180;
		}
	if (pwm_value_pre != pwm_value)
		{
			pwm_value_pre = pwm_value;
			user_pwm_setvalue(pwm_value_pre);
		}
}
