#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>

#pragma pack(push, 1)
typedef struct _USBLEDSetPatternMessage
{
	uint8_t pattern;
} USBLEDSetPatternMessage_t;
#pragma pack(pop)

#endif