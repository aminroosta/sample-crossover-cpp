#include <afxwin.h>      //MFC core and standard components
#include "resource.h"    //main symbols
#include <opencv2/opencv.hpp>

//Globals
//CEdit * TEST;
class GAME_FORM : public CDialog
{
public:
	GAME_FORM(CWnd* pParent = NULL) : CDialog(GAME_FORM::IDD, pParent)
	{    }
	// Dialog Data, name of dialog form
	enum { IDD = DLG_LOGIN };
protected:
	virtual void DoDataExchange(CDataExchange* pDX) { CDialog::DoDataExchange(pDX); }
	//Called right after constructor. Initialize things here.
	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();

		cv::VideoCapture cap(0);
		if (cap.isOpened()) {
			cv::Mat frame;
			cap >> frame;
			cv::imwrite("frame.jpg", frame);
			cap.release();
		}
		//TEST = (CEdit *)GetDlgItem(IDC_TEST);
		//TEST->SetWindowText(L"Hello!");
		return true;
	}
public:
	DECLARE_MESSAGE_MAP()
};
//-----------------------------------------------------------------------------------------
class TheGame : public CWinApp
{
public:
	TheGame() {  }
public:
	virtual BOOL InitInstance()
	{
		CWinApp::InitInstance();
		GAME_FORM dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		return FALSE;
	} //close function
};
//-----------------------------------------------------------------------------------------

//Need a Message Map Macro for both CDialog and CWinApp
BEGIN_MESSAGE_MAP(GAME_FORM, CDialog)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------------------
TheGame theApp;  //Starts the Application