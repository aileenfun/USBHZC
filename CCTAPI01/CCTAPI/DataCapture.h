#pragma once
#include "DataProcess.h"
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
	int			m_iCount;		//数据计数器
	int			m_iRowIndex;	//行索引
	bool        m_bFindDbFive;	//标记是否找到55
	byte*		m_pInData;		//接收数据缓冲
	byte*		m_pOutData;		//输出数据缓冲
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

