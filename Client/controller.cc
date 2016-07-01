#include "controller.h"
#include "utils.h"
#include "cwnd.h"

using namespace utility;
using web::json::value;

controller::controller() :
	lst(nullptr),
	txt_input(nullptr),
	txt_confirm(nullptr),

	lbl_first(nullptr),
	lbl_second(nullptr),
	lbl_status(nullptr),
	lbl_notify(nullptr),

	btn_perv(nullptr),
	btn_next(nullptr),
	btn_balance_check(nullptr),
	btn_cash_withdraw(nullptr),
	btn_pin_change(nullptr),
	btn_mini_statement(nullptr),
	page(CARDID_PAGE) { }

void controller::init(CDialog * dialog) {
	get_item(&form, dialog, MAIN_FORM);

	get_item(&lst, dialog, LST);
	get_item(&txt_input, dialog, TXT_INPUT);
	get_item(&txt_confirm, dialog, TXT_CONFIRM);

	get_item(&lbl_first, dialog, LBL_FIRST);
	get_item(&lbl_second, dialog, LBL_SECOND);
	get_item(&lbl_status, dialog, LBL_STATUS);
	get_item(&lbl_notify, dialog, LBL_NOTIFY);

	get_item(&btn_perv, dialog, BTN_PERV);
	get_item(&btn_next, dialog, BTN_NEXT);
	get_item(&btn_balance_check, dialog, BTN_BALANCE_CHECK);
	get_item(&btn_cash_withdraw, dialog, BTN_CASH_WITHDRAW);
	get_item(&btn_pin_change, dialog, BTN_PIN_CHANGE);
	get_item(&btn_mini_statement, dialog, BTN_MINI_STATEMENT);


	cwnd(lst).hide();
	cwnd(txt_input).disable().text();
	cwnd(txt_confirm).hide();

	cwnd(lbl_first).disable().text(U("Enter the 4 digit card id: "));
	cwnd(lbl_second).disable().text();
	cwnd(lbl_status).disable().text();
	cwnd(lbl_notify).disable().text();

	cwnd(btn_next).disable().text(U("Next"));
	cwnd(btn_perv).hide();
	cwnd(btn_balance_check).hide();
	cwnd(btn_cash_withdraw).hide();
	cwnd(btn_pin_change).hide();
	cwnd(btn_mini_statement).hide();

	config = get_app_config().get();
	login_user();
}

string_t cardid;

void controller::btn_next_click() {
	if (page == CARDID_PAGE) {
			
	}
	//CString cpin;
	//txt_cardid->GetWindowTextW(cpin);
	//std::wstring pin(cpin);
	//lbl_status->SetWindowTextW(pin.c_str());
	//btn_next->ShowWindow(SW_HIDE);
}

void controller::login_user() {
	cwnd(lbl_status).enable().text(U("Logging into rest server ..."));
	repo.authorize(config[U("name")].as_string(), config[U("password")].as_string())
		.then([this](value res) {
			if (res.has_field(U("error"))) {
				cwnd(lbl_status).text(res[U("error")].to_string());
				return;
			}

			cwnd(lbl_first).enable().text(U("Enter your card id: "));
			cwnd(txt_input).enable();
			cwnd(btn_next).enable().text(U("Next"));
			cwnd(lbl_status).enable().text(U("Login successfull. Enter your cardid now."));
		});
}
