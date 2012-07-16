#include "quadroconsole.h"

using namespace std;

void QuadroCopterDisplayHeader(void);
HANDLE USBOpenQuadroCopterDeviceHandle(void);
void USBCloseQuadroCopterDeviceHandle(HANDLE qcDevHandleP);

int main(int argc, char **argv)
{	
	int pattern = 0;

	for ( ; ; )
	{
		system("cls");
		QuadroCopterDisplayHeader();
		cout << "Enter LED pattern (range 0 - 3): ";
		cin >> pattern;

		if ((pattern > -1) && (pattern < 4))
		{
			break;
		}
	}

	HANDLE qcDevHandleP = USBOpenQuadroCopterDeviceHandle();

	if (! qcDevHandleP)
	{
		return 1;
	}

	byte reportBuffer1[2] = {0x01, pattern};
	if (! HidD_SetFeature(qcDevHandleP, reportBuffer1, 2))
	{
		cout << "Error sending report to device" << endl;
        DWORD d = GetLastError();
        cout << d << endl;
	}

	byte reportBuffer2[2] = {0x02, 0x00};
	if (! HidD_GetFeature(qcDevHandleP, reportBuffer2, 2))
	{
		cout << "Error sending report to device" << endl;
        DWORD d = GetLastError();
        cout << d << endl;
	}

	USBCloseQuadroCopterDeviceHandle(qcDevHandleP);

	return 0;
}

void QuadroCopterDisplayHeader(void)
{
	cout << endl << endl;
	cout << "+------------------------------------------------+" << endl;
	cout << "+ > QuadroCore < : QuadroConsole                 +" << endl;
	cout << "+------------------------------------------------+" << endl;
	cout << "+------------------------------------------------+" << endl;
	cout << endl << endl;
}

HANDLE USBOpenQuadroCopterDeviceHandle(void)
{
	GUID hidGUID;
	HDEVINFO devInfoP = 0;
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	DWORD dwIndex = 0;
	SP_DEVICE_INTERFACE_DATA devData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA devDetailData = NULL;
	devData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	DWORD devDetailDataSize;
	DWORD ReqSize;

	HidD_GetHidGuid(&hidGUID);
	devInfoP = SetupDiGetClassDevs(&hidGUID, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
	
	if (devInfoP == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	while(SetupDiEnumDeviceInterfaces(devInfoP, NULL, &hidGUID, dwIndex, &devData))
	{

		dwIndex++;

		SetupDiGetDeviceInterfaceDetail(devInfoP, &devData, NULL, 0, &devDetailDataSize, NULL);
		devDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(devDetailDataSize);
		devDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		if (! SetupDiGetDeviceInterfaceDetail (devInfoP, &devData, devDetailData, devDetailDataSize, &ReqSize, NULL))
		{
			SetupDiDestroyDeviceInfoList(devInfoP);
			free(devDetailData);
			return NULL;
		}

		if (NULL != wcsstr(devDetailData->DevicePath, _T("vid_04b3&pid_0001")))
		{
			hDevice = CreateFile(devDetailData->DevicePath,
						GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

			SetupDiDestroyDeviceInfoList(devInfoP);
			free(devDetailData);
			break;
		}

		free(devDetailData);
	}

	return hDevice;
}

void USBCloseQuadroCopterDeviceHandle(HANDLE qcDevHandleP)
{
	if (qcDevHandleP)
	{
		CloseHandle(qcDevHandleP);
	}
}