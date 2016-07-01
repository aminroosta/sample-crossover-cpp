#pragma once
#include <afxwin.h>      //MFC core and standard components
#include "resource.h"    //main symbols
#include <string>

struct login {
	login();
	void init(CDialog* dialog);

	CDialog* dialog;
	CButton* btn_next;
	CEdit* txt_cardid;
	CStatic* lbl_status;

	/* ---------------------- events ----------------------*/
	void btn_next_click();
};