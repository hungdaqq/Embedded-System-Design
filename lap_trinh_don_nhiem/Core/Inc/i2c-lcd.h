#include "stm32f1xx_hal.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

void MX_I2C1_Init(void);

void lcd_init (void);   // khoi tao lcd

void lcd_send_cmd (char cmd);  // gui cac lenh len lcd

void lcd_send_data (char data);  // gui du lieu lcd

void lcd_send_string (char *str);  // gui chuoi lcd

void lcd_put_cur(int row, int col);  // dat toa do cho vi tri nhap len lcd

void lcd_clear (void); // xoa lcd
void lcd_display_temp (void); // hien thi nhiet do len lcd
