/***********************************************************************************************************************
 * 
 * > QuadroCore <
 * 
 * Copyright (C) 2012 by Chris Channing
 *
 ***********************************************************************************************************************
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
 * an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 ***********************************************************************************************************************/

#include "quadrocore.h"

void LCDReset(void);
void LCDEnableBackLight(void);
void LCDInitSPI(void);
void LCDWriteCommand(uint8_t command);
void LCDWriteData(uint8_t data);
void LCDSetSelected(bool_t selected);
void LCDSetCommandMode(void);
void LCDSetDataMode(void);
void LCDEnable(void);
void LCDTransmit(uint8_t value);

void LCDInit(void)
{
	LCDReset();
	LCDInitSPI();
	LCDEnableBackLight();
	LCDEnable();
}

void LCDWriteString(char *value)
{
	LCDSetSelected(true);
	{
		LCDSetDataMode();
		LCDTransmit(0xFF);
		
	}
	LCDSetSelected(false);	
}

void LCDClear(void)
{
	
}

inline void LCDReset(void)
{
	PortSetPinLow(&PORTA, 3);
	sleep(10);
	PortSetPinHigh(&PORTA, 3);
	sleep(10);
}

inline void LCDEnableBackLight(void)
{
	PortSetPinHigh(&PORTE, 4);
}

inline void LCDInitSPI(void)
{
	// enable MSPI, set UDORD to MSB first, set UCPHA = 1
	USARTD0.CTRLC = (USART_CMODE_MSPI_gc | 0x02);
	
	// configure the clock pin for PORTD
	PortSetPinHigh(&PORTD, 1);
	PORTD.PIN1CTRL = PORT_INVEN_bm;
	
	// calculate and set the BAUD rate
	{
		uint16_t bsel_value = (uint16_t)((((((CPU_HZ) << 1) / (LCD_BAUD * 2)) + 1) >> 1) - 1);
		USARTD0.BAUDCTRLB = (uint8_t)((~USART_BSCALE_gm) & (bsel_value >> 8));
		USARTD0.BAUDCTRLA = (uint8_t)(bsel_value);
	}
	
	// enable transmitter
	USARTD0.CTRLB = USART_TXEN_bm;
}

void LCDWriteCommand(uint8_t command)
{
	LCDSetSelected(true);
	{
		LCDSetCommandMode();
		LCDTransmit(command);
		
	}	
	LCDSetSelected(false);
}

void LCDWriteData(uint8_t data)
{
	LCDSetSelected(true);
	{
		LCDSetDataMode();
	}
	LCDSetSelected(false);
}

void LCDSetSelected(bool_t selected)
{
	if (selected)
	{
		PortSetPinLow(&PORTD, 0);	
	}
	else
	{
		PortSetPinHigh(&PORTD, 0);		
	}
}

void LCDSetCommandMode(void)
{
	PortSetPinLow(&PORTF, 3);
}

void LCDSetDataMode(void)
{
	PortSetPinHigh(&PORTF, 3);
}

void LCDEnable(void)
{	
	// The column address is set to increasing
	LCDWriteCommand(ST7565R_CMD_ADC_NORMAL);

	// Non-inverted display
	LCDWriteCommand(ST7565R_CMD_DISPLAY_NORMAL);

	// The common mode scan direction is reversed COM31->COM0
	LCDWriteCommand(ST7565R_CMD_REVERSE_SCAN_DIRECTION);

	// Set the voltage bias ratio to 1/6
	LCDWriteCommand(ST7565R_CMD_LCD_BIAS_1_DIV_6_DUTY33);

	// Set booster circuit, voltage regulator and voltage follower all to on
	LCDWriteCommand(ST7565R_CMD_POWER_CTRL_ALL_ON);

	// Set the booster ratio to 2X,3X,4X
	LCDWriteCommand(ST7565R_CMD_BOOSTER_RATIO_SET);
	LCDWriteCommand(ST7565R_CMD_BOOSTER_RATIO_2X_3X_4X);

	// Set voltage resistor ratio to 1
	LCDWriteCommand(ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_1);

	/* Set contrast to max value*/
	LCDWriteCommand(ST7565R_CMD_ELECTRONIC_VOLUME_MODE_SET);
	LCDWriteCommand(ST7565R_CMD_ELECTRONIC_VOLUME(ST7565R_DISPLAY_CONTRAST_MAX));
	
	LCDWriteCommand(ST7565R_CMD_DISPLAY_ALL_POINTS_ON);
}

void LCDTransmit(uint8_t value)
{
	while (USART_DREIF_bm != (USARTD0.STATUS & USART_DREIF_bm))
	{
		// wait until the data register is ready
	}
	
	// transmit data
	USARTD0.DATA = value;
	
	while (USART_TXCIF_bm != (USARTD0.STATUS & USART_TXCIF_bm))
	{
		// wait until the data has been sent through to the LCD
	}
	
	// clear the tx complete flag
	USARTD0.STATUS = USART_TXCIF_bm;
}