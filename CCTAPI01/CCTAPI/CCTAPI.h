// CCTAPI.h : main header file for the CCTAPI DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef COMPILE_API
	#define CCT_API __declspec(dllexport) 
#else 
	#define CCT_API __declspec(dllimport)
#endif

#include "resource.h"		// main symbols
#include "DataCapture.h"
#include "CyAPI.h"
typedef enum TRIGMODE{TgMd_Auto,TgMd_FPGA,TgMd_Soft,TgMd_OutSig};
typedef struct USB_ORDER_S
{
	UCHAR			Target;		//0:TGT_DEVICE;1:TGT_INIFC;2:TGT_ENDPT;3:TGT_OTHER
	UCHAR			ReqType;	//0:REQ_STD;1:REQ_CLASS;2:REQ_VENDOR
	UCHAR			Direction;	//0:DIR_TO_DEVICE;1:DIR_FROM_DEVICE
	UCHAR           ReqCode;
	WORD            Value;
	WORD			Index;

	BYTE*			pData;
	WORD			DataBytes;
	USB_ORDER_S()
	{
		Target=0;
		ReqType=2;
		Direction=0;
		ReqCode=0;
		Value=0;
		Index=0;
		pData=NULL;
		DataBytes=0;
	}
}USB_ORDER,*PUSB_ORDER;
class  CCCTAPIAppUSB
{
public:
	CCT_API CCCTAPIAppUSB(HANDLE h=NULL);
	CCT_API int OpenUsb(int devNum=0);
	CCT_API int CloseUsb();
	CCT_API int startCap(int height,int width,LPMV_CALLBACK2 CallBackFunc,int camnum=0);
	CCT_API int stopCap();
	CCT_API unsigned char WrEEPROM(WORD iAddr,WORD iValue);
	CCT_API void WrSensorReg(unsigned short, unsigned short);
	CCT_API unsigned short RdSensorReg(unsigned short);
	 void WrFpgaReg(unsigned char iAddr, unsigned char iValue);
	CCT_API unsigned char RdEEPROM(WORD iAddr,unsigned char * buffer,int &len);
	 unsigned char RdFpgaReg(unsigned char);
	CCT_API void InitSensor(void);
	CCT_API void GetFpsMbs(int &FCount,long &lBytePerSecond );
	CCT_API void setFpgaFreq(unsigned char fpgafreq);
	CCT_API void setGainValue(unsigned short GainValue);
	CCT_API void setAutoGainExpo(bool isAutoGain, bool isAutoExpo);
	CCT_API void setExpoValue(unsigned short ExpoValue);
	CCT_API void setResolution(unsigned char resNo);
	CCT_API void setNormal(void);
	CCT_API void setXmirror(void);
	CCT_API void setYmirror(void);
	CCT_API void setXYmirror(void);
	CCT_API void SendUsbSpeed2Fpga(unsigned char speedType);
	CCT_API int RdDeviceID(unsigned char* buff,int& len);
	CCT_API int WrDeviceID(unsigned char* buff,int& len);
	CCT_API int RdDeviceSN(unsigned char* buff,int& len);
	CCT_API int WrDeviceSN(unsigned char* buff,int& len);
	CCT_API void SetTrigMode(TRIGMODE m);
	int usbOrderWrapper(int code,int dir,int index,int value,unsigned char *buffer, int &len);
	CCT_API int softTrigOnce();
	 void destory();
public:
	int WriteData(char*pbuff,LONG &lBytes);
	int SendOrder(PUSB_ORDER pOrder );
	int Reset();
	int DeviceReset();
	bool b_opened;
private:
	char*         m_pReadBuff;
	bool m_bUsbOpen;
	CDataProcess* m_pDataProcess;
	CDataCapture*  m_pDataCapture;

	bool b_closed;
	CCyUSBDevice *m_Usb;
	CCyUSBEndPoint *Endpt;
};


	