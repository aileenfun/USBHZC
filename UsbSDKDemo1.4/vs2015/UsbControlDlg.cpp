
// UsbControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UsbControl.h"	
#include "UsbControlDlg.h"
#include "afxdialogex.h"
#include "CyAPI.h"
#include <Dbt.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//added by qbc
#define RESOLUTION_752_480 0x55
#define RESOLUTION_640_480  0x56
unsigned char g_byteResolutionType = RESOLUTION_752_480;
GUID guid = {0xBE18AA60, 0x7F6A, 0x11D4, 0x97, 0xDD, 0x00, 0x01, 0x02, 0x29, 0xB9, 0x59};

int g_width=752;
int g_height=480;
bool g_bSaveVedio = false;
HWND m_hwnd;
cv::VideoWriter h_vw;// ("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, cv::Size(g_width, g_height), false);
volatile bool snap;
volatile bool save_all;

bool g_bRoi = false;
bool g_bRoiBox = false;
unsigned int g_iRoiWidth = 10;
unsigned int g_iRoiHeight = 10;

BYTE* g_byteRoiBuffer1 = NULL;
BYTE* g_byteRoiBuffer2 = NULL;
BYTE* g_byteRoiBuffer3 = NULL;
BYTE* g_byteRoiBuffer4 = NULL;
BYTE* g_byteRoiBuffer5 = NULL;

bool g_bFillRoiBuf = false;

CPoint g_ptRoi1(0,0);
CPoint g_ptRoi2(0, 0);
CPoint g_ptRoi3(0, 0);
CPoint g_ptRoi4(0, 0);
CPoint g_ptRoi5(0, 0);

bool g_bRoiWidthOK = false;
bool g_bRoiHeightOK = false;


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CUsbControlDlg �Ի���
CUsbControlDlg::CUsbControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUsbControlDlg::IDD, pParent)
	, m_iProcType(0)
	, m_sEdit_Width(_T(""))
	, m_sEdit_Height(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pFileRbf=NULL;
	m_bUsbOpen=FALSE;
	m_bSendData=FALSE;
	memset(m_byData,0,sizeof(BYTE)*64);
	m_sUsbOrder.pData=m_byData;
	m_pVideoDlg=NULL;
	m_bCloseWnd=FALSE;
	m_pBrush=NULL;
	m_pVideoDataFile=NULL;
	m_bReview=FALSE;
	m_bSave=FALSE;
	m_Init = FALSE;
	snap=false;
	h_cctapi=NULL;
	h_cctapi2=NULL;
	g_bRoi = false;
	g_bRoiBox = false;
	g_iRoiWidth = 0;
	g_iRoiHeight = 0;
}

CUsbControlDlg::~CUsbControlDlg()
{
	
}

void CUsbControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_NORMAL, m_iProcType);
	DDX_Control(pDX, IDC_EDITHwTrigFreq, eFpgaFreq);
	DDX_Control(pDX, IDC_EDITGainValue, eGainValue);
	DDX_Control(pDX, IDC_EDITExpoValue, eExpoValue);
	DDX_Control(pDX, IDC_CHECKAutoGain, cbAutoGain);
	DDX_Control(pDX, IDC_CHECKAutoExpo, cbAutoExpo);
	DDX_Control(pDX, IDC_CHECK_SAVEALL, m_chk_save_all);
}

BEGIN_MESSAGE_MAP(CUsbControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_VIDEOCAPTURE, &CUsbControlDlg::OnBnClickedBtnVideocapture)
	ON_BN_CLICKED(IDC_BTN_STOPCAPTURE, &CUsbControlDlg::OnBnClickedBtnStopcapture)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SNAP, &CUsbControlDlg::OnBnClickedBnSnap)
	ON_EN_CHANGE(IDC_EDITGainValue, &CUsbControlDlg::setGainValue)
	ON_BN_CLICKED(IDC_CHECKAutoGain, &CUsbControlDlg::setAutoGainExpo)
	ON_BN_CLICKED(IDC_CHECKAutoExpo, &CUsbControlDlg::setAutoGainExpo)
	ON_EN_CHANGE(IDC_EDITExpoValue, &CUsbControlDlg::setExpoValue)
	ON_EN_CHANGE(IDC_EDITHwTrigFreq, &CUsbControlDlg::setFpgaFreq)
	ON_BN_CLICKED(IDC_CHECK_SAVEALL, &CUsbControlDlg::OnBnClickedCheckSaveall)
	ON_BN_CLICKED(IDC_BUTTON_WR_SENSOR, &CUsbControlDlg::OnBnClickedButtonWrSensor)
	ON_BN_CLICKED(IDC_BUTTON_RD_SENSOR, &CUsbControlDlg::OnBnClickedButtonRdSensor)
	ON_BN_CLICKED(IDC_BUTTON_WR_EEPROM, &CUsbControlDlg::OnBnClickedButtonWrEeprom)
	ON_BN_CLICKED(IDC_BUTTON_RD_EEPROM, &CUsbControlDlg::OnBnClickedButtonRdEeprom)
	ON_BN_CLICKED(IDC_BUTTON_INIT_SENSOR, &CUsbControlDlg::OnBnClickedButtonInitSensor)
	ON_BN_CLICKED(IDC_RADIO1, &CUsbControlDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CUsbControlDlg::OnBnClickedRadio2)

	ON_BN_CLICKED(IDC_RADIO_NORMAL, &CUsbControlDlg::OnBnClickedRadioNormal)
	ON_BN_CLICKED(IDC_RADIO_XMIRROR, &CUsbControlDlg::OnBnClickedRadioXmirror)
	ON_BN_CLICKED(IDC_RADIO_YMIRROR, &CUsbControlDlg::OnBnClickedRadioYmirror)
	ON_BN_CLICKED(IDC_RADIO_XYMIRROR, &CUsbControlDlg::OnBnClickedRadioXymirror)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_USB, &CUsbControlDlg::OnBnClickedButtonOpenUsb)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_USB, &CUsbControlDlg::OnBnClickedButtonCloseUsb)
	ON_BN_CLICKED(IDC_CHECK_SAVE_VEDIO, &CUsbControlDlg::OnBnClickedCheckSaveVedio)
	ON_BN_CLICKED(IDC_RADIO_AUTO_TRIG, &CUsbControlDlg::OnBnClickedRadioAutoTrig)
	ON_BN_CLICKED(IDC_BUTTON_USB_SPEED_CKECK, &CUsbControlDlg::OnBnClickedButtonUsbSpeedCkeck)
	ON_BN_CLICKED(IDC_RADIO_FPGA_TRIG, &CUsbControlDlg::OnBnClickedRadioFpgaTrig)

	ON_BN_CLICKED(IDC_RADIO_SOFT_TRIG, &CUsbControlDlg::OnBnClickedRadioSoftTrig)
	ON_BN_CLICKED(IDC_RADIO_EXT_TRIG, &CUsbControlDlg::OnBnClickedRadioExtTrig)
	ON_BN_CLICKED(IDC_BUTTON_RD_ID, &CUsbControlDlg::OnBnClickedButtonRdId)
	ON_BN_CLICKED(IDC_BTN_SOFTTRIG, &CUsbControlDlg::OnBnClickedBtnSofttrig)
	ON_WM_DEVICECHANGE()
	ON_BN_CLICKED(IDC_BUTTON_Wr_ID, &CUsbControlDlg::OnBnClickedButtonWrId)
	ON_BN_CLICKED(IDC_BUTTON_RD_SN, &CUsbControlDlg::OnBnClickedButtonRdSn)
	ON_BN_CLICKED(IDC_BUTTON_Wr_SN, &CUsbControlDlg::OnBnClickedButtonWrSn)
END_MESSAGE_MAP()


BOOL CUsbControlDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN)// || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;                // Do not process further
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

// CUsbControlDlg ��Ϣ�������
BOOL CUsbControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//------------------------------------
	CRect cRect,wRect,mRect;
	GetDesktopWindow()->GetWindowRect(wRect);
	GetWindowRect(cRect);
	mRect.right=wRect.right-50;
	mRect.bottom=wRect.bottom-50;
	mRect.left=mRect.right-cRect.Width();
	mRect.top=mRect.bottom-cRect.Height();
	MoveWindow(mRect);
	//------------------------------------
	m_pFileRbf=new CFile();
	SetTimer(1,1000,NULL);

	m_pBrush=new CBrush[2];
	m_pBrush[0].CreateSolidBrush(RGB(99,208,242));
	m_pBrush[1].CreateSolidBrush(RGB(174,238,250));

	cbAutoExpo.SetCheck(1);
	cbAutoGain.SetCheck(1);


	SetDlgItemText(IDC_STATIC_TEXT, L" ");
	GetDlgItem(IDC_EDITHwTrigFreq)->EnableWindow(false);

	CButton* pBtn = (CButton *)GetDlgItem(IDC_RADIO_AUTO_TRIG);
	pBtn->SetCheck(1);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO1);
	pBtn->SetCheck(1);
	
	SetDlgItemText(IDC_EDITHwTrigFreq, L"25");
	CCyUSBDevice udev=new CCyUSBDevice(NULL);
	devcnt=udev.DeviceCount();
	udev.Close();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUsbControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();//���ؿ���̨����ģ��
		int temp1 = 1;
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUsbControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUsbControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void BMPHeader(int lWidth, int lHeight,byte* m_buf)
{
	int mlBpp=8;
	bool lReverse=true;
	BITMAPFILEHEADER bhh;
	BITMAPINFOHEADER bih;
	memset(&bhh,0,sizeof(BITMAPFILEHEADER));
	memset(&bih,0,sizeof(BITMAPINFOHEADER));

	int widthStep				=	(((lWidth * mlBpp) + 31) & (~31)) / 8; //ÿ��ʵ��ռ�õĴ�С��ÿ�ж�����䵽һ��4�ֽڱ߽磩
	int QUADSize 				= 	mlBpp==8?sizeof(RGBQUAD)*256:0;

	//�����ɫͼ���ļ�ͷ
	bhh.bfOffBits				=	(DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + QUADSize; 
	bhh.bfSize					=	(DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + QUADSize + widthStep*lHeight;  
	bhh.bfReserved1				=	0; 
	bhh.bfReserved2				=	0;
	bhh.bfType					=	0x4d42;  

	//�����ɫͼ����Ϣͷ
	bih.biBitCount				=	mlBpp;
	bih.biSize					=	sizeof(BITMAPINFOHEADER);
	bih.biHeight				=	(lReverse?-1:1)*lHeight;
	bih.biWidth					=	lWidth;  
	bih.biPlanes				=	1;
	bih.biCompression			=	BI_RGB;
	bih.biSizeImage				=	widthStep*lHeight;  
	bih.biXPelsPerMeter			=	0;  
	bih.biYPelsPerMeter			=	0;  
	bih.biClrUsed				=	0;  
	bih.biClrImportant			=	0;  
	
	//����Ҷ�ͼ�ĵ�ɫ��
	RGBQUAD rgbquad[256];
	if(mlBpp==8)
	{
		for(int i=0;i<256;i++)
		{
			rgbquad[i].rgbBlue=i;
			rgbquad[i].rgbGreen=i;
			rgbquad[i].rgbRed=i;
			rgbquad[i].rgbReserved=0;
		}
	}

	int DIBSize = widthStep * lHeight;
	//TRACE(_T("DIBSIze is %d"),DIBSize);

	bool b_save_file	=true;
	if(b_save_file)
	{
		CString strName;
		CString camFolder;
		camFolder.Format(L"d:\\c6UDP\\cam%d",0);
		if(CreateDirectory(camFolder,NULL)||ERROR_ALREADY_EXISTS == GetLastError())
		{
			int iFileIndex=1;
			do 
			{
				strName.Format(L"d:\\c6UDP\\cam%d\\V_%d.bmp",0,iFileIndex);
				++iFileIndex;
			} while (_waccess(strName,0)==0);
			CT2CA pszConvertedAnsiString (strName);
			std::string cvfilename(pszConvertedAnsiString);
			
			CFile file;  
			if(file.Open(strName,CFile::modeWrite | CFile::modeCreate))  
			{
				file.Write((LPSTR)&bhh,sizeof(BITMAPFILEHEADER));  
				file.Write((LPSTR)&bih,sizeof(BITMAPINFOHEADER));  
				if(mlBpp==8) file.Write(&rgbquad,sizeof(RGBQUAD)*256);
				file.Write(m_buf,DIBSize);  
				file.Close();  
				return ;  
			}  
		}
	}

	
}

void _stdcall RawCallBack(LPVOID lpParam, LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	
	cv::Mat frame(g_height, g_width, CV_8UC1,pDataBuffer);
	cv::imshow("disp",frame);
	if(snap||save_all)
	{
		CString strName;
		CString camFolder;
		//camFolder.Format(L"cam%d",0);
		if(1)//CreateDirectory(camFolder,NULL)||ERROR_ALREADY_EXISTS == GetLastError())
		{
			int iFileIndex=1;
			do 
			{
				strName.Format(L"V1_%d.bmp",iFileIndex);
				++iFileIndex;
			} while (_waccess(strName,0)==0);
		
			CT2CA pszConvertedAnsiString (strName);
			std::string cvfilename(pszConvertedAnsiString);

			cv::vector<int> compression_params;
			compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
			compression_params.push_back(0);    // ��ѹ��png.  
			//sprintf(fileName, "Picture %d.png", ++countCamera); //�����ļ���  
			//cv::Mat pImgMat(pImg, 0);  //��IplImageת��ΪMat  
			//imwrite(fileName, pImgMat, compression_params);  //����ͼƬ</span>  


			cv::imwrite(cvfilename,frame, compression_params);
			snap=false;
		}
	}
	if (g_bSaveVedio == true)
		h_vw.write(frame);//save video operation

}
void _stdcall RawCallBack2(LPVOID lpParam, LPVOID lpUser)
{
	BYTE *pDataBuffer = (BYTE*)lpParam;
	
	cv::Mat frame(g_height, g_width, CV_8UC1,pDataBuffer);
	cv::imshow("disp1",frame);
	
	if(snap||save_all)
	{
		CString strName;
		CString camFolder;
		//camFolder.Format(L"cam%d",0);
		if(1)//CreateDirectory(camFolder,NULL)||ERROR_ALREADY_EXISTS == GetLastError())
		{
			int iFileIndex=1;
			do 
			{
				strName.Format(L"V2_%d.bmp",iFileIndex);
				++iFileIndex;
			} while (_waccess(strName,0)==0);

			CT2CA pszConvertedAnsiString (strName);
			std::string cvfilename(pszConvertedAnsiString);

			cv::vector<int> compression_params;
			compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
			compression_params.push_back(0);   


			cv::imwrite(cvfilename,frame, compression_params);
			snap=false;
		}
	}
	if (g_bSaveVedio == true)
		h_vw.write(frame);//save video operation
		

}
void CUsbControlDlg::OnBnClickedBtnVideocapture()
{
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������

	GetDlgItem(IDC_RADIO1)->EnableWindow(false);
	GetDlgItem(IDC_RADIO2)->EnableWindow(false);

	cv::namedWindow("disp");
	cv::namedWindow("disp1");
	HWND hWnd = (HWND)cvGetWindowHandle("disp");//��ȡ�Ӵ��ڵ�HWND
	HWND hParentWnd = ::GetParent(hWnd);//��ȡ������HWND��������������Ҫ�õ�

	 //���ش��ڱ����� 
	long style = GetWindowLong(hParentWnd, GWL_STYLE);
	style &= ~(WS_SYSMENU);
	SetWindowLong(hParentWnd, GWL_STYLE, style);
	
	 hWnd = (HWND)cvGetWindowHandle("disp1");
	 hParentWnd = ::GetParent(hWnd);
	style = GetWindowLong(hParentWnd, GWL_STYLE);
	style &= ~(WS_SYSMENU);
	SetWindowLong(hParentWnd, GWL_STYLE, style);

	SetDlgItemText(IDC_STATIC_TEXT,L"�ɼ���...");
	if(h_cctapi->startCap(g_height,g_width,RawCallBack,0)<0)
	{
	SetDlgItemText(IDC_STATIC_TEXT,L"USB�豸��ʧ�ܣ�");

	GetDlgItem(IDC_RADIO1)->EnableWindow(true);
	GetDlgItem(IDC_RADIO2)->EnableWindow(true);
	return;
	}
	

	if(h_cctapi2->startCap(g_height,g_width,RawCallBack2,1)<0)
	{
	SetDlgItemText(IDC_STATIC_TEXT,L"USB2�豸��ʧ�ܣ�");
	return;
	}

}


void CUsbControlDlg::OnBnClickedBtnStopcapture()
{
	if(h_cctapi->stopCap()!=0)
	{
		SetDlgItemText(IDC_STATIC_TEXT,L"��δ�ɼ�");
		
	}
	if(h_cctapi2->stopCap()!=0)
	{
		SetDlgItemText(IDC_STATIC_TEXT,L"��δ�ɼ�");
		
	}

	cv::destroyWindow("disp");
	cv::destroyWindow("disp1");
	SetDlgItemText(IDC_STATIC_TEXT,L" ");

	GetDlgItem(IDC_RADIO1)->EnableWindow(true);
	GetDlgItem(IDC_RADIO2)->EnableWindow(true);
	
	SetDlgItemText(IDC_STATIC_TEXT, L"ֹͣ�ɼ���");

	//h_vw.release();
}

void CUsbControlDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
	KillTimer(1);

	m_bCloseWnd=TRUE;

	Sleep(100);

	//CloseUsb();
	//CyUsb_Destroy();
	/*
	if(m_pBrush!=NULL)
	{
		for(int i=0;i<2;++i)
		{
			if(m_pBrush[i].m_hObject!=NULL)
			{
				m_pBrush[i].DeleteObject();
			}
		}
		delete[] m_pBrush;
		m_pBrush=NULL;
	}
	*/
}


BOOL CUsbControlDlg::OnDeviceChange( UINT nEventType, DWORD dwData )
{
	CCyUSBDevice udev=new CCyUSBDevice();
	if(devcnt>udev.DeviceCount())
	{

		devcnt=udev.DeviceCount();
		MessageBox(L"-----");
	}
	else if(devcnt<udev.DeviceCount())
	{
		devcnt=udev.DeviceCount();
		MessageBox(L"+++++");
	}
	devcnt=udev.DeviceCount();
	udev.Close();
	return TRUE;
}
void CUsbControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int iFrame=0;
	long lBytePerSecond = 0;


	CString str;
	/*
	switch(nIDEvent)
	{
		case 1:
			{
				if((h_cctapi!=NULL)&&(h_cctapi->b_opened))
				{
					h_cctapi->GetFpsMbs(iFrame, lBytePerSecond);
					str.Format(L"cam1: %d Fps     %0.4f MBs",iFrame,float(lBytePerSecond)/1024.0/1024.0);
					HWND hWnd = (HWND)cvGetWindowHandle("disp");//��ȡ�Ӵ��ڵ�HWND
					HWND hParentWnd = ::GetParent(hWnd);//��ȡ������HWND��������������Ҫ�õ�

					if(hParentWnd !=NULL)
					{
						::SetWindowText(hParentWnd,str);
					}
				}
				if((h_cctapi2!=NULL)&&(h_cctapi2->b_opened))
				{
					h_cctapi2->GetFpsMbs(iFrame, lBytePerSecond);
					str.Format(L"cam2: %d Fps     %0.4f MBs",iFrame,float(lBytePerSecond)/1024.0/1024.0);
					HWND hWnd = (HWND)cvGetWindowHandle("disp1");//��ȡ�Ӵ��ڵ�HWND
					HWND hParentWnd = ::GetParent(hWnd);//��ȡ������HWND��������������Ҫ�õ�

					if(hParentWnd !=NULL)
					{
						::SetWindowText(hParentWnd,str);
					}
				}
			}
		default:
			break;
	}
	*/
	CDialogEx::OnTimer(nIDEvent);
}

//void CUsbControlDlg::OnBnClickedRadioProcType()
//{
//	UpdateData(TRUE);
	//h_cctapi->setMirrorType(DataProcessType(m_iProcType));
//}


HBRUSH CUsbControlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  �ڴ˸��� DC ���κ�����
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	int ID=pWnd->GetDlgCtrlID();
	if(ID==IDC_STATIC_TEXT)
	{
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);
	}
	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_BTN:
		return m_pBrush[0];
	case CTLCOLOR_STATIC:
		return m_pBrush[0];
	default:
		return hbr;
	}
}


void CUsbControlDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString s_temp;
	UpdateData(true);
	s_temp = m_sEdit_Width.GetString();
	g_width = _tstoi(s_temp);
	s_temp.ReleaseBuffer();
}


void CUsbControlDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString s_temp;
	UpdateData(true);
	s_temp = m_sEdit_Height.GetString();
	g_height = _tstoi(s_temp);
	s_temp.ReleaseBuffer();
}

void CUsbControlDlg::Split(CString in, CString *out, int &outcnt, CString spliter,int maxn)
{

	int d_len=spliter.GetLength();
	int j=0;
	int n=0;
	int m_pos;
	while(1)
	{
		m_pos= in.Find(spliter,j);
		if(m_pos==-1 && j==0)
		{
			out[0]=in.Mid(0);
			outcnt=0;//-1
			break;
		}
		if((m_pos==-1 && j!=0)||(n==maxn))
		{
			out[n]=in.Mid(j,in.GetLength()-j);
			outcnt=n;
			break;
		}

		if(j==0)
		{
			out[n]=in.Mid(0,m_pos);
			j=m_pos+d_len;
		}
		else
		{
			out[n]=in.Mid(j,m_pos-j);
			j=m_pos+d_len;
		}
		n++;
	}
}

void CUsbControlDlg::OnBnClickedBnSnap()
{
	SetDlgItemText(IDC_STATIC_TEXT, L"��ʼ�����ͼ��");
	snap=true;
}

void CUsbControlDlg::setFpgaFreq()
{
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	SetDlgItemText(IDC_STATIC_TEXT, L"FPGA����Ƶ���趨�ɹ���");
	CString s_temp;
	UpdateData(true);
	eFpgaFreq.GetWindowText(s_temp);
	unsigned char fpgafreq= _tstoi(s_temp);
	s_temp.ReleaseBuffer();
	//int a=cbTrigMode.GetCurSel();
	if(fpgafreq>0)
	{
		h_cctapi->setFpgaFreq(fpgafreq & 0xff);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_TEXT,L"Check Trig value");
	}
}


void CUsbControlDlg::setGainValue()
{
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	
	CString s_temp;
	UpdateData(true);
	eGainValue.GetWindowText(s_temp);
	unsigned short GainValue= _tstoi(s_temp);
	s_temp.ReleaseBuffer();

	if(GainValue>0)
	{
		h_cctapi->setGainValue(GainValue);
		SetDlgItemText(IDC_STATIC_TEXT, L"����ֵ�趨�ɹ���");
	}
	else
	{
		SetDlgItemText(IDC_STATIC_TEXT,L"Check Gain?");
	}
}


void CUsbControlDlg::setAutoGainExpo()
{
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	if ((cbAutoGain.GetCheck() == true) && (cbAutoExpo.GetCheck() == true))
	{
		h_cctapi->setAutoGainExpo(true, true);

		//GetDlgItem(IDC_EDITGainValue)->EnableWindow(false);
		//GetDlgItem(IDC_EDITExpoValue)->EnableWindow(false);
		SetDlgItemText(IDC_STATIC_TEXT, L"�Զ����棬�Զ��ع⡣");
	}	
	else if ((cbAutoGain.GetCheck() == true) && (cbAutoExpo.GetCheck() == false))
	{
		h_cctapi->setAutoGainExpo(true, false);

		//GetDlgItem(IDC_EDITGainValue)->EnableWindow(false);
		//GetDlgItem(IDC_EDITExpoValue)->EnableWindow(true);
		SetDlgItemText(IDC_STATIC_TEXT, L"�Զ����棬�ֶ��ع⡣");
	}
	else if ((cbAutoGain.GetCheck() == false) && (cbAutoExpo.GetCheck() == true))
	{
		h_cctapi->setAutoGainExpo(false, true);

		//GetDlgItem(IDC_EDITGainValue)->EnableWindow(true);
		//GetDlgItem(IDC_EDITExpoValue)->EnableWindow(false);
		SetDlgItemText(IDC_STATIC_TEXT, L"�ֶ����棬�Զ��ع⡣");
	}
	else if ((cbAutoGain.GetCheck() == false) && (cbAutoExpo.GetCheck() == false))
	{
		h_cctapi->setAutoGainExpo(false, false);

		//GetDlgItem(IDC_EDITGainValue)->EnableWindow(true);
		//GetDlgItem(IDC_EDITExpoValue)->EnableWindow(true);
		SetDlgItemText(IDC_STATIC_TEXT, L"�ֶ����棬�ֶ��ع⡣");
	}
	else;
}


void CUsbControlDlg::setExpoValue()
{
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	
	CString s_temp;
	UpdateData(true);
	eExpoValue.GetWindowText(s_temp);
	unsigned short ExpoValue= _tstoi(s_temp);
	s_temp.ReleaseBuffer();
	if(/*cbAutoExpo.GetCheck()==false&&*/ExpoValue>0)
	{
		h_cctapi->setExpoValue(ExpoValue);
		SetDlgItemText(IDC_STATIC_TEXT, L"�ع�ֵ�趨�ɹ���");
	}
	else
	{
		SetDlgItemText(IDC_STATIC_TEXT,L"Check Expo?");
	}
}

void CUsbControlDlg::OnBnClickedCheckSaveall()
{
	// TODO: Add your control notification handler code here
	save_all=m_chk_save_all.GetCheck();

}


void CUsbControlDlg::OnBnClickedButtonWrSensor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	CString strAddr, strValue;

	GetDlgItemText(IDC_EDIT_SENSOR_REGISTER_ADDR, strAddr);
	GetDlgItemText(IDC_EDIT_SENSOR_REGISTER_VALUE, strValue);

	unsigned short iAddr = str2hex(strAddr);
	unsigned short iValue = str2hex(strValue);

	h_cctapi->WrSensorReg(iAddr, iValue);
	SetDlgItemText(IDC_STATIC_TEXT, L"����Sensor�Ĵ����ɹ���");
}
int CUsbControlDlg::str2hex(CString str)
{
	int nLength = str.GetLength();
	int nBytes = WideCharToMultiByte(CP_ACP, 0, str, nLength, NULL, 0, NULL, NULL);
	char* p = new char[nBytes + 1];
	memset(p, 0, nLength + 1);
	WideCharToMultiByte(CP_OEMCP, 0, str, nLength, p, nBytes, NULL, NULL);
	p[nBytes] = 0;
	int a;
	sscanf(p, "%x", &a);
	delete[] p;
	return a;

}


void CUsbControlDlg::OnBnClickedButtonRdSensor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	CString strAddr;

	GetDlgItemText(IDC_EDIT_SENSOR_REGISTER_ADDR, strAddr);

	unsigned short iAddr = str2hex(strAddr);	
	unsigned short irxval = h_cctapi->RdSensorReg(iAddr);
	CString s_temp;
	s_temp.Format(_T("%04x"), irxval);
	SetDlgItemText(IDC_EDIT_SENSOR_REGISTER_VALUE, s_temp);
	SetDlgItemText(IDC_STATIC_TEXT, L"��ȡSensor�Ĵ����ɹ���");
}


void CUsbControlDlg::OnBnClickedButtonWrEeprom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	CString strAddr, strValue;

	GetDlgItemText(IDC_EDIT_FPGA_REGISTER_ADDR, strAddr);
	GetDlgItemText(IDC_EDIT_FPGA_REGISTER_VALUE, strValue);

	WORD iAddr = str2hex(strAddr);
	WORD iValue = str2hex(strValue);

	h_cctapi->WrEEPROM(iAddr, iValue);
	SetDlgItemText(IDC_STATIC_TEXT, L"дEEPROM��");
}


void CUsbControlDlg::OnBnClickedButtonRdEeprom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	CString strAddr;
	GetDlgItemText(IDC_EDIT_FPGA_REGISTER_ADDR, strAddr);
	WORD iAddr = str2hex(strAddr);
	unsigned char irxval;
	int len=1;
	h_cctapi->RdEEPROM(iAddr,&irxval,len);
	CString s_temp;
	s_temp.Format(_T("%02x"), irxval);
	SetDlgItemText(IDC_EDIT_FPGA_REGISTER_VALUE, s_temp);
	SetDlgItemText(IDC_STATIC_TEXT, L"��EEPROM��");
}


void CUsbControlDlg::OnBnClickedButtonInitSensor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	h_cctapi->InitSensor();
	h_cctapi2->InitSensor();
	h_cctapi->setResolution(0);
	h_cctapi2->setResolution(0);
	SetDlgItemText(IDC_STATIC_TEXT, L"��ʼ��sensor�ɹ���");
}

void CUsbControlDlg::OnBnClickedRadio1()//752*480
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	h_cctapi->setResolution(0);
	h_cctapi2->setResolution(0);
	g_byteResolutionType = RESOLUTION_752_480;
	g_width = 752;
	g_height = 480;
	SetDlgItemText(IDC_STATIC_TEXT, L"�ֱ���752*480��");
}


void CUsbControlDlg::OnBnClickedRadio2()//640*480
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	h_cctapi->setResolution(1);
	h_cctapi2->setResolution(1);
	g_byteResolutionType = RESOLUTION_640_480;
	g_width = 640 ;
	g_height = 480;
	SetDlgItemText(IDC_STATIC_TEXT, L"�ֱ���640*480��");
}

void CUsbControlDlg::OnBnClickedRadioNormal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	h_cctapi->setNormal();
}


void CUsbControlDlg::OnBnClickedRadioXmirror()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	h_cctapi->setXmirror();
}


void CUsbControlDlg::OnBnClickedRadioYmirror()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	h_cctapi->setYmirror();
}


void CUsbControlDlg::OnBnClickedRadioXymirror()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}

	h_cctapi->setXYmirror();
}


void CUsbControlDlg::OnBnClickedButtonOpenUsb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(h_cctapi==NULL)
	h_cctapi=new CCCTAPIAppUSB();
	
	if (h_cctapi->OpenUsb()<0)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"��USBʧ�ܡ�");
		return;
	}
	m_bUsbOpen = true;
	if(h_cctapi2==NULL)
	h_cctapi2=new CCCTAPIAppUSB(this);
	if (h_cctapi2->OpenUsb(1)<0)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"��USB1ʧ�ܡ�");
		return;
	}
	
	SetDlgItemText(IDC_STATIC_TEXT, L"��USB�ɹ���");



}


void CUsbControlDlg::OnBnClickedButtonCloseUsb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bUsbOpen = false;
	OnBnClickedBtnStopcapture();
	h_cctapi->CloseUsb();
	h_cctapi2->CloseUsb();
	delete h_cctapi;
	delete h_cctapi2;
}


void CUsbControlDlg::OnBnClickedCheckSaveVedio()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CUsbControlDlg::OnBnClickedRadioAutoTrig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}

	h_cctapi->SetTrigMode(TgMd_Auto);

	GetDlgItem(IDC_EDITHwTrigFreq)->EnableWindow(false);
	SetDlgItemText(IDC_STATIC_TEXT, L"�Զ�������");
}


void CUsbControlDlg::OnBnClickedRadioFpgaTrig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	
	GetDlgItem(IDC_EDITHwTrigFreq)->EnableWindow(true);

	CString s_temp;
	UpdateData(true);
	eFpgaFreq.GetWindowText(s_temp);
	unsigned char fpgafreq = _tstoi(s_temp);
	if (/*cbTrigMode.GetCurSel() == 2 &&*/ fpgafreq <= 0)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"Need Fpga Freq");
		return;
	}

	h_cctapi->SetTrigMode(TgMd_FPGA);
	SetDlgItemText(IDC_STATIC_TEXT, L"FPGA������");


	
}


void CUsbControlDlg::OnBnClickedButtonUsbSpeedCkeck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}

	CCyUSBDevice* pUsbDev = new CCyUSBDevice(NULL);

	if (pUsbDev->bHighSpeed == false)
	{
		SetDlgItemText(IDC_STATIC_TEXT2, L"USB3.0");
		h_cctapi->SendUsbSpeed2Fpga(0x0);
	}
		
	else
	{
		SetDlgItemText(IDC_STATIC_TEXT2, L"USB2.0");
		h_cctapi->SendUsbSpeed2Fpga(0x1);
	}
		
}



void CUsbControlDlg::OnBnClickedRadioSoftTrig()
{
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	// TODO: Add your control notification handler code here
	h_cctapi->SetTrigMode(TgMd_Soft);
	SetDlgItemText(IDC_STATIC_TEXT, L"���������");
}


void CUsbControlDlg::OnBnClickedRadioExtTrig()
{
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	// TODO: Add your control notification handler code here
	h_cctapi->SetTrigMode(TgMd_OutSig);
	SetDlgItemText(IDC_STATIC_TEXT, L"�ⲿ������");
}





void CUsbControlDlg::OnBnClickedBtnSofttrig()
{
	// TODO: Add your control notification handler code here
	h_cctapi->softTrigOnce();
}

void CUsbControlDlg::OnBnClickedButtonRdId()
{
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	CString strValue;
	unsigned char iValue = 0;
	int len=1;
	h_cctapi->RdDeviceID(&iValue,len);
	strValue.Format(_T("%02x"), iValue);
	SetDlgItemText(IDC_EDIT_DEVID, strValue);
	SetDlgItemText(IDC_STATIC_TEXT, L"��ID�ɹ���");
}
void CUsbControlDlg::OnBnClickedButtonWrId()
{
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	CString strValue;
	GetDlgItemText(IDC_EDIT_DEVID, strValue);
	unsigned char iValue = str2hex(strValue);
	int len=1;
	h_cctapi->WrDeviceID(&iValue,len);
	SetDlgItemText(IDC_STATIC_TEXT, L"����ID�ɹ���");
}


void CUsbControlDlg::OnBnClickedButtonRdSn()
{
	// TODO: Add your control notification handler code here
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	CString strValue;
	unsigned char iValue[4] = {0};
	int len=4;
	h_cctapi->RdDeviceSN(iValue,len);
	int temp=iValue[0]<<8*0;
	temp+=iValue[1]<<8*1;
	temp+=iValue[2]<<8*2;
	temp+=iValue[3]<<8*3;
	strValue.Format(_T("%02x"), temp);
	SetDlgItemText(IDC_EDIT_SN, strValue);
	SetDlgItemText(IDC_STATIC_TEXT, L"��SN�ɹ���");
}


void CUsbControlDlg::OnBnClickedButtonWrSn()
{
	// TODO: Add your control notification handler code here
	if (!m_bUsbOpen)
	{
		SetDlgItemText(IDC_STATIC_TEXT, L"USBδ�򿪡�");
		return;
	}
	CString strValue;
	GetDlgItemText(IDC_EDIT_SN, strValue);
	int temp = str2hex(strValue);
	unsigned char iValue[4]={0};
	iValue[0]=(temp>>8*0)&0xff;
	iValue[1]=(temp>>8*1)&0xff;
	iValue[2]=(temp>>8*2)&0xff;
	iValue[3]=(temp>>8*3)&0xff;
	int len=4;
	h_cctapi->WrDeviceSN(iValue,len);
	SetDlgItemText(IDC_STATIC_TEXT, L"����SN�ɹ���");
}
