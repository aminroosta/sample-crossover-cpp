#include "controller.h"
#include "utils.h"
#include "state.h"

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

	state().visible(false).apply_to(lst);
	state().active(false).apply_to(txt_input).text();
	state().visible(false).apply_to(txt_confirm);

	state().active(false).apply_to(lbl_first).text(U("Enter the 4 digit card id: "));
	state().active(true).apply_to(lbl_second).text();
	state().active(true).apply_to(lbl_status).text();
	state().active(true).apply_to(lbl_notify).text();
	state().active(false).apply_to(btn_next).text(U("Next"));
	state().visible(false).apply_to(btn_perv);
	state().visible(false).apply_to(btn_balance_check);
	state().visible(false).apply_to(btn_cash_withdraw);
	state().visible(false).apply_to(btn_pin_change);
	state().visible(false).apply_to(btn_mini_statement);

	config = get_app_config().get();
	login_user();
}

void controller::btn_next_click() {
	//CString cpin;
	//txt_cardid->GetWindowTextW(cpin);
	//std::wstring pin(cpin);
	//lbl_status->SetWindowTextW(pin.c_str());
	//btn_next->ShowWindow(SW_HIDE);
}

void controller::login_user() {
	state().active(true).apply_to(lbl_status).text(U("Logging into rest server ..."));
	repo.authorize(config[U("name")].as_string(), config[U("password")].as_string())
		.then([this](value res) {
			if (res.has_field(U("error"))) {
				state().active(true).apply_to(lbl_status).text(res[U("error")].to_string());
				return;
			}
			state().active(true).apply_to(lbl_first).text(U("Enter your card id: "));
			state().active(true).apply_to(txt_input);
			state().active(true).apply_to(btn_next).text(U("Next"));
			state().active(true).apply_to(lbl_status).text(U("Login was successfull"));
		});
}
