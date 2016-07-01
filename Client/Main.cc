#include <afxwin.h>
#include "resource.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include "controller.h"

class MainForm : public CDialog {
public:
	MainForm(CWnd* pParent = NULL) : CDialog(MAIN_FORM, pParent) {    }
protected:
	//virtual void DoDataExchange(CDataExchange* pDX) { CDialog::DoDataExchange(pDX); }
	//Called right after constructor. Initialize things here.
	controller controller;
	virtual BOOL OnInitDialog() {
		CDialog::OnInitDialog();
		controller.init(this);

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
				ctrl_next = controller.btn_next;
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
	afx_msg void btn_next_click() { controller.btn_next_click(); }
	afx_msg void btn_perv_click() { controller.btn_perv_click(); }
	afx_msg void btn_balance_check_click() { controller.btn_balance_check_click(); }
	afx_msg void btn_cash_withdraw_click() { controller.btn_cash_withdraw_click(); }
	afx_msg void btn_pin_change_click() { controller.btn_pin_change_click(); }
	afx_msg void btn_mini_statement_click() { controller.btn_mini_statement_click(); }
	/* --------------------- end event handlers --------------------*/
	DECLARE_MESSAGE_MAP()
};
//-----------------------------------------------------------------------------------------
struct MainApp : public CWinApp {
	MainApp() {  }
	virtual int InitInstance() {
		CWinApp::InitInstance();
		MainForm form;
		m_pMainWnd = &form;
		INT_PTR nResponse = form.DoModal();
		return false;
	}
};

/*------------------------------ message maps -----------------------------------*/
BEGIN_MESSAGE_MAP(MainForm, CDialog)
	ON_COMMAND(BTN_NEXT, btn_next_click)
	ON_COMMAND(BTN_PERV, btn_perv_click)
	ON_COMMAND(BTN_BALANCE_CHECK, btn_balance_check_click)
	ON_COMMAND(BTN_CASH_WITHDRAW, btn_cash_withdraw_click)
	ON_COMMAND(BTN_PIN_CHANGE, btn_pin_change_click)
	ON_COMMAND(BTN_MINI_STATEMENT, btn_mini_statement_click)
END_MESSAGE_MAP()

MainApp main_app;
