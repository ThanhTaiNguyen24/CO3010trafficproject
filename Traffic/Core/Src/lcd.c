/*
 * lcd.c
 *
 *  Created on: Oct 31, 2024
 *      Author: ASUS
 */


#include "lcd.h"

extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD (0x21 << 1) // change this according to ur setup

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_init (void) {
	lcd_send_cmd (0x33); /* set 4-bits interface */
	lcd_send_cmd (0x32);
	HAL_Delay(50);
	lcd_send_cmd (0x28); /* start to set LCD function */
	HAL_Delay(50);
	lcd_send_cmd (0x01); /* clear display */
	HAL_Delay(50);
	lcd_send_cmd (0x06); /* set entry mode */
	HAL_Delay(50);
	lcd_send_cmd (0x0c); /* set display to on */
	HAL_Delay(50);
	lcd_send_cmd (0x02); /* move cursor to home and set data address to 0 */
	HAL_Delay(50);
	lcd_send_cmd (0x80);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_clear_display (void)
{
	lcd_send_cmd (0x01); //clear display
	HAL_Delay(2);
}

void lcd_goto_XY (int row, int col)
{
	uint8_t pos_Addr;
	if(row == 1)
	{
		pos_Addr = 0x80 + row - 1 + col;
	}
	else
	{
		pos_Addr = 0x80 | (0x40 + col);
	}
	lcd_send_cmd(pos_Addr);
}

void updateLCDBuffer(){
	if (timer3_flag == 1){
		lcd_clear_display();
		char LCDBuffer6[16];
		char LCDBuffer7[16];
		lcd_goto_XY(1, 0);
		sprintf(LCDBuffer6, "Road 1: %ds", road1duration/1000);
		lcd_send_string(LCDBuffer6);
		lcd_goto_XY(2, 0);
		sprintf(LCDBuffer7, "Road 2: %ds", road2duration/1000);
		lcd_send_string(LCDBuffer7);
		setTimer3(1000);
	}
}

void updateLCDManualRedBuffer(){
	if (timer3_flag == 1){
		lcd_clear_display();
		char LCDBuffer[16];
		char LCDBuffer1[16];
		lcd_goto_XY(1, 0);
		sprintf(LCDBuffer, "Duration: %ds", (currentred/1000));
		lcd_send_string(LCDBuffer);
		lcd_goto_XY(2, 0);
		sprintf(LCDBuffer1, "Mode: %d", status);
		lcd_send_string(LCDBuffer1);
		setTimer3(1000);
	}
}

void updateLCDManualYellowBuffer(){
	if (timer3_flag == 1){
		lcd_clear_display();
		char LCDBuffer2[16];
		char LCDBuffer3[16];
		lcd_goto_XY(1, 0);
		sprintf(LCDBuffer2, "Duration: %ds", (currentyellow/1000));
		lcd_send_string(LCDBuffer2);
		lcd_goto_XY(2, 0);
		sprintf(LCDBuffer3, "Mode: %d", (status));
		lcd_send_string(LCDBuffer3);
		setTimer3(1000);
	}
}

void updateLCDManualGreenBuffer(){
	if (timer3_flag == 1){
		lcd_clear_display();
		char LCDBuffer4[16];
		char LCDBuffer5[16];
		lcd_goto_XY(1, 0);
		sprintf(LCDBuffer4, "Duration: %ds", (currentgreen/1000));
		lcd_send_string(LCDBuffer4);
		lcd_goto_XY(2, 0);
		sprintf(LCDBuffer5, "Mode: %d", (status));
		lcd_send_string(LCDBuffer5);
		setTimer3(1000);
	}
}

