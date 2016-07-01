#include <afxwin.h>
#include "resource.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include "login.h"

class GAME_FORM : public CDialog {
public:
	GAME_FORM(CWnd* pParent = NULL) : CDialog(GAME_FORM::IDD, pParent)
	{    }
	// Dialog Data, name of dialog form
	enum { IDD = DLG_CARD_ID };
protected:
	//virtual void DoDataExchange(CDataExchange* pDX) { CDialog::DoDataExchange(pDX); }
	//Called right after constructor. Initialize things here.
	login login;
	virtual BOOL OnInitDialog() {
		CDialog::OnInitDialog();
		login.init(this);

		//try {
		//	cv::VideoCapture cap(0);
		//	if (cap.isOpened()) {
		//		cv::Mat frame;
		//		cap >> frame;
		//		cv::imwrite("myimg.jpg", frame);
		//		cap.release();
		//	}
		//}
		//catch (std::exception& e) {
		//	auto msg = e.what();
		//}
		return true;
	}
	
	/* handle enter key presses in CEdit controlls */
	virtual void OnOK() {
		CWnd* ctrl_cur = GetFocus();
		CWnd* ctrl_next = ctrl_cur;
		int ctrl_ID = ctrl_cur->GetDlgCtrlID();

		switch (ctrl_ID) {
			case TXT_INPUT:
				ctrl_next = login.btn_next;
				break;
			case IDOK:
				CDialog::OnOK();
				break;
			default:
				break;
		}
		ctrl_next->SetFocus();
	}

public:
	/* --------------------- event handlers ----------------------- */
	afx_msg void btn_next_click() { login.btn_next_click(); }
	/* --------------------- end event handlers --------------------*/
	DECLARE_MESSAGE_MAP()
};
//-----------------------------------------------------------------------------------------
struct MainApp : public CWinApp {
	MainApp() {  }
	virtual int InitInstance() {
		CWinApp::InitInstance();
		GAME_FORM dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		return false;
	}
};
//-----------------------------------------------------------------------------------------

/*------------------------------ message maps -----------------------------------*/
BEGIN_MESSAGE_MAP(GAME_FORM, CDialog)
	ON_COMMAND(BTN_NEXT, btn_next_click)
	ON_COMMAND(TXT_INPUT, btn_next_click)
END_MESSAGE_MAP()

MainApp main_app;
