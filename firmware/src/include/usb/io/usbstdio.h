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

#ifndef USBSTDREQ_H_
#define USBSTDREQ_H_

#include "usb/io/usbio.h"

#define USB_REQUEST_TYPE_FLD_TYPE_bm 0x60
#define USB_REQUEST_TYPE_FLD_TYPE_STANDARD_bm (0 << 6)
#define USB_REQUEST_TYPE_FLD_TYPE_CLASS_bm (1 << 6)
#define USB_REQUEST_TYPE_FLD_TYPE_VENDOR_bm (2 << 5)
#define USB_REQUEST_TYPE_FLD_TYPE_RESERVED_bm (3 << 5)
#define USB_REQUEST_TYPE_FLD_RECIPIENT_bm 0x1F
#define USB_REQUEST_TYPE_FLD_RECIPIENT_DEVICE_bm (0 << 4)
#define USB_REQUEST_TYPE_FLD_RECIPIENT_INTERFACE_bm (1 << 4)
#define USB_REQUEST_TYPE_FLD_RECIPIENT_ENDPOINT_bm (2 << 3)
#define USB_REQUEST_TYPE_FLD_RECIPIENT_OTHER_bm (3 << 3)

#define USB_REQUEST_TYPE_STANDARD 0
#define USB_REQUEST_TYPE_CLASS 1

#define USB_REQUEST_TYPE_RECIPIENT_DEVICE 0
#define USB_REQUEST_TYPE_RECIPIENT_INTERFACE 1
#define USB_REQUEST_TYPE_RECIPIENT_ENDPOINT 2

#define USB_REQUEST_DEVICE_GET_STATUS 0x00
#define USB_REQUEST_DEVICE_CLEAR_FEATURE 0x01
#define USB_REQUEST_DEVICE_SET_FEATURE 0x03
#define USB_REQUEST_DEVICE_GET_DESCRIPTOR 0x06
#define USB_REQUEST_DEVICE_SET_ADDRESS 0x05
#define	USB_REQUEST_DEVICE_GET_CONFIGURATION 0x08
#define USB_REQUEST_DEVICE_SET_CONFIGURATION 0x09
#define USB_REQUEST_INTERFACE_GET_DESCRIPTOR 0x06
#define USB_REQUEST_CLASS_GET_HID_REPORT 0x01
#define USB_REQUEST_CLASS_SET_HID_REPORT 0x09
#define USB_REQUEST_DEVICE_SYNC_FRAME 0x12

#define USB_REQUEST_TYPE_HANDLER_COUNT 18

typedef enum _USBStandardDescriptorType
{
	USB_STANDARD_DESCRIPTOR_TYPE_DEVICE = 0x01,
	USB_STANDARD_DESCRIPTOR_TYPE_DEVICE_CONFIGURATION = 0x02,
	USB_STANDARD_DESCRIPTOR_TYPE_DEVICE_STRING = 0x03,
	USB_STANDARD_DESCRIPTOR_TYPE_INTERFACE_CONFIGURATION = 0x04,
	USB_STANDARD_DESCRIPTOR_TYPE_ENDPOINT = 0x05
} USBStandardDescriptorType_t;

typedef enum _USBClassDescriptorType
{
	USB_CLASS_DESCRIPTOR_TYPE_HID = 0x21,
	USB_CLASS_DESCRIPTOR_TYPE_REPORT = 0x22
} USBDescriptorType_t;

typedef struct _USBStandardRequest
{
	uint8_t requestType;
	uint8_t request;
	uint16_t value;
	uint16_t index;
	uint16_t length;
} USBStandardRequest_t;

typedef struct _USBStandardStringDescriptor
{
	uint8_t length;
	uint8_t descriptorType;
	uint8_t rawBlock;
} USBStandardStringDescriptor_t;

typedef void (*USB_STANDARD_REQUEST_HANDLER_FUNC)(USBControlTransfer_t *usbControlTransferP);

typedef struct _USBStandardRequestHandler
{
	uint8_t type;
	uint8_t recipient;
	uint8_t id;
	uint8_t valueLowByte;
	uint8_t valueHighByte;
	bool_t compareValueHighByte;
	bool_t compareValueLowByte;
	USB_STANDARD_REQUEST_HANDLER_FUNC handlerFuncP;
} USBStandardRequestHandler_t;

typedef struct _USBStandardDeviceDescriptor
{
	uint8_t length;
	uint8_t descriptorType;
	uint16_t usbVersion;
	uint8_t deviceClass;
	uint8_t deviceSubClass;
	uint8_t deviceProtocol;
	uint8_t maxPacketSize;
	uint16_t vendorId;
	uint16_t productId;
	uint16_t deviceVersion;
	uint8_t manufacturerIndex;
	uint8_t productIndex;
	uint8_t serialNumberIndex;
	uint8_t numberOfConfigurations;
} USBStandardDeviceDescriptor_t;

typedef struct _USBStandardDeviceConfigurationDescriptor
{
	uint8_t length;
	uint8_t descriptorType;
	uint16_t totalLength;
	uint8_t numInterfaces;
	uint8_t configurationValue;
	uint8_t configurationIndex;
	uint8_t attributes;
	uint8_t maxPower;
} USBStandardDeviceConfigurationDescriptor_t;

typedef struct _USBStandardInterfaceConfigurationDescriptor
{
	uint8_t length;
	uint8_t descriptorType;
	uint8_t interfaceNumber;
	uint8_t alternateSetting;
	uint8_t numberOfEndpoints;
	uint8_t interfaceClass;
	uint8_t interfaceSubClass;
	uint8_t interfaceProtocol;
	uint8_t interfaceIndex;
} USBStandardInterfaceConfigurationDescriptor_t;

typedef struct _USBStandardHIDConfigurationDescriptor
{
	uint8_t length;
	uint8_t descriptorType;
	uint16_t version;
	uint8_t countryCode;
	uint8_t numberOfDescriptors;
	uint8_t nestedDescriptorType;
	uint16_t nestedDesciptorLength;
} USBStandardHIDConfigurationDescriptor_t;

typedef struct _USBStandardEndpointConfigurationDescriptor
{
	uint8_t length;
	uint8_t descriptorType;
	uint8_t endpointAddress;
	uint8_t attributes;
	uint16_t maxPacketSize;
	uint8_t interval;
} USBStandardEndpointConfigurationDescriptor_t;

void USBDeviceParseStandardRequestMetaData(USBControlTransfer_t *usbControlTransferP);
bool_t USBDeviceProcessStandardRequest(USBControlTransfer_t *usbControlTransferP);
void USBDeviceReset(void);
void USBDeviceGetStatus(USBControlTransfer_t *usbControlTransferP);
void USBDeviceClearFeature(USBControlTransfer_t *usbControlTransferP);
void USBDeviceSetFeature(USBControlTransfer_t *usbControlTransferP);
void USBDeviceGetDescriptor(USBControlTransfer_t *usbControlTransferP);
void USBDeviceGetConfigurationDescriptor(USBControlTransfer_t *usbControlTransferP);
void USBDeviceSetAddress(uint8_t address);
void USBDeviceSetDeferredAddress(USBControlTransfer_t *usbControlTransferP);
void USBDeviceGetString(USBControlTransfer_t *usbControlTransferP);
void USBDeviceGetConfiguration(USBControlTransfer_t *usbControlTransferP);
void USBDeviceSetConfiguration(USBControlTransfer_t * usbControlTransferP);
void USBDeviceInterfaceGetStatus(USBControlTransfer_t *usbControlTransferP);
void USBDeviceInterfaceClearFeature(USBControlTransfer_t *usbControlTransferP);
void USBDeviceInterfaceSetFeature(USBControlTransfer_t *usbControlTransferP);
void USBDeviceGetHIDReportDescriptor(USBControlTransfer_t *usbControlTransferP);
void USBDeviceEndpointGetStatus(USBControlTransfer_t *usbControlTransferP);
void USBDeviceEndpointClearFeature(USBControlTransfer_t *usbControlTransferP);
void USBDeviceEndpointSetFeature(USBControlTransfer_t *usbControlTransferP);
void USBDeviceEndpointSyncFrame(USBControlTransfer_t *usbControlTransferP);

#endif /* USBSTDREQ_H_ */