#pragma once
#include <afxwin.h>      //MFC core and standard components
#include "resource.h"    //main symbols
#include "repository.h"
#include <string>

/* different pages application goes throw */
enum pages {
	CARDID_PAGE,
	PIN_PAGE,
	MAIN_PAGE,
	BALANCE_CHECK_PAGE,
	CASH_WITHDRAW_PAGE,
	PIN_CHANGE_PAGE,
	MINI_STATEMENT_PAGE
};

struct controller {
	controller();
	void init(CDialog* dialog);

	CDialog* form;

	CEdit* lst;
	CEdit* txt_input;
	CEdit* txt_confirm;

	CStatic* lbl_first;
	CStatic* lbl_second;
	CStatic* lbl_status;
	CStatic* lbl_notify;

	CButton* btn_perv;
	CButton* btn_next;
	CButton* btn_balance_check;
	CButton* btn_cash_withdraw;
	CButton* btn_pin_change;
	CButton* btn_mini_statement;

	repository repo;
	pages page;
	/* ---------------------- events ----------------------*/
	void btn_next_click();
};