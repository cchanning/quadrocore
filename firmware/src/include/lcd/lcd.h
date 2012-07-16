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

#ifndef LCD_H_
#define LCD_H_

#define ST7565R_CMD_DISPLAY_ON                     0xAF
#define ST7565R_CMD_DISPLAY_OFF                    0xAE
#define ST7565R_CMD_START_LINE_SET(line)           (0x40 | (line))
#define ST7565R_CMD_PAGE_ADDRESS_SET(page)         (0xB0 | (page))
#define ST7565R_CMD_COLUMN_ADDRESS_SET_MSB(column) (0x10 | (column))
#define ST7565R_CMD_COLUMN_ADDRESS_SET_LSB(column) (0x00 | (column))
#define ST7565R_CMD_ADC_NORMAL                     0xA0
#define ST7565R_CMD_ADC_REVERSE                    0xA1
#define ST7565R_CMD_DISPLAY_NORMAL                 0xA6
#define ST7565R_CMD_DISPLAY_REVERSE                0xA7
#define ST7565R_CMD_DISPLAY_ALL_POINTS_OFF         0xA4
#define ST7565R_CMD_DISPLAY_ALL_POINTS_ON          0xA5
#define ST7565R_CMD_LCD_BIAS_1_DIV_5_DUTY33        0xA1
#define ST7565R_CMD_LCD_BIAS_1_DIV_6_DUTY33        0xA2
#define ST7565R_CMD_NORMAL_SCAN_DIRECTION          0xC0
#define ST7565R_CMD_REVERSE_SCAN_DIRECTION         0xC8
#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_0       0x20
#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_1       0x21
#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_2       0x22
#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_3       0x23
#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_4       0x24
#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_5       0x25
#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_6       0x26
#define ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_7       0x27
#define ST7565R_CMD_POWER_CTRL_ALL_ON              0x2F
#define ST7565R_CMD_SLEEP_MODE                     0xAC
#define ST7565R_CMD_NORMAL_MODE                    0xAD
#define ST7565R_CMD_RESET                          0xE2
#define ST7565R_CMD_NOP                            0xE3
#define ST7565R_CMD_ELECTRONIC_VOLUME_MODE_SET     0x81
#define ST7565R_CMD_ELECTRONIC_VOLUME(volume)      (0x3F & (~volume))
#define ST7565R_CMD_BOOSTER_RATIO_SET              0xF8
#define ST7565R_CMD_BOOSTER_RATIO_2X_3X_4X         0x00
#define ST7565R_CMD_BOOSTER_RATIO_5X               0x01
#define ST7565R_CMD_BOOSTER_RATIO_6X               0x03
#define ST7565R_CMD_STATUS_READ                    0x00
#define ST7565R_CMD_END                            0xEE
#define ST7565R_CMD_READ_MODIFY_WRITE              0xE0

#define ST7565R_DISPLAY_CONTRAST_MAX 40
#define ST7565R_DISPLAY_CONTRAST_MIN 30

#define LCD_BAUD ((uint32_t)1000000)

void LCDInit(void);
void LCDWriteString(char *value);
void LCDClear(void);

#endif /* LCD_H_ */