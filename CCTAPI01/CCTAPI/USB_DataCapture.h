#pragma once
#include "USB_DataProcess.h"
#include "CyAPI.h"

class CDataCapture
{
public:
	CDataCapture(CCyUSBDevice *usb);
	virtual ~CDataCapture(void);

public:
	int Open(CDataProcess *pProcess,int height,int width);//
	int Close();
	int Input(const LPVOID lpData,const DWORD dwSize);
	int startThread();
	int ReadData(char* pbuff,LONG &lBytes);
	long m_lBytePerSecond;
private:
	CDataProcess *m_pDataProcess;
	int			m_iCount;		//���ݼ�����
	int			m_iRowIndex;	//������
	bool        m_bFindDbFive;	//����Ƿ��ҵ�55
	byte*		m_pInData;		//�������ݻ���
	byte*		m_pOutData;		//������ݻ���
	char* m_pReadBuff;
	long ReadDataBytes;
	long lRet;
	static unsigned int __stdcall ThreadProcess( void* handle );
	int ThreadProcessFunction();
	HANDLE m_hThread;
	volatile BOOL m_bCapture;
	int g_width_L;
	int g_width;
	int g_height;
	CCyUSBDevice *m_Usb;
};

