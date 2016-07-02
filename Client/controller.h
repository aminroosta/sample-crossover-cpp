#pragma once
#include <afxwin.h>      //MFC core and standard components
#include <afxcmn.h>	
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

	CListCtrl* lst;
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
	web::json::value config;
	web::json::value state;
	/* ---------------------- events ----------------------*/
	void btn_next_click();
	void btn_perv_click();

	/* helper function for main menu buttons */
	pplx::task<web::json::value> mainmenu_helper(std::function<pplx::task<web::json::value>(void)> getter);

	void btn_balance_check_click();
	void btn_cash_withdraw_click();
	void btn_pin_change_click();
	void btn_mini_statement_click();

	void login_user();
	void card_set();
	void cash_withdraw(int amount);
	void pin_change(utility::string_t oldpin, utility::string_t newpin);
};