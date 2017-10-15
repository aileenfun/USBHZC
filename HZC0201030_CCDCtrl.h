//#pragma once
#ifndef __PR_HZC0201030CAM_H_
#define __PR_HZC0201030CAM_H_
#include "cputimer.h"
#include "prstruct.h"
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <fstream> 
#include "opencv2/opencv.hpp"
#define  MOJEMVC1000MF 29046
#define tTrace TRACE  
extern CString TestBmpFile; 
using namespace cv;
using namespace std;
#include "..\CCTAPI_USB\CCTAPI\USB_API.h"
#define HZC0201_CAM_ARRAY 10
#define HZC0201030HB 1
#define HZC0201130HB 2
#define HZCAPI "C"   __declspec(dllexport)   int   __stdcall 
typedef  struct HZC30IBuf
{
	BYTE ImageBuf[752*480];
};
typedef  struct HZC130IBuf
{
	BYTE ImageBuf[1280*1024];
};
class HZC0201030_CCDCtrl
{
public:
	HZC0201030_CCDCtrl(void);
	~HZC0201030_CCDCtrl(void);
public:
	int nIndex;
	int CCD_ID; //--- CCD 连¢?接¨®顺3序¨°号?
	int CCD_SN; //--- CCD 系¦Ì列¢D号?，ê?8位?数ºy字Á?
	int thresholdValue ;
	unsigned char SubNum;
	ULONG m_nDeviceNum;
	USHORT nDeviceType;
	double pixelRate;
   	//BYTE SrhImgBuf1[CCD_W*CCD_H];  //color buffer,to display
	unsigned char  Image_index;
	BYTE mRGBBuf[752*480];
	BYTE mRGBBuf2[752*480];;
	BYTE mRGBBuf1[1280*1024];//CCD_W*CCD_H*3];
	BYTE mRGBBuf3[1280*1024];//CCD_W*CCD_H*3];
	CCpuTimer framtm;
	int Card_debug ;
	int hardware_CamID;
	int hardware_BmpFileNo;
	int show_channel;               //通道
	int m_iDevIndex;				//设备序号
	int board;
	bool initFlag;
	//bool UsbInitFlag;
	//enum TRIGMODE{TgMd_Auto,TgMd_FPGA,TgMd_Soft,TgMd_OutSig};
	int HzcMirror;
	CCCTAPIAppUSB *h_cctapi;
	HZC30IBuf  HZC0201030ImageBuf[HZC0201_CAM_ARRAY];
	HZC130IBuf  HZC0201130ImageBuf[HZC0201_CAM_ARRAY];
	int ImageOK[HZC0201_CAM_ARRAY];
private:
	//反回二次化图像到上位
	int SecondaryChangeImage(int w,int h,BYTE* InBuf,int Threshold_, BYTE* OutBuf);
public:
	int Init_HZC0201030();
	int InitCCDSN();//--- 仅用于初始化相机并获得系列号
	int ExitCCDSN();
	int Exit();
	//mode=1为硬触发，mode=0 free
	void HZC0106030F_trigMode(int TgMd);
	int setExpoGainValue(unsigned short Gain, unsigned short Expo);
	int Usb2SetMirrorMode(int nMode );
	void GetMbs(int *iparm);
	int ReadDeviceID(unsigned char* iparam);
	int WriteDeviceID(unsigned char* iparam);
	int ReadDeviceSN(unsigned char* iparam);
	int WriteDeviceSN(unsigned char* iparam);
	int SoftTrig();
	int GetCCDReady(int camid);
	int HZC0201130_GetImage(BYTE* buf,int ImageBufID,int Secondary); //copy image from the CCD buffer
    int HZC0201030_GetImage(BYTE* buf,int ImageBufID,int Secondary) ;
	int OpenUsb(int index);
	int UsbInit(int index);
};
//extern HZC0201030_CCDCtrl  HZC0201030B[9];
#endif 
