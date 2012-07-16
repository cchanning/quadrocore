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

static volatile uint8_t ledPattern = 0;

void USBDeviceHIDSetLights(USBControlTransfer_t *usbControlTransferP)
{	
	ledPattern = *((uint8_t *)usbControlTransferP->usbDataBufferOutP + 1);
	PORTR.DIR = 0xFF;
	PORTR.OUT = (register8_t)ledPattern;	
}

void USBDeviceHIDGetLights(USBControlTransfer_t *usbControlTransferP)
{
	*((uint8_t *)usbControlTransferP->usbDataBufferInP) = 0x02;
	*((uint8_t *)usbControlTransferP->usbDataBufferInP + 1) = ledPattern;
	
	usbControlTransferP->actualLength = 2;
}