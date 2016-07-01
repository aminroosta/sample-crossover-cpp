#pragma once
#include <afxwin.h>      //MFC core and standard components
#include "resource.h"    //main symbols
#include <string>

struct controller {
	controller();
	void init(CDialog* dialog);

	CDialog* form;
	CButton* btn_next;
	CEdit* txt_cardid;
	CStatic* lbl_status;

	/* ---------------------- events ----------------------*/
	void btn_next_click();
};