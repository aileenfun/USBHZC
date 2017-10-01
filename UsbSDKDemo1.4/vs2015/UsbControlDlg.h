
// UsbControlDlg.h : 头文件
//

#pragma once
#include "CCTAPI.h"
#include "VideoDlg.h"
#include "afxwin.h"
#include <cv.hpp>
#include <opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
// CUsbControlDlg 对话框
typedef
	VOID
	(WINAPI * LPMV_CALLBACK2)(LPVOID lpParam, LPVOID lpUser);

class CUsbControlDlg : public CDialogEx
{
// 构造
	HICON m_hIcon;
	enum { IDD = IDD_USBCONTROL_DIALOG };
	DECLARE_MESSAGE_MAP()

public:
	CUsbControlDlg(CWnd* pParent = NULL);	
	~CUsbControlDlg();
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
	virtual BOOL OnInitDialog();

private:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnOpenusb();
	afx_msg void OnBnClickedBtnOpenfile();
	afx_msg void OnBnClickedBtnVideocapture();
	afx_msg void OnBnClickedBtnStopcapture();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData );
	afx_msg void OnCbnSelchangeComboDriver();
	afx_msg void OnBnClickedRadioDriver();
	//afx_msg void OnBnClickedRadioProcType();
	afx_msg void OnBnClickedRadioChangeType();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnReview();
	static unsigned int __stdcall ThreadProcess(void* pParam);
	void ThreadProcessFunction();
	BOOL CloseRbfFile();
	BOOL OpenRbfFile();
	void SendData();
	BOOL OpenDataFile();
	BOOL CloseDataFile();

	//void _stdcall RawCallBack(LPVOID lpParam,LPVOID lpUser);
	void Split(CString in, CString *out, int &outcnt, CString spliter,int maxn);
private:
	BOOL	m_bUsbOpen;
	CFile*	m_pFileRbf;					//下位机程序文件
	CString m_strRbfFileName;
	BYTE          m_byData[64];
	USB_ORDER     m_sUsbOrder;
	BOOL		  m_bSendData;
	CVideoDlg*    m_pVideoDlg;
	HANDLE        m_hThread;
	char*         m_pReadBuff;
	BOOL		  m_bCloseWnd;
	long          m_lBytePerSecond;
	int			  m_iRdoDriver;
	int			  m_iProcType;
	CBrush*       m_pBrush;	
	CFile*        m_pVideoDataFile;
	BOOL          m_bReview;
	BOOL          m_bSave;
	BOOL          m_Init;
	CString       m_strDataFileName;
	CCCTAPIAppUSB *h_cctapi;
	CCCTAPIAppUSB *h_cctapi2;
	
	int devcnt;
public:
	CEdit m_Edit_Width;
	afx_msg void OnEnChangeEdit1();
	CEdit m_Edit_Height;
	afx_msg void OnEnChangeEdit2();
	CString m_sEdit_Width;
	CString m_sEdit_Height;
	BOOL PreTranslateMessage(MSG* pMsg);
	int test;
	int test2;
	afx_msg void OnBnClickedBnSnap();
	CComboBox cbTrigMode;
	CEdit eFpgaFreq;
	CEdit eGainValue;
	CEdit eExpoValue;
	afx_msg void setGainValue();
	CButton cbAutoGain;
	CButton cbAutoExpo;
	afx_msg void setAutoGainExpo();
	afx_msg void setExpoValue();
	afx_msg void setFpgaFreq();
	CButton m_chk_save_all;
	afx_msg void OnBnClickedCheckSaveall();
	afx_msg void OnBnClickedButtonWrSensor();
	int str2hex(CString);
	afx_msg void OnBnClickedButtonRdSensor();
	afx_msg void OnBnClickedButtonWrEeprom();
	afx_msg void OnBnClickedButtonRdEeprom();
	afx_msg void OnBnClickedButtonInitSensor();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadioNormal();
	afx_msg void OnBnClickedRadioXmirror();
	afx_msg void OnBnClickedRadioYmirror();
	afx_msg void OnBnClickedRadioXymirror();
	afx_msg void OnBnClickedButtonOpenUsb();
	afx_msg void OnBnClickedButtonCloseUsb();
	afx_msg void OnBnClickedCheckSaveVedio();
	afx_msg void OnBnClickedRadioAutoTrig();
	afx_msg void OnBnClickedRadioFpgaTrig();
	afx_msg void OnBnClickedButtonUsbSpeedCkeck();
	afx_msg void OnBnClickedRadioSoftTrig();
	afx_msg void OnBnClickedRadioExtTrig();
	afx_msg void OnBnClickedButtonRdId();
	afx_msg void OnBnClickedBtnSofttrig();
	afx_msg void OnBnClickedButtonWrId();
	afx_msg void OnBnClickedButtonRdSn();
	afx_msg void OnBnClickedButtonWrSn();

};
