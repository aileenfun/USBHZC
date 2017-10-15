#include "stdafx.h"
#include "HZC0201030_CCDCtrl.h"
#include "MSZPRTemp.h"
#define HZCAPI "C"   __declspec(dllexport)   int   __stdcall 
extern int  C1280_W;
extern int  C1024_H ;
extern int  C752_W;
extern int  C752_W ;
extern int  C640_W;
extern int  C480_H ;
extern HZCAPI PRSys_HZC0201030_CCDCtrl(int ccdno, int mode, int ImageBufID, int Secondary, int* pidata);
extern MSZPRTemp MSZTemp[10];
void CALLBACK HZC0201030_RawCallBack1(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201030_RawCallBack2(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201030_RawCallBack3(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201030_RawCallBack4(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201030_RawCallBack5(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201030_RawCallBack6(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201030_RawCallBack7(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201030_RawCallBack8(LPVOID lpParam,LPVOID lpUser);
void _stdcall HZC0201030_RawCallBack9(LPVOID lpParam,LPVOID lpUser);
void _stdcall HZC0201030_RawCallBack10(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201130_RawCallBack1(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201130_RawCallBack2(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201130_RawCallBack3(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201130_RawCallBack4(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201130_RawCallBack5(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201130_RawCallBack6(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201130_RawCallBack7(LPVOID lpParam,LPVOID lpUser);
void CALLBACK HZC0201130_RawCallBack8(LPVOID lpParam,LPVOID lpUser);
void _stdcall HZC0201130_RawCallBack9(LPVOID lpParam,LPVOID lpUser);
void _stdcall HZC0201130_RawCallBack10(LPVOID lpParam,LPVOID lpUser);
HZC0201030_CCDCtrl::HZC0201030_CCDCtrl(void)
{
	CCD_ID=0;
	CCD_SN=0;
	nIndex=0;
	Image_index = 0;
	m_nDeviceNum=0;
	nDeviceType=0;
	for(int i=0; i<HZC0201_CAM_ARRAY;i++)
	{
		ImageOK[i] = 0;
	}
	thresholdValue = 0;
	//UsbInitFlag = false;
	
}
HZC0201030_CCDCtrl::~HZC0201030_CCDCtrl(void)
{
}
int HZC0201030_CCDCtrl::OpenUsb(int CCD_ID)
{
	if (h_cctapi == NULL)
	h_cctapi = new CCCTAPIAppUSB(this);
	nIndex = CCD_ID ;
	if (h_cctapi->b_opened == true) { return -1; }
	if (h_cctapi->OpenUsb(nIndex)<0)
	{
		return 0;
	}
	return 1;
}
int HZC0201030_CCDCtrl::UsbInit(int CCD_ID)//bugfix1
{
	//int len = 4;
	
	if (h_cctapi == NULL)
	h_cctapi = new CCCTAPIAppUSB(this);
	
	nIndex = CCD_ID;
	if (h_cctapi->b_opened == true) { h_cctapi->CloseUsb(); }
	if (h_cctapi->OpenUsb(nIndex)<0)
	{
		//UsbInitFlag = true;
		return 0;
	}
	h_cctapi->SendUsbSpeed2Fpga(1);
	unsigned char num0[4];
	ReadDeviceID(num0);
	int temp0 = num0[0] << 8 * 0;
	temp0 += num0[1] << 8 * 1;
	temp0 += num0[2] << 8 * 2;
	temp0 += num0[3] << 8 * 3;
	SubNum = temp0;
	if ((int)SubNum == HZC0201030HB)
	{
		switch (nIndex)
		{
		case 0:
			if (h_cctapi->startCap(C480_H, C752_W, HZC0201030_RawCallBack1, 0)<0)
			{
				return 31;
			}
			break;
		case 1:
			if (h_cctapi->startCap(C480_H, C752_W, HZC0201030_RawCallBack2, 0)<0)
			{
				return 32;
			}
			break;
		case 2:
			if (h_cctapi->startCap(C480_H, C752_W, HZC0201030_RawCallBack3, 0)<0)
			{
				return 33;
			}
			break;
		case 3:
			if (h_cctapi->startCap(C480_H, C752_W, HZC0201030_RawCallBack4, 0)<0)
			{
				return 34;
			}
			break;
		case 4:
			if (h_cctapi->startCap(C480_H, C752_W, HZC0201030_RawCallBack5, 0)<0)
			{
				return 35;
			}
			break;
		case 5:
			if (h_cctapi->startCap(C480_H, C752_W, HZC0201030_RawCallBack6, 0)<0)
			{
				return 36;
			}
			break;
		case 6:
			if (h_cctapi->startCap(C480_H, C752_W, HZC0201030_RawCallBack7, 0)<0)
			{
				return 37;
			}
			break;
		case 7:
			if (h_cctapi->startCap(C480_H, C752_W, HZC0201030_RawCallBack8, 0)<0)
			{
				return 38;
			}
			break;
		}
	
	}
	if (SubNum == HZC0201130HB)
	{
		switch (nIndex)
		{
		case 0:
			if (h_cctapi->startCap(C1024_H, C1280_W, HZC0201130_RawCallBack1, 0)<0)
			{
				return 131;
			}
			break;
		case 1:
			if (h_cctapi->startCap(C1024_H, C1280_W, HZC0201130_RawCallBack2, 0)<0)
			{
				return 132;
			}
			break;
		case 2:
			if (h_cctapi->startCap(C1024_H, C1280_W, HZC0201130_RawCallBack3, 0)<0)
			{
				return 133;
			}
			break;
		case 3:
			if (h_cctapi->startCap(C1024_H, C1280_W, HZC0201130_RawCallBack4, 0)<0)
			{
				return 134;
			}
			break;
		case 4:
			if (h_cctapi->startCap(C1024_H, C1280_W, HZC0201130_RawCallBack5, 0)<0)
			{
				return 135;
			}
			break;
		case 5:
			if (h_cctapi->startCap(C1024_H, C1280_W, HZC0201130_RawCallBack6, 0)<0)
			{
				return 136;
			}
			break;
		case 6:
			if (h_cctapi->startCap(C1024_H, C1280_W, HZC0201130_RawCallBack7, 0)<0)
			{
				return 137;
			}
			break;
		case 7:
			if (h_cctapi->startCap(C1024_H, C1280_W, HZC0201130_RawCallBack8, 0)<0)
			{
				return 138;
			}
			break;
		}
	}
	h_cctapi->SetTrigMode(TgMd_Soft);
	h_cctapi->setAutoGainExpo(false, false);
	h_cctapi->setExpoValue(30);
	h_cctapi->setGainValue(30);
	h_cctapi->setResolution(0);
	return 1;
}
int HZC0201030_CCDCtrl::Init_HZC0201030()
{
	//int len=4;
	
	if (h_cctapi == NULL)
	h_cctapi=new CCCTAPIAppUSB(this);
	nIndex = CCD_ID;
	if (h_cctapi->b_opened == true) { return -1; }
	if (h_cctapi->OpenUsb(nIndex)<0)
	{
		return 0;
	}
	h_cctapi->SendUsbSpeed2Fpga(1);
	unsigned char num0[4];
	ReadDeviceID(num0);
	int temp0 = num0[0] << 8 * 0;
	temp0 += num0[1] << 8 * 1;
	temp0 += num0[2] << 8 * 2;
	temp0 += num0[3] << 8 * 3;
	SubNum = temp0;
	if((int)SubNum == HZC0201030HB)
	{
		switch(nIndex)
		{
		case 0:
			if(h_cctapi->startCap(C480_H,C752_W,HZC0201030_RawCallBack1,0)<0)
			{
			  return 31;
			}
			break;
		case 1:
			if(h_cctapi->startCap(C480_H,C752_W,HZC0201030_RawCallBack2,0)<0)
			{
			  return 32;
			}
			break;
		case 2:
			if(h_cctapi->startCap(C480_H,C752_W,HZC0201030_RawCallBack3,0)<0)
			{
			  return 33;
			}
			break;
		case 3:
			if(h_cctapi->startCap(C480_H,C752_W,HZC0201030_RawCallBack4,0)<0)
			{
			  return 34;
			}
			break;
		case 4:
			if(h_cctapi->startCap(C480_H,C752_W,HZC0201030_RawCallBack5,0)<0)
			{
			  return 35;
			}
			break;
		case 5:
			if(h_cctapi->startCap(C480_H,C752_W,HZC0201030_RawCallBack6,0)<0)
			{
			  return 36;
			}
			break;
		case 6:
			if(h_cctapi->startCap(C480_H,C752_W,HZC0201030_RawCallBack7,0)<0)
			{
			  return 37;
			}
			break;
		case 7:
			if(h_cctapi->startCap(C480_H,C752_W,HZC0201030_RawCallBack8,0)<0)
			{
			  return 38;
			}
			break;
		case 8:
			if (h_cctapi->startCap(C480_H, C752_W, HZC0201030_RawCallBack9, 0)<0)
			{
				return 37;
			}
			break;
		case 9:
			if (h_cctapi->startCap(C480_H, C752_W, HZC0201030_RawCallBack10, 0)<0)
			{
				return 38;
			}
			break;
		}
	}
	if(SubNum == HZC0201130HB)
	{
		switch(nIndex)
		{
		case 0:
			if(h_cctapi->startCap(C1024_H,C1280_W, HZC0201130_RawCallBack1,0)<0)
			{
			  return 131;
			}
			break;
		case 1:
			if(h_cctapi->startCap(C1024_H,C1280_W, HZC0201130_RawCallBack2,0)<0)
			{
			  return 132;
			}
			break;
		case 2:
			if(h_cctapi->startCap(C1024_H,C1280_W,HZC0201130_RawCallBack3,0)<0)
			{
			  return 133;
			}
			break;
		case 3:
			if(h_cctapi->startCap(C1024_H,C1280_W,HZC0201130_RawCallBack4,0)<0)
			{
			  return 134;
			}
			break;
		case 4:
			if(h_cctapi->startCap(C1024_H,C1280_W,HZC0201130_RawCallBack5,0)<0)
			{
			  return 135;
			}
			break;
		case 5:
			if(h_cctapi->startCap(C1024_H,C1280_W,HZC0201130_RawCallBack6,0)<0)
			{
			  return 136;
			}
			break;
		case 6:
			if(h_cctapi->startCap(C1024_H,C1280_W,HZC0201130_RawCallBack7,0)<0)
			{
			  return 137;
			}
			break;
		case 7:
			if(h_cctapi->startCap(C1024_H,C1280_W,HZC0201130_RawCallBack8,0)<0)
			{
			  return 138;
			}
			break;
		case 8:
			if (h_cctapi->startCap(C1024_H, C1280_W, HZC0201130_RawCallBack9, 0)<0)
			{
				return 137;
			}
			break;
		case 9:
			if (h_cctapi->startCap(C1024_H, C1280_W, HZC0201130_RawCallBack10, 0)<0)
			{
				return 138;
			}
			break;
		}
	}
	unsigned char snnum[4];
	ReadDeviceSN(snnum);
	int temp = snnum[0] << 8 * 0;
	temp += snnum[1] << 8 * 1;
	temp += snnum[2] << 8 * 2;
	temp += snnum[3] << 8 * 3;
	CCD_SN = temp;
	h_cctapi->SetTrigMode(TgMd_Soft);
	h_cctapi->setAutoGainExpo(false,false);
	h_cctapi->setExpoValue(30);
	h_cctapi->setGainValue(30);
	h_cctapi->setResolution(0);
	return 1;
}
int HZC0201030_CCDCtrl::InitCCDSN()
{
	int len=4;
	if (h_cctapi == NULL)
	h_cctapi=new CCCTAPIAppUSB(this);
	if (h_cctapi->b_opened == true) { return -1; }
	nIndex = CCD_ID;
	if (h_cctapi->OpenUsb(nIndex)<0)
	{
		return 1;
	}
	unsigned char snnum;
	ReadDeviceSN(&snnum);
	CCD_SN = (int)snnum;
	return 1;
}
int HZC0201030_CCDCtrl::ExitCCDSN()
{
	if (!h_cctapi)
	{
		return(0);
	}
	h_cctapi=NULL;
	//UsbInitFlag = false;
	return 1;
}
int HZC0201030_CCDCtrl::Exit()
{
	if (!h_cctapi)
	{
		return(0);
	}
	//UsbInitFlag = false;
	h_cctapi->CloseUsb();
	return 1;
}
int HZC0201030_CCDCtrl::setExpoGainValue(unsigned short Gain,unsigned short Expo)
{
	if (!h_cctapi)
	{
		return(0);
	}
	h_cctapi->setExpoValue(Expo);
	h_cctapi->setGainValue(Gain);
	return 1;
}
int HZC0201030_CCDCtrl::ReadDeviceID(unsigned char* buff)
{
	int length=4;
	byte readbuff[4];
	if (!h_cctapi)
	{
		return(0);
	}
	h_cctapi->RdDeviceID(readbuff,length);
	//readbuff[4] = length;
	memcpy(buff,readbuff, length);
	//length=len;
	return 1;
}
int HZC0201030_CCDCtrl::WriteDeviceID(unsigned char* iparam)
{
	int len = 4;
	unsigned char buff[4];
	buff[0] = (unsigned char)*(iparam + 0);
	buff[1] = (unsigned char)*(iparam + 1);
	buff[2] = (unsigned char)*(iparam + 2);
	buff[3] = (unsigned char)*(iparam + 3);

	if (!h_cctapi)
	{
		return(0);
	}
	h_cctapi->WrDeviceID(buff, len);

	return 1;
}
int HZC0201030_CCDCtrl::ReadDeviceSN(unsigned char* buff)
{
	int length =4;
	byte readbuff[4];
	if (!h_cctapi)
	{
		return(0);
	}
	h_cctapi->RdDeviceSN(readbuff, length);
	//readbuff[4] = length;
	memcpy(buff,readbuff, length);

	return 1;
}
int HZC0201030_CCDCtrl::WriteDeviceSN(unsigned char* iparam)
{
	int len = 4;
	if (!h_cctapi)
	{
		return(0);
	}
	unsigned char buff[4];
	buff[0] = (unsigned char)*(iparam + 0);
	buff[1] = (unsigned char)*(iparam + 1);
	buff[2] = (unsigned char)*(iparam + 2);
	buff[3] = (unsigned char)*(iparam + 3);

	h_cctapi->WrDeviceSN(buff,len);

	return len;
	return 1;

	//// TODO: Add your control notification handler code here
	//if (!m_bUsbOpen)
	//{
	//	SetDlgItemText(IDC_STATIC_TEXT, L"USB未打开。");
	//	return;
	//}
	//CString strValue;
	//GetDlgItemText(IDC_EDIT_SN, strValue);
	//int temp = str2hex(strValue);
	//unsigned char iValue[4] = { 0 };
	//iValue[0] = (temp >> 8 * 0) & 0xff;
	//iValue[1] = (temp >> 8 * 1) & 0xff;
	//iValue[2] = (temp >> 8 * 2) & 0xff;
	//iValue[3] = (temp >> 8 * 3) & 0xff;
	//int len = 4;
	//h_cctapi->WrDeviceSN(iValue, len);
	//SetDlgItemText(IDC_STATIC_TEXT, L"设置SN成功。");
}
int HZC0201030_CCDCtrl::Usb2SetMirrorMode(int nMode )
{
	if (!h_cctapi)
	{
		return(0);
	}
	switch(nMode)
	{
	case 0:
	    h_cctapi->setNormal();
		break;
	case 1:
		h_cctapi->setXmirror();
		break;
	case 2:
		h_cctapi->setYmirror();
		break;
	}
    return 1;
}
//mode=1为硬触发，mode=0 free
void HZC0201030_CCDCtrl::HZC0106030F_trigMode(int TgMd)
{//enum TRIGMODE{TgMd_Auto,TgMd_FPGA,TgMd_Soft,TgMd_OutSig};
	if (!h_cctapi)
	{
		return;
	}
	switch(TgMd)
	{
	case 0:
	    h_cctapi->SetTrigMode(TgMd_Soft);
		break;
	case 1:
		h_cctapi->SetTrigMode(TgMd_OutSig);
		break;
	case 2:
		h_cctapi->SetTrigMode(TgMd_FPGA);
		break;
	case 3:
		h_cctapi->SetTrigMode(TgMd_Auto);
		break;
	}
}
void HZC0201030_CCDCtrl::GetMbs(int *iparm)
{
	int iFrame=0;
	long lBytePerSecond = 0;
	if (!h_cctapi)
	{
		return;
	}
	h_cctapi->GetFpsMbs(iFrame,lBytePerSecond );
	int* fidata= (int*) iparm;
	*(fidata+0) = iFrame;
	*(fidata+0) = lBytePerSecond;
}
int HZC0201030_CCDCtrl::SoftTrig()
{

	if (!h_cctapi)
	{
		return(0);
	}
	h_cctapi->softTrigOnce();
	return 1;
}
int HZC0201030_CCDCtrl::GetCCDReady(int camid)
{
	return(ImageOK[camid]);
}
int HZC0201030_CCDCtrl::HZC0201130_GetImage(BYTE* buf,int ImageBufID,int Secondary) //copy image from the CCD buffer
{
	if (!h_cctapi)
	{
		int i=0;
		//图片测试代码
		CString str;
		str.Format("bmp\\image%d.bmp",CCD_ID);

		if (TestBmpFile.GetLength() > 10)
		{
			str = TestBmpFile;
		}
		else if (TestBmpFile.GetLength() == 2)
		{
			memcpy(buf,mRGBBuf3,C1280_W*C1024_H);
			return 0;
		}

		CFile  bmpfile;
		if (!bmpfile.Open(str,CFile::modeRead   |   CFile::shareDenyWrite,NULL))
		{
			return(0);
		}

		i=bmpfile.GetLength();
		if (i == C1280_W*C1024_H+1078)
		{
			i=bmpfile.Seek(1078,CFile::begin); //跳过BMP文件头
			i=bmpfile.Read(mRGBBuf3,C1280_W*C1024_H);   //读取文件，一定要CCD_W*CCD_H
			if(0 == Secondary)SecondaryChangeImage(C1280_W,C1024_H,mRGBBuf3,thresholdValue,mRGBBuf3);//显示二次化图片
			int x,y,ratio = 2;
			for ( y = 0; y < C1024_H / ratio; y++)
			{
				for ( x = 0; x < C1280_W / ratio; x++)
				{
					*(buf+y*(C1280_W / ratio)+x)=mRGBBuf3[(C1280_W * y * ratio) + (x * ratio)];
				}
			}
			C1280_W = 1280/2;
			C1024_H = 1024/2;
		}

		bmpfile.Close();
		return(0);
	}


	int loop = 0;
	framtm.Start();
	while (ImageOK[0] == 0)
	{
		loop++;
		if (framtm.Getms() > 35)
		{
			break;
		}
	}

	if(0 == Secondary)
	{
		memcpy(buf,HZC0201130ImageBuf[ImageBufID].ImageBuf,C1280_W*C1024_H);
		SecondaryChangeImage(C1280_W,C1024_H,buf,thresholdValue,buf);
			
	}
	else
	{
		memcpy(buf,HZC0201130ImageBuf[ImageBufID].ImageBuf,C1280_W*C1024_H);
	}
	return ImageOK[ImageBufID];
}
int HZC0201030_CCDCtrl::HZC0201030_GetImage(BYTE* buf,int ImageBufID,int Secondary) 
{
	if (!h_cctapi)
	{
		ULONGLONG i=0;
		//图片测试代码
		CString str;
		str.Format("bmp\\image%d.bmp",CCD_ID);

		if (TestBmpFile.GetLength() > 10)
		{
			str=TestBmpFile;
		}
		else if (TestBmpFile.GetLength() == 2)
		{
			memcpy(buf,mRGBBuf,C752_W*C480_H);
			return 0;
		}

		CFile   bmpfile;
		if (!bmpfile.Open(str,CFile::modeRead   |   CFile::shareDenyWrite,NULL))
		{

			return(0);
		}
		//memcpy(buf,mRGBBuf,CCD_W*CCD_H);

		i=bmpfile.GetLength();
		if (i == C752_W*C480_H+1078)
		{
			i=bmpfile.Seek(1078,CFile::begin); //跳过BMP文件头
			i=bmpfile.Read(mRGBBuf,C752_W*C480_H);   //读取文件，一定要CCD_W*CCD_H
			if(0 == Secondary)
			{
				memcpy(buf,mRGBBuf,C752_W*C480_H);//mark
				SecondaryChangeImage(C752_W,C480_H,buf,thresholdValue,buf);//显示二次化图片
			}
			else
			{
			    memcpy(buf,mRGBBuf,C752_W*C480_H);//mark
			}
		}
		//memcpy(SrhImgBuf1,buf,CCD_W*CCD_H);
		bmpfile.Close();
		return(0);
	}
	//从 CCD 获取图像
	int i=0;
	int loop = 0;
	framtm.Start();
	while (ImageOK[0] == 0)
	{
		loop++;
		if (framtm.Getms() > 35)
		{
			break;
		}
	}
	if(0 == Secondary) 
	{
		memcpy(buf,HZC0201030ImageBuf[ImageBufID].ImageBuf,C752_W*C480_H);//mark
		SecondaryChangeImage(C752_W,C480_H,buf,thresholdValue,buf);
	}
	else
	{
		memcpy(buf,HZC0201030ImageBuf[ImageBufID].ImageBuf,C752_W*C480_H);//mark
	}
	return ImageOK[0];
}
int HZC0201030_CCDCtrl::SecondaryChangeImage(int w,int h,BYTE* InBuf,int Threshold_, BYTE* OutBuf)
{
	//if(0 != Secondary)return -1;
	//从图像缓冲区读取图像
	Mat img = Mat(h ,w,CV_8UC1,InBuf);

	//判断图像是否正确读入程序
	if(img.empty())
	{
		//-1：表示程序未正常读入
		TRACE("\n ======================== >Secondary image is empty. Return  %d .",-3);
		return -2;
	}

	int threshval = Threshold_;
	//int TypeThre;

	//if (Secondary = 0)TypeThre = 0;else TypeThre = 3;

	threshold(img,img,threshval,255,CV_THRESH_BINARY);
	//threshold(img,img,threshval,255,1);
	IplImage* IplMat = &img.operator IplImage();  

	OutBuf = (BYTE *)IplMat->imageData;

	//return 1;
	return 1;
}

void CALLBACK HZC0201030_RawCallBack1(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	//if (MSZTemp[1].HZC0201030B.CCD_ID < 0)return;
	/*if (MSZTemp[1].HZC0201030B.Image_index >10)return;*/
	if (MSZTemp[0].HZC0201030B.Image_index < 10)//MSZTemp[ccdno].HZC0201030B
	{
		memcpy(MSZTemp[0].HZC0201030B.HZC0201030ImageBuf[MSZTemp[0].HZC0201030B.Image_index].ImageBuf,pDataBuffer,C752_W*C480_H);
		MSZTemp[0].HZC0201030B.ImageOK[MSZTemp[0].HZC0201030B.Image_index] = 1;
		MSZTemp[0].HZC0201030B.Image_index++;
	}
}
void CALLBACK HZC0201030_RawCallBack2(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	//HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	//if (MSZTemp[2].HZC0201030B.CCD_ID < 1)return;
	/*if (MSZTemp[2].HZC0201030B.Image_index < 0)return;*/
	if (MSZTemp[1].HZC0201030B.Image_index < 10)
	{
		memcpy(MSZTemp[1].HZC0201030B.HZC0201030ImageBuf[MSZTemp[1].HZC0201030B.Image_index].ImageBuf,pDataBuffer,C752_W*C480_H);
		MSZTemp[1].HZC0201030B.ImageOK[MSZTemp[1].HZC0201030B.Image_index] = 1;
		MSZTemp[1].HZC0201030B.Image_index++;
	}
}
void CALLBACK HZC0201030_RawCallBack3(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	//HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	//if (MSZTemp[3].HZC0201030B.CCD_ID < 1)return;
	if (MSZTemp[2].HZC0201030B.Image_index < 10)
	{
		memcpy(MSZTemp[2].HZC0201030B.HZC0201030ImageBuf[MSZTemp[2].HZC0201030B.Image_index].ImageBuf,pDataBuffer,C752_W*C480_H);
		MSZTemp[2].HZC0201030B.ImageOK[MSZTemp[2].HZC0201030B.Image_index] = 1;
		MSZTemp[2].HZC0201030B.Image_index++;
	}
}
void CALLBACK HZC0201030_RawCallBack4(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201030ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer,C752_W*C480_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201030_RawCallBack5(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201030ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer,C752_W*C480_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201030_RawCallBack6(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201030ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer,C752_W*C480_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201030_RawCallBack7(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201030ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer,C752_W*C480_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201030_RawCallBack8(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201030ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer,C752_W*C480_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201030_RawCallBack9(LPVOID lpParam, LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201030ImageBuf[ccd->Image_index].ImageBuf, pDataBuffer, C752_W*C480_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201030_RawCallBack10(LPVOID lpParam, LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201030ImageBuf[ccd->Image_index].ImageBuf, pDataBuffer, C752_W*C480_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201130_RawCallBack1(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201130ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer,C1280_W*C1024_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201130_RawCallBack2(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201130ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer,C1280_W*C1024_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201130_RawCallBack3(LPVOID lpParam,LPVOID lpUser)
{	
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201130ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer, C1280_W*C1024_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201130_RawCallBack4(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201130ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer, C1280_W*C1024_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201130_RawCallBack5(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201130ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer, C1280_W*C1024_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201130_RawCallBack6(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201130ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer, C1280_W*C1024_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201130_RawCallBack7(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201130ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer, C1280_W*C1024_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201130_RawCallBack8(LPVOID lpParam,LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201130ImageBuf[ccd->Image_index].ImageBuf,pDataBuffer, C1280_W*C1024_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201130_RawCallBack9(LPVOID lpParam, LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201130ImageBuf[ccd->Image_index].ImageBuf, pDataBuffer, C1280_W*C1024_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
void CALLBACK HZC0201130_RawCallBack10(LPVOID lpParam, LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	HZC0201030_CCDCtrl *ccd = (HZC0201030_CCDCtrl*)lpUser;
	if (ccd->Image_index < 10)
	{
		memcpy(ccd->HZC0201130ImageBuf[ccd->Image_index].ImageBuf, pDataBuffer, C1280_W*C1024_H);
		ccd->ImageOK[ccd->Image_index] = 1;
		ccd->Image_index++;
	}
}
extern HZCAPI PRSys_HZC0201030_CCDCtrl(int ccdno, int mode, int ImageBufID, int Secondary, int* pidata)
{
	int rtn = 0;

	switch (mode)
	{
	case 1: //--- 初始化所有的相机
	{
		int ccd = 0;
		int ccdbit = 1;
		if (pidata == NULL)
		{
			for (ccd = 0; ccd<10; ccd++)
			{
				MSZTemp[ccd].HZC0201030B.CCD_ID = ccd;
				MSZTemp[ccd].CCD_ID = ccd;
			}
		}
		else
		{
			//--- 按相机指定顺序初始化
			int ccdch[10];
			int ccdsn[10];
			for (ccd = 0; ccd<10; ccd++)
			{
				//--- 首先接CCD驱动程序顺序初始化所有相机并得到相机的SN号
				MSZTemp[ccd].HZC0201030B.CCD_ID = ccd;
				MSZTemp[ccd].HZC0201030B.InitCCDSN();
				MSZTemp[ccd].CCD_ID = ccd;
				Sleep(100);
			}
			for (ccd = 0; ccd<10; ccd++)
			{
				//--- 接下关闭所有的相机
				ccdch[ccd] = ccd;
				ccdsn[ccd] = MSZTemp[ccd].HZC0201030B.CCD_SN;
				MSZTemp[ccd].HZC0201030B.ExitCCDSN();
				Sleep(100);
			}
			//--- 依据SN号对应关系排列相机初始化 CH 顺序
			//--- MSZTemp[ccd].MSZ_CCDCtrl.CCD_ID =系统的 CH 编号
			//--- *(pidata+ccd-1)         = 相机索引号
			int ch = 0;
			for (ccd = 0; ccd<10; ccd++)
			{
				for (ch = 0; ch<10; ch++)
				{
					if (ccdsn[ch] == *(pidata + ccd))
					{
						MSZTemp[ccd].HZC0201030B.CCD_ID = ch;
						MSZTemp[ccd].CCD_ID=ch;
					}
				}
			}
		}
		for (ccd = 0; ccd<10; ccd++)
		{
			rtn = MSZTemp[ccd].HZC0201030B.Init_HZC0201030();
			ccdbit = (ccdbit << 1) | rtn;
			TRACE("\n ======================== > Return  %d .", rtn);
			Sleep(50);
		}

		int* fidata = (int*)pidata;

		*(fidata + 0) = ccdbit;
		//rtn=1;
	}
	break;

	case -1: //--- 关闭所有的相机
	{
		int ccd = 0;

		for (ccd = 0; ccd<10; ccd++)
		{
			rtn = MSZTemp[ccd].HZC0201030B.Exit();
		}
	}
	break;
	case 2: //--- 软触发信号
	{
		switch (*(pidata + 0))
		{
			case 0:
			{
				MSZTemp[ccdno].HZC0201030B.Image_index = 0;
				rtn = MSZTemp[ccdno].HZC0201030B.SoftTrig();
			}
			break;
			case 10:
			{
				if (MSZTemp[ccdno].HZC0201030B.Image_index > 9)MSZTemp[ccdno].HZC0201030B.Image_index = 0;
				rtn = MSZTemp[ccdno].HZC0201030B.SoftTrig();

			}
		break;
		}
	}
	break;

	case 3: //--- 判断相机是否采集图像完成
	{
		rtn = MSZTemp[ccdno].HZC0201030B.GetCCDReady(ImageBufID);
	}
	break;

	case 5: //--- 设置CCD增益及快门时间
	{
		rtn = MSZTemp[ccdno].HZC0201030B.setExpoGainValue(*(pidata + 0), *(pidata + 1));
	}
	break;

	case 8: //--- 获得当前图像到 一维数组[405x339]//405, 339
	{
		void* pv = (void*)pidata;
		rtn = MSZTemp[ccdno].HZC0201030B.HZC0201130_GetImage((BYTE*)pv, ImageBufID, Secondary);//130W元件相机
	}
	break;
	case 9: //--- 软触发信号
	{
		switch (*(pidata + 0))
		{
			case 0:
			{
				MSZTemp[ccdno].HZC0201030B.Image_index = 0;
				rtn = MSZTemp[ccdno].HZC0201030B.SoftTrig();
			}
			break;
			case 10:
			{
				if (MSZTemp[ccdno].HZC0201030B.Image_index > 9)MSZTemp[ccdno].HZC0201030B.Image_index = 0;
				rtn = MSZTemp[ccdno].HZC0201030B.SoftTrig();

			}
			break;
		}
	}
	break;
	case 10: //--- 获得当前图像到 一维数组[752x480]
	{
		void* pv = (void*)pidata;
		rtn = MSZTemp[ccdno].HZC0201030B.HZC0201030_GetImage((BYTE*)pv, ImageBufID, Secondary);
	}
	break;
	case 11: //--- 获得当前图像到 一维数组[752x480]
	{
		void* pv = (void*)pidata;//(BYTE*)pv
		rtn = MSZTemp[ccdno].HZC0201030B.HZC0201030_GetImage((BYTE*)pv, ImageBufID, Secondary);
	}
	break;
	case 12: //--- 获得当前图像到 一维数组[752x480]
	{
		MSZTemp[ccdno].HZC0201030B.Image_index = 0;
		rtn = MSZTemp[ccdno].HZC0201030B.SoftTrig();
		Sleep(50);
		MSZTemp[ccdno].GetHZC0201030_Image(0, 1);
		MSZTemp[ccdno].SaveHZC0201030CCDImage();
	}
	break;
	case 13: //--- 获得当前图像到 一维数组[752x480]
	{
		MSZTemp[ccdno].HZC0201030B.Image_index = 0;
		rtn = MSZTemp[ccdno].HZC0201030B.SoftTrig();
		Sleep(50);
		MSZTemp[ccdno].GetHZC0201130_Image(0, 1);
		MSZTemp[ccdno].SaveHZC0201130CCDImage();
		//rtn =1;
	}
	break;
	case 14: //--- 获得当前图像到 一维数组[752x480]
	{
		MSZTemp[ccdno].HZC0201030B.Image_index = 0;
		rtn = MSZTemp[ccdno].HZC0201030B.SoftTrig();
		Sleep(50);
		MSZTemp[ccdno].GetHZC0201030_Image(0, 1);
		MSZTemp[ccdno].GetMarkImage();
	}
	break;
	case 15: //--- 获得当前图像到 一维数组[752x480]
	{
		MSZTemp[ccdno].HZC0201030B.thresholdValue = *(pidata + 0);
		rtn = 1;
	}
	break;
	case 16: //--- 获得当前图像到 一维数组[752x480] Usb2SetMirrorMode(BYTE nMode )
	{
		//rtn = MSZTemp[ccdno].HZC0201030B.Image_index;
	}
	break;
	case 17:
	{
		rtn = MSZTemp[ccdno].HZC0201030B.Usb2SetMirrorMode((int)*(pidata + 0));
	}
	break;
	case 18:
	{
		rtn = MSZTemp[ccdno].HZC0201030B.OpenUsb(ccdno);
	}
	break;
	case 19:
	{
		rtn = MSZTemp[ccdno].HZC0201030B.UsbInit(ccdno);
	}
	break;
	case 20:
	{
		MSZTemp[ccdno].HZC0201030B.Image_index = *(pidata + 0);
		rtn = 1;
	}
	break;
	case 21:
	{
		int ccd;
		void* pv = (void*)pidata;
		for (ccd = 0; ccd<10; ccd++)
		{
			MSZTemp[ccd].HZC0201030B.CCD_ID = ccd ;
			MSZTemp[ccd].CCD_ID = ccd;
			rtn = MSZTemp[ccd].HZC0201030B.Init_HZC0201030();
			*(pidata + ccd) = MSZTemp[ccd].HZC0201030B.CCD_SN;
			TRACE("\n ======================== >CCD %d CCD_SN  %d .", ccd, *(pidata + ccd ));
			Sleep(50);
		}
	}
	break;
	case 22:
	{
		rtn = MSZTemp[ccdno].HZC0201030B.WriteDeviceID((unsigned char*)pidata);
	}
	break;
	case 23:
	{
		unsigned char iValue[4] = { 0 };
		//int len = 4;
		rtn = MSZTemp[ccdno].HZC0201030B.ReadDeviceID(iValue);
		int* fidata = (int *)pidata;
		*(fidata + 0) = iValue[0];
		*(fidata + 1) = iValue[1];
		*(fidata + 2) = iValue[2];
		*(fidata + 3) = iValue[3];
		//rtn = len;
	}
	break;
	case 24:
	{
		rtn = MSZTemp[ccdno].HZC0201030B.WriteDeviceSN((unsigned char*)pidata);
		//WriteDeviceSN(unsigned char* iparam, int& length)
	}
	break;
	case 25:
	{
		unsigned char iValue[4] = { 0 };
		//int len = 4;
		rtn = MSZTemp[ccdno].HZC0201030B.ReadDeviceSN(iValue);
		int* fidata = (int *)pidata;
		*(fidata + 0) = iValue[0];
		*(fidata + 1) = iValue[1];
		*(fidata + 2) = iValue[2];
		*(fidata + 3) = iValue[3];
		//rtn = len;
	}
	break;
	}
	
	pidata = NULL;
	return(rtn);
}