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

static bool_t usbSetupRequestHandlerTableInitialized = false;
static USBStandardRequestHandler_t USB_STANDARD_REQUEST_HANDLER_TABLE[USB_REQUEST_TYPE_HANDLER_COUNT];

static const UTF8String_t USB_DEVICE_STRINGS[] =
{
	{
		.charP = L"> QuadroCore <",
		.lengthInBytes = sizeof(L"> QuadroCore <")
	},
	{
		.charP = L"QuadroCopter",
		.lengthInBytes = sizeof(L"QuadroCopter")
	},
	{
		.charP = L"QuadroCopter HID Interface",
		.lengthInBytes = sizeof(L"QuadroCopter HID Interface")
	}
};

void USBDeviceSetAddressCallback(ptr_t dataP);

USBStandardRequestHandler_t* USBStandardRequestHandlerTableGet(void)
{
	if (! usbSetupRequestHandlerTableInitialized)
	{
		int i = 0;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].type = USB_REQUEST_TYPE_FLD_TYPE_STANDARD_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].recipient = USB_REQUEST_TYPE_FLD_RECIPIENT_DEVICE_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].id = USB_REQUEST_DEVICE_SET_ADDRESS;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].handlerFuncP = &USBDeviceSetDeferredAddress;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueHighByte = false;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueLowByte = false;
		
		i++;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].type = USB_REQUEST_TYPE_FLD_TYPE_STANDARD_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].recipient = USB_REQUEST_TYPE_FLD_RECIPIENT_DEVICE_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].id = USB_REQUEST_DEVICE_GET_DESCRIPTOR;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].handlerFuncP = &USBDeviceGetDescriptor;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueHighByte = true;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueLowByte = false;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].valueHighByte = 0x01;
		
		i++;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].type = USB_REQUEST_TYPE_FLD_TYPE_STANDARD_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].recipient = USB_REQUEST_TYPE_FLD_RECIPIENT_DEVICE_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].id = USB_REQUEST_DEVICE_GET_DESCRIPTOR;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].handlerFuncP = &USBDeviceGetConfigurationDescriptor;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueHighByte = true;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueLowByte = false;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].valueHighByte = 0x02;
		
		i++;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].type = USB_REQUEST_TYPE_FLD_TYPE_STANDARD_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].recipient = USB_REQUEST_TYPE_FLD_RECIPIENT_DEVICE_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].id = USB_REQUEST_DEVICE_GET_DESCRIPTOR;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].handlerFuncP = &USBDeviceGetString;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueHighByte = true;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueLowByte = false;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].valueHighByte = 0x03;
		
		i++;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].type = USB_REQUEST_TYPE_FLD_TYPE_STANDARD_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].recipient = USB_REQUEST_TYPE_FLD_RECIPIENT_DEVICE_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].id = USB_REQUEST_DEVICE_GET_CONFIGURATION;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].handlerFuncP = &USBDeviceGetConfiguration;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueHighByte = false;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueLowByte = false;
		
		i++;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].type = USB_REQUEST_TYPE_FLD_TYPE_STANDARD_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].recipient = USB_REQUEST_TYPE_FLD_RECIPIENT_DEVICE_bm;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].id = USB_REQUEST_DEVICE_SET_CONFIGURATION;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].handlerFuncP = &USBDeviceSetConfiguration;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueHighByte = false;
		USB_STANDARD_REQUEST_HANDLER_TABLE[i].compareValueLowByte = false;
				
		usbSetupRequestHandlerTableInitialized = true;
	}
	
	return USB_STANDARD_REQUEST_HANDLER_TABLE;
}

USBStandardRequestHandler_t* USBDeviceStandardRequestResolveHandler(USBStandardRequest_t *usbStandardRequestP)
{
	USBStandardRequestHandler_t *usbStandardRequestHandlerTableP = USBStandardRequestHandlerTableGet();
		
	if (! usbStandardRequestHandlerTableP)
	{
		return NULL;
	}
		
	//decode the type and recipient from the request
	{
		uint8_t type = usbStandardRequestP->requestType & USB_REQUEST_TYPE_FLD_TYPE_bm;
		uint8_t recipient = usbStandardRequestP->requestType & USB_REQUEST_TYPE_FLD_RECIPIENT_bm;
		
		for (USBStandardRequestHandler_t *usbStandardRequestHandlerP = usbStandardRequestHandlerTableP; usbStandardRequestHandlerP < (usbStandardRequestHandlerTableP + USB_REQUEST_TYPE_HANDLER_COUNT); usbStandardRequestHandlerP++)
		{
			// if we get match on the id, just double check that we're scoped properly for request just in case request ids are not unique
			if ((usbStandardRequestHandlerP->id == usbStandardRequestP->request) && (usbStandardRequestHandlerP->recipient == recipient) && (usbStandardRequestHandlerP->type == type))
			{
				// we need to examine the request further, as some requests use the value field to specify additional qualifying information e.g. descriptor type
				if ((usbStandardRequestHandlerP->compareValueHighByte) || (usbStandardRequestHandlerP->compareValueLowByte))
				{
					uint16_t value = usbStandardRequestP->value;
					uint8_t valueHighByte = (value >> 8);
					uint8_t valueLowByte = (value & 0x00FF);
					
					if ((usbStandardRequestHandlerP->compareValueHighByte) && (usbStandardRequestHandlerP->compareValueLowByte))
					{
						if ((usbStandardRequestHandlerP->valueHighByte == valueHighByte) && (usbStandardRequestHandlerP->valueLowByte == valueLowByte))
						{
							return usbStandardRequestHandlerP;
						}	
					}
					else if ((usbStandardRequestHandlerP->compareValueHighByte) && (! usbStandardRequestHandlerP->compareValueLowByte))
					{
						if (usbStandardRequestHandlerP->valueHighByte == valueHighByte)
						{
							return usbStandardRequestHandlerP;
						}
					}
					else if ((! usbStandardRequestHandlerP->compareValueHighByte) && (usbStandardRequestHandlerP->compareValueLowByte))
					{
						if (usbStandardRequestHandlerP->valueLowByte == valueLowByte)
						{
							return usbStandardRequestHandlerP;
						}												
					}
				}
				else
				{
					return usbStandardRequestHandlerP;
				}
			}
		}			
	}
				
	return NULL;
}

void USBDeviceParseStandardRequestMetaData(USBControlTransfer_t *usbControlTransferP)
{
	USBStandardRequest_t *usbStandardRequestP = (USBStandardRequest_t *)usbControlTransferP->usbRequestP;
	usbControlTransferP->requestedLength = usbStandardRequestP->length;
	usbControlTransferP->actualLength = 0;
	usbControlTransferP->usbTransferDirection = ((usbStandardRequestP->requestType >> 7) > 0 ? USB_TRANSFER_DIRECTION_IN : USB_TRANSFER_DIRECTION_OUT);
}

bool_t USBDeviceProcessStandardRequest(USBControlTransfer_t *usbControlTransferP)
{
	USBStandardRequestHandler_t *usbStandardRequestHandlerP = USBDeviceStandardRequestResolveHandler((USBStandardRequest_t *)usbControlTransferP->usbRequestP);
		
	// this request isn't supported
	if (! usbStandardRequestHandlerP)
	{
		return false;
	}
	
	// delegate to the request handler
	(*usbStandardRequestHandlerP->handlerFuncP)(usbControlTransferP);
	
	return true;
}

void USBDeviceReset(void)
{
	USBControlTransferResetAll();
	USBEndpointResetAll();
	USBDeviceSetAddress(0);
	USB.STATUS &= ~USB_BUSRST_bm;
}

void USBDeviceGetDescriptor(USBControlTransfer_t *usbControlTransferP)
{
	USBStandardDeviceDescriptor_t *usbStandardDeviceDescriptorP = (USBStandardDeviceDescriptor_t *)usbControlTransferP->usbDataBufferInP;
	
	usbStandardDeviceDescriptorP->length = sizeof(USBStandardDeviceDescriptor_t);
	usbStandardDeviceDescriptorP->descriptorType = USB_STANDARD_DESCRIPTOR_TYPE_DEVICE;
	usbStandardDeviceDescriptorP->deviceClass = 0x00;
	usbStandardDeviceDescriptorP->deviceSubClass = 0x00;
	usbStandardDeviceDescriptorP->deviceProtocol = 0x00;
	usbStandardDeviceDescriptorP->maxPacketSize = usbControlTransferP->usbEndpointP->usbEndpointConfigurationP->maxPacketSize;
	usbStandardDeviceDescriptorP->vendorId = 0x03EB;
	usbStandardDeviceDescriptorP->productId = 0x2FE2;
	usbStandardDeviceDescriptorP->deviceVersion = 0x0002;
	usbStandardDeviceDescriptorP->manufacturerIndex = 0x01;
	usbStandardDeviceDescriptorP->productIndex = 0x02;
	usbStandardDeviceDescriptorP->serialNumberIndex = 0x00;
	usbStandardDeviceDescriptorP->numberOfConfigurations = 0x01;
	usbStandardDeviceDescriptorP->usbVersion = 0x0200;
	
	usbControlTransferP->actualLength = usbStandardDeviceDescriptorP->length;
}

void USBDeviceGetConfigurationDescriptor(USBControlTransfer_t *usbControlTransferP)
{
	// the device configuration goes first, but fill that in once we have populated the buffer
	ptr_t dataBufferP = usbControlTransferP->usbDataBufferInP + sizeof(USBStandardDeviceConfigurationDescriptor_t);
	
	// fill in the interface descriptor (advertise ourself as a HID)
	{
		USBStandardInterfaceConfigurationDescriptor_t *usbStandardInterfaceConfigurationDescriptorP = (USBStandardInterfaceConfigurationDescriptor_t *)dataBufferP;
		usbStandardInterfaceConfigurationDescriptorP->length = sizeof(USBStandardInterfaceConfigurationDescriptor_t);
		usbStandardInterfaceConfigurationDescriptorP->descriptorType = USB_STANDARD_DESCRIPTOR_TYPE_INTERFACE_CONFIGURATION;
		usbStandardInterfaceConfigurationDescriptorP->interfaceNumber = 0x00;
		usbStandardInterfaceConfigurationDescriptorP->alternateSetting = 0x00;
		usbStandardInterfaceConfigurationDescriptorP->numberOfEndpoints = 1;
		usbStandardInterfaceConfigurationDescriptorP->interfaceClass = 0x03;
		usbStandardInterfaceConfigurationDescriptorP->interfaceSubClass = 0x00;
		usbStandardInterfaceConfigurationDescriptorP->interfaceProtocol = 0x00;
		usbStandardInterfaceConfigurationDescriptorP->interfaceIndex = 0x03;
		
		dataBufferP += usbStandardInterfaceConfigurationDescriptorP->length;
		usbControlTransferP->actualLength += usbStandardInterfaceConfigurationDescriptorP->length;
	}
	
	// fill in the HID descriptor
	{
		USBStandardHIDConfigurationDescriptor_t *usbStandardHIDConfigurationDescriptorP = (USBStandardHIDConfigurationDescriptor_t *)dataBufferP;
		usbStandardHIDConfigurationDescriptorP->length = sizeof(USBStandardHIDConfigurationDescriptor_t);
		usbStandardHIDConfigurationDescriptorP->descriptorType = USB_CLASS_DESCRIPTOR_TYPE_HID;
		usbStandardHIDConfigurationDescriptorP->version = 0x0110;
		usbStandardHIDConfigurationDescriptorP->countryCode = 0x00;
		usbStandardHIDConfigurationDescriptorP->numberOfDescriptors = 0x01;
		usbStandardHIDConfigurationDescriptorP->nestedDescriptorType = USB_CLASS_DESCRIPTOR_TYPE_REPORT;
		usbStandardHIDConfigurationDescriptorP->nestedDesciptorLength = 0x01; // to do
		
		dataBufferP += usbStandardHIDConfigurationDescriptorP->length;
		usbControlTransferP->actualLength += usbStandardHIDConfigurationDescriptorP->length;
	}
	
	// fill in the endpoint descriptor
	{
		USBStandardEndpointConfigurationDescriptor_t *usbStandardEndpointConfigurationDescriptorP = (USBStandardEndpointConfigurationDescriptor_t *)dataBufferP;
		usbStandardEndpointConfigurationDescriptorP->length = sizeof(USBStandardEndpointConfigurationDescriptor_t);
		usbStandardEndpointConfigurationDescriptorP->descriptorType = USB_STANDARD_DESCRIPTOR_TYPE_ENDPOINT;
		usbStandardEndpointConfigurationDescriptorP->endpointAddress = 0x81;
		usbStandardEndpointConfigurationDescriptorP->attributes = 0x03; // interrupt data enpoint
		usbStandardEndpointConfigurationDescriptorP->maxPacketSize = usbControlTransferP->usbEndpointP->usbEndpointConfigurationP->maxPacketSize;
		usbStandardEndpointConfigurationDescriptorP->interval = 0xFF; // poll every 255ms as we're not going to use this endpoint for sending data back (we'll use endpoint 0 instead)
		
		dataBufferP += usbStandardEndpointConfigurationDescriptorP->length;
		usbControlTransferP->actualLength += usbStandardEndpointConfigurationDescriptorP->length;
	}
	
	// finally, fill in the device configuration descriptor (starting at the beginning of the data buffer)
	{
		USBStandardDeviceConfigurationDescriptor_t *usbStandardDeviceConfigurationDescriptorP = (USBStandardDeviceConfigurationDescriptor_t *)usbControlTransferP->usbDataBufferInP;
		usbStandardDeviceConfigurationDescriptorP->length = sizeof(USBStandardDeviceConfigurationDescriptor_t);
		usbStandardDeviceConfigurationDescriptorP->descriptorType = USB_STANDARD_DESCRIPTOR_TYPE_DEVICE_CONFIGURATION;
		usbStandardDeviceConfigurationDescriptorP->totalLength = usbStandardDeviceConfigurationDescriptorP->length + usbControlTransferP->actualLength;
		usbStandardDeviceConfigurationDescriptorP->numInterfaces = 0x01;
		usbStandardDeviceConfigurationDescriptorP->configurationValue = 0x01;
		usbStandardDeviceConfigurationDescriptorP->configurationIndex = 0x01;
		usbStandardDeviceConfigurationDescriptorP->attributes = 0x80;
		usbStandardDeviceConfigurationDescriptorP->maxPower = 0x32;	
		
		usbControlTransferP->actualLength += usbStandardDeviceConfigurationDescriptorP->length;
	}
}

void USBDeviceSetDeferredAddress(USBControlTransfer_t *usbControlTransferP)
{
	//a usb address is only 7 bits long, guarantee we don't get any other garbage in the 16bit request value
	uint8_t *addressP = calloc(1, sizeof(uint8_t));
	*addressP = ((USBStandardRequest_t*)usbControlTransferP->usbRequestP)->value & 0x007F;
	
	usbControlTransferP->completionStageFuncP = &USBDeviceSetAddressCallback;
	usbControlTransferP->completionStageDataP = (ptr_t)addressP;
}

void USBDeviceSetAddressCallback(ptr_t addressP)
{
	USBDeviceSetAddress(*((uint8_t *)addressP));
}

void USBDeviceSetAddress(uint8_t address)
{
	USB.ADDR = (register8_t)address;	
}

void USBDeviceGetString(USBControlTransfer_t *usbControlTransferP)
{
	USBStandardStringDescriptor_t *usbStandardStringDescriptorP = (USBStandardStringDescriptor_t *)usbControlTransferP->usbDataBufferInP;
	USBStandardRequest_t *usbStandardRequestP = (USBStandardRequest_t *)usbControlTransferP->usbRequestP;
	uint8_t stringIndex = (usbStandardRequestP->value & 0x00FF);
	
	/**
		The host has requested the default string index 0, this includes a list of supported languages. We only
		support American English.
	 */
	if (stringIndex == 0)
	{
		uint16_t *langId = (uint16_t *)&usbStandardStringDescriptorP->rawBlock;
		usbStandardStringDescriptorP->length = sizeof(uint16_t) + 2;
		usbStandardStringDescriptorP->descriptorType = USB_STANDARD_DESCRIPTOR_TYPE_DEVICE_STRING;
		*(langId) = 0x0409;
	}
	else
	{
		UTF8String_t utfString;
		
		stringIndex--;
		utfString = USB_DEVICE_STRINGS[stringIndex];
		usbStandardStringDescriptorP->length = utfString.lengthInBytes + 2;
		usbStandardStringDescriptorP->descriptorType = USB_STANDARD_DESCRIPTOR_TYPE_DEVICE_STRING;
		memcpy(&usbStandardStringDescriptorP->rawBlock, utfString.charP, utfString.lengthInBytes);
	}
	
	usbControlTransferP->actualLength = usbStandardStringDescriptorP->length;
}

void USBDeviceGetConfiguration(USBControlTransfer_t *usbControlTransferP)
{
	/**
		We only have one active configuration (always enabled).
	 */
	*((uint8_t *)usbControlTransferP->usbDataBufferInP) = 0x01;
	usbControlTransferP->actualLength = 1;
}

void USBDeviceSetConfiguration(USBControlTransfer_t * usbControlTransferP)
{
	/**
		We only have one active configuration (always enabled), so there is nothing to do.
	 */
}

void USBDeviceGetHIDReportDescriptor(USBControlTransfer_t *usbControlTransferP)
{
	uint8_t reportDescriptor[23] = {
		0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
		0x09, 0x01,                    // USAGE (Vendor Usage 1)
		0xa1, 0x01,                    // COLLECTION (Application)
		0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
		0x75, 0x08,                    //   REPORT_SIZE (8)
		0x85, 0x01,                    //   REPORT_ID (1) (USB)
		0x95, 0x01,                    //   REPORT_COUNT (1)
		0x09, 0x01,                    //   USAGE (Vendor Usage 1)
		0x81, 0x02,                    //   INPUT (Data,Var,Abs)
		0xc0                           // END_COLLECTION
	};
	
	memcpy(usbControlTransferP->usbDataBufferInP, (ptr_t)reportDescriptor, 23);
}