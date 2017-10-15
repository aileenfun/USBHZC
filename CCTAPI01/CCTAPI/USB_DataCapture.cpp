#include "StdAfx.h"
#include "USB_DataCapture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
CDataCapture::CDataCapture(CCyUSBDevice *usb)
{
	m_pDataProcess=NULL;
	m_iCount=0;		
	m_iRowIndex=0;		
	m_bFindDbFive=FALSE;	
	m_pInData=NULL;		
	m_pOutData=NULL;	
	m_Usb=usb;
}

CDataCapture::~CDataCapture(void)
{
	Close();
}
int CDataCapture::Open(CDataProcess *pProcess,int height,int width)
{
	m_pDataProcess=pProcess;
	g_height=height;
	g_width=width;
	g_width_L=g_width;
	ReadDataBytes=g_height*g_width+512;
	m_pReadBuff=new char[ReadDataBytes];//�˴�Ӧ��m_pReadBuff�Ƿ�����ڴ�ɹ������Ƿ�ΪNULL
	m_pOutData=new byte[g_height*g_width_L+1];//+1 added by qbc
	m_pInData=new byte[(ReadDataBytes*4)];
	//memset(m_pInData,0,(ReadDataBytes+g_width_L+3)*sizeof(byte));//(ReadDataBytes+g_width_L+3)*sizeof(byte)��ͬ��sizeof��m_pInData��
	m_bCapture=TRUE;
	m_hThread = (HANDLE)_beginthreadex(NULL,0,ThreadProcess,this,0,NULL);
	m_lBytePerSecond=0;
	return 0;
}
int CDataCapture::Close()
{
	m_bCapture=FALSE;
	Sleep(1000);
	m_bCapture=FALSE;
	return 0;
}
int CDataCapture::ThreadProcessFunction()
{
	long lRet=0;
	while(m_bCapture)
	{
		lRet=ReadDataBytes;
		ReadData(m_pReadBuff,lRet);
		if(lRet>0)
		{
			if(!m_bCapture) break;
			m_lBytePerSecond+=lRet;
			Input(m_pReadBuff,lRet);//���ԣ���ʱע��
		}
		Sleep(1);
	}
	if(m_pOutData!=NULL)
	{
		delete[] m_pOutData;
		m_pOutData=NULL;
	}
	if(m_pInData!=NULL)
	{
		delete[] m_pInData;
		m_pInData=NULL;
	}
	delete[] m_pReadBuff;
	return 0;
}
unsigned int __stdcall CDataCapture::ThreadProcess( void* handle )
{
	CDataCapture* pThis=(CDataCapture*)handle;
	pThis->ThreadProcessFunction();
	return 0;
}
int CDataCapture::Input( const LPVOID lpData,const DWORD dwSize )
{
	int iBytes=0;
	iBytes=dwSize+m_iCount;//m_iCount��һ�ο���ʣ������
	bool b_header=false,b_imu=false;
	memcpy(m_pInData+m_iCount,lpData,dwSize);
	int datalen=datalen=g_width*g_height;
	for(int i=0;i<iBytes;++i)
	{
		if ((i + datalen) >= iBytes)
		{
			m_iCount = iBytes - i;
			memcpy(m_pInData, m_pInData + i, m_iCount);
			return 0;
		}

		if(m_pInData[i]==0x33&&m_pInData[i+1]==0xcc&&m_pInData[i+14]==0x22&&m_pInData[i+15]==0xdd&&b_header==FALSE)
		{
			i=i+16;
			memcpy(m_pOutData,m_pInData+i,datalen);
			m_pDataProcess->Input(m_pOutData,datalen);
		}
#ifdef _USE55AA
		if(!m_bFindDbFive)
		{
			if(m_pInData[i]==0x55)
			{
				m_bFindDbFive=TRUE;
			}
			continue;
		}
		if(0xaa==m_pInData[i])
		{
			if((i+g_width_L+2)>=iBytes)//���ʣ�µ���󼸸����ݳ���С��video_width*2+2�кŸ��������Թ�������һ�У���������һ����
			{
				m_iCount=iBytes-i;
				memcpy(m_pInData,m_pInData+i,m_iCount);
				return 0;
			}
			m_iRowIndex=m_pInData[i+1];		//�кŸ�8λ
			m_iRowIndex<<=8;				 
			m_iRowIndex+=m_pInData[i+2];	//�кŵ�8λ
			if(m_iRowIndex>g_height+2)
			{
				//AfxMessageBox(L"�кų���");
				return 0;
				//exit(1);
			}
			memcpy(m_pOutData+m_iRowIndex*g_width_L,m_pInData+i+3,g_width_L);
			if(m_iRowIndex>=(g_height-1))
			{
				m_pDataProcess->Input(m_pOutData,g_height*g_width_L);
			}
			i=i+g_width_L+2;
		}
		m_bFindDbFive=FALSE;//�ҵ�0x55�������¸������ǲ���0xaa����״̬λΪFALSE,Ȼ��������0x55
#endif
	}
	return 0;
}
int CDataCapture::ReadData( char* pbuff,LONG &lBytes )
{
	if(m_Usb->IsOpen())
	{
		if(m_Usb->BulkInEndPt->GetXferSize()<lBytes)
		{
			m_Usb->BulkInEndPt->SetXferSize(lBytes);
		}
		if(m_Usb->BulkInEndPt->XferData((PUCHAR)pbuff,lBytes))
		{
			return 0;
		}
	}
	return -1;
}