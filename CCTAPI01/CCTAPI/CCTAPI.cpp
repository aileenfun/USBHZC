// CCTAPI.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CCTAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCCTAPIAppUSB

#define ORDER_IN 1
#define ORDER_OUT 0

// CCCTAPIApp construction
CCT_API CCCTAPIAppUSB::CCCTAPIAppUSB(HANDLE h)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bUsbOpen=FALSE;
	m_pDataProcess=NULL;
	m_pDataCapture=NULL;
	b_opened=false;
	m_Usb=new CCyUSBDevice(h);
}


int CCT_API CCCTAPIAppUSB::OpenUsb(int devNum)
{
	
	m_Usb->Open(devNum);
	if(m_Usb->IsOpen())
	{
		m_Usb->ControlEndPt->Target = TGT_DEVICE;
		m_Usb->ControlEndPt->ReqType = REQ_VENDOR;
		m_Usb->ControlEndPt->Value = 0;
		m_Usb->ControlEndPt->Index = 0;
		b_opened=true;
		return m_Usb->VendorID;
	}
	return -1;
}
int CCT_API CCCTAPIAppUSB::CloseUsb()
{
	if(m_Usb->IsOpen())
	{
		m_Usb->Close();
		b_opened=false;
		return 0;
	}
	return -1;
}

int CCT_API CCCTAPIAppUSB::startCap(int height,int width,LPMV_CALLBACK2 CallBackFunc,int camnum)
{
	if(!b_opened)
		return -1;
	Reset();
	b_closed=false;//全局变量
	m_pDataProcess=new CDataProcess();
	m_pDataCapture=new CDataCapture(m_Usb);
	m_pDataProcess->Open(height,width,CallBackFunc);
	m_pDataCapture->Open(m_pDataProcess,height,width);
	WrFpgaReg(0x09,0x00);//停止采集
	WrFpgaReg(0x09,0x01);//开启数据传输
	return 0;
}

int CCT_API CCCTAPIAppUSB::stopCap()
{

	if(b_closed)
		return -1;
	b_closed=true;
	m_pDataCapture->Close();
	m_pDataProcess->Close();
	WrFpgaReg(0x09,0x00);
	Reset();
	return 0;
}
void CCT_API CCCTAPIAppUSB::WrSensorReg(unsigned short iAddr, unsigned short iValue)
{
	if(!b_opened)
		return ;
	int len=2;
	usbOrderWrapper(0xF1,ORDER_OUT,iAddr,iValue,0,len);
	return;

}
unsigned short CCT_API CCCTAPIAppUSB::RdSensorReg(unsigned short iAddr)
{
	if(!b_opened)
		return -1;
	UINT8 rxval[2];
	int len=2;
	usbOrderWrapper(0xF2,ORDER_IN,iAddr,0,rxval,len);
	
	unsigned short irxval = rxval[0] << 8;
	irxval += rxval[1];
	return irxval;

}
void CCCTAPIAppUSB::WrFpgaReg(unsigned char iAddr, unsigned char iValue)
{
	if(!b_opened)
		return ;
	int len=1;
	usbOrderWrapper(0xF3,ORDER_OUT,iAddr,iValue,0,len);
	return;

}
unsigned char CCCTAPIAppUSB::RdFpgaReg(unsigned char iAddr)
{	
	UINT8 rxval[1]={0};
	int len=1;
	usbOrderWrapper(0xF4,ORDER_IN,iAddr,0,rxval,len);
	return rxval[1];
}
unsigned char CCT_API  CCCTAPIAppUSB::WrEEPROM(WORD iAddr,WORD iValue)
{
	unsigned char rst;
	int len=2;
	unsigned char readbuff[2];
	usbOrderWrapper(0xF5,ORDER_OUT,iAddr,iValue,readbuff,len);
	return len;
}
unsigned char CCT_API  CCCTAPIAppUSB::RdEEPROM(WORD iAddr,unsigned char * buffer,int &length)
{
	int len=2;
	usbOrderWrapper(0xF6,ORDER_IN,iAddr,0,buffer,len);
	length=len;
	return len;
}
/*
int CCCTAPIAppUSB::SendOrder( PUSB_ORDER pOrder )
{
	if(m_Usb!=NULL&&m_Usb->IsOpen())
	{
		m_Usb->ControlEndPt->Target=(CTL_XFER_TGT_TYPE)pOrder->Target;
		m_Usb->ControlEndPt->ReqType=(CTL_XFER_REQ_TYPE)pOrder->ReqType;
		m_Usb->ControlEndPt->Direction=(CTL_XFER_DIR_TYPE)pOrder->Direction;
		m_Usb->ControlEndPt->ReqCode=pOrder->ReqCode;
		m_Usb->ControlEndPt->Value=pOrder->Value;
		m_Usb->ControlEndPt->Index=pOrder->Index;
		LONG lBytes=0;
		lBytes=pOrder->DataBytes;
		if(m_Usb->ControlEndPt->XferData((PUCHAR)(pOrder->pData),lBytes))
		{
			pOrder->DataBytes=lBytes;
			return 0;
		}
		pOrder->DataBytes=0;
	}
	return -1;
}
*/
int CCCTAPIAppUSB::usbOrderWrapper(int code,int dir,int index,int value,unsigned char *buffer, int &len)
{
	if(m_Usb==NULL&&!m_Usb->IsOpen())
	{
		return -1;
	}

	LONG lBytes=len;
	m_Usb->ControlEndPt->Target=TGT_DEVICE;
	m_Usb->ControlEndPt->ReqType=REQ_VENDOR;
	m_Usb->ControlEndPt->Direction=(CTL_XFER_DIR_TYPE)dir;
	m_Usb->ControlEndPt->ReqCode=code;
	m_Usb->ControlEndPt->Value=value;
	m_Usb->ControlEndPt->Index=index;
	UINT8 dummybuffer[64];
	if(buffer==NULL)
		buffer=dummybuffer;
	lBytes=len;

	if(dir==ORDER_OUT)
	{
		if(len>64)
		{
			return -64;
		}
		m_Usb->ControlEndPt->XferData(buffer,lBytes);
		return len;
	}
	else
	{
		m_Usb->ControlEndPt->XferData(buffer,lBytes);
		if(len>=lBytes)
		{
			return lBytes;
		}
		else//not enough space
		{
			return -lBytes;
		}
	}
	
}
int CCT_API CCCTAPIAppUSB::RdDeviceID(unsigned char* buff,int& length)
{
	int len=1;
	byte readbuff[1];
	usbOrderWrapper(0xD1,ORDER_IN,0,0,readbuff,len);
	memcpy(buff,readbuff,len);
	length=len;
	return len;
}
int CCT_API CCCTAPIAppUSB::WrDeviceID(unsigned char* buff,int& length)
{
	int len=1;
	usbOrderWrapper(0xD0,ORDER_OUT,0,0,buff,len);
	length=len;
	return len;
}
int CCT_API CCCTAPIAppUSB::RdDeviceSN(unsigned char* buff,int& length)
{
	int len=4;
	byte readbuff[4];
	usbOrderWrapper(0xD3,ORDER_IN,0,0,readbuff,len);
	memcpy(buff,readbuff,len);
	length=len;
	return len;
}
int CCT_API CCCTAPIAppUSB::WrDeviceSN(unsigned char* buff,int& length)
{
	int len=4;
	usbOrderWrapper(0xD2,ORDER_OUT,0,0,buff,len);
	length=len;
	return len;
}
void CCT_API CCCTAPIAppUSB::InitSensor(void)
{
	if(!b_opened)
		return ;
	int dummy=2;
	usbOrderWrapper(0xF0,ORDER_OUT,0,0,0,dummy);
	return;

}
void CCT_API CCCTAPIAppUSB::GetFpsMbs(int &fCount,long &lBytePerSecond )
{
	if(m_pDataProcess!=NULL)
	{
		m_pDataProcess->GetFrameCount(fCount);
		lBytePerSecond=m_pDataCapture->m_lBytePerSecond;
		//str.Format(L"%d Fps     %0.4f MBs",FCount,float(m_lBytePerSecond)/1024.0/1024.0);
		m_pDataCapture->m_lBytePerSecond=0;
	}

}
void CCT_API CCCTAPIAppUSB::SetTrigMode(TRIGMODE m)
{
		switch(m)
		{
			case TgMd_Auto:
				WrSensorReg(0x07,0x0388);
				WrSensorReg(0x20,0x01c1);
				WrFpgaReg(0,0);
				break;
			case TgMd_FPGA:
				WrSensorReg(0x07,0x0398);
				WrSensorReg(0x20,0x03d5);
				WrFpgaReg(0,1);
				break;
			case TgMd_Soft:
				WrSensorReg(0x07,0x0398);
				WrSensorReg(0x20,0x03d5);
				WrFpgaReg(0,2);
				break;
			case TgMd_OutSig:
				WrSensorReg(0x07,0x0398);
				WrSensorReg(0x20,0x03d5);
				WrFpgaReg(0,3);
				break;
			default:
				WrSensorReg(0x07,0x0388);
				WrSensorReg(0x20,0x01c1);
				WrFpgaReg(0,0);
				break;
		}
}

void CCT_API CCCTAPIAppUSB::setFpgaFreq(unsigned char fpgafreq)
{
	WrFpgaReg(0x05, fpgafreq & 0xff);
}
void CCT_API CCCTAPIAppUSB::setGainValue(unsigned short GainValue)
{
	WrSensorReg(0x35, GainValue);
}
void  CCT_API CCCTAPIAppUSB::setExpoValue(unsigned short ExpoValue)
{
	WrSensorReg(0x0b, ExpoValue);
}
void CCT_API CCCTAPIAppUSB::setAutoGainExpo(bool isAutoGain, bool isAutoExpo)
{
	if((isAutoGain==true)&&(isAutoExpo==true))
		WrSensorReg(0xAF, 0x0003);
	else if ((isAutoGain==true)&&(isAutoExpo==false))
		WrSensorReg(0xAF, 0x0002);
	else if ((isAutoGain==false)&&(isAutoExpo==true))
		WrSensorReg(0xAF, 0x0001);
	else if ((isAutoGain==false)&&(isAutoExpo==false))
		WrSensorReg(0xAF, 0x0000);
	else;
}

void  CCT_API CCCTAPIAppUSB::setResolution(unsigned char resNo)
{
	if(!b_opened)
		return;
	if(resNo==0)//752*480
	{
		WrSensorReg(1,1);
		WrSensorReg(2, 4);
		WrSensorReg(3, 0x01e0);
		WrSensorReg(4,0x02f0);

		WrFpgaReg(0x01, 0x2);
		WrFpgaReg(0x02, 0xf0);
		WrFpgaReg(0x03, 1);
		WrFpgaReg(0x04, 0xe0);
	}
	else if(resNo==1)//640*480
	{
		WrSensorReg(0x1, 0x0038);
		WrSensorReg(0x2, 0x0004);
		WrSensorReg(0x3, 0x01e0);
		WrSensorReg(0x4, 0x0280);

		WrFpgaReg(0x01, 0x02);
		WrFpgaReg(0x02, 0x80);
		WrFpgaReg(0x03, 0x01);
		WrFpgaReg(0x04, 0xe0);
	}
}
void CCT_API CCCTAPIAppUSB::setNormal(void)
{
	WrSensorReg(0x0d, 0x0300);
}
void CCT_API CCCTAPIAppUSB::setXmirror(void)
{
	WrSensorReg(0x0d, 0x320);
}
void CCT_API CCCTAPIAppUSB::setYmirror(void)
{
	WrSensorReg(0x0d, 0x310);
}
void CCT_API CCCTAPIAppUSB::setXYmirror(void)
{
	WrSensorReg(0x0d, 0x330);
}
void CCT_API CCCTAPIAppUSB::SendUsbSpeed2Fpga(unsigned char speedType)
{
	WrFpgaReg(0x8,speedType);
}




int CCCTAPIAppUSB::WriteData( char* pbuff,LONG &lBytes )
{
	if(m_Usb->IsOpen())
	{
		if(m_Usb->BulkOutEndPt->XferData((PUCHAR)pbuff,lBytes))
		{
			return lBytes;
		}
		return -1;
	}
}
int CCCTAPIAppUSB::Reset()
{
	Endpt=m_Usb->EndPointOf(0x86);
	if(Endpt) Endpt->Reset();
	return 0;
}
int CCCTAPIAppUSB::DeviceReset()
{
	m_Usb->Reset();
	return 0;
}
int CCCTAPIAppUSB::softTrigOnce()
{
	WrFpgaReg(0x0a,0);
	WrFpgaReg(0x0a,1);

	return 0;
}