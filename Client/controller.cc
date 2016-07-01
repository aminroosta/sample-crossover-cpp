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


void controller::btn_next_click() {
	if (page == CARDID_PAGE) {
		auto cardid = cwnd(txt_input).gettext();
		if (cardid.length() != 4) {
			cwnd(lbl_status).enable().text(U("Card id must be 4 digits!"));
			return;
		}
		page = PIN_PAGE;

		state[U("cardid")] = value(cardid);
		cwnd(txt_input).text();
		cwnd(btn_next).text(U("Enter"));
		cwnd(lbl_first).text(U("Enter your 4 digit pin number:"));
		cwnd(lbl_status).text(U("Enter you pin number to enter."));
		cwnd(btn_perv).enable().text(U("Back"));
		return;
	}
	if (page == PIN_PAGE) {
		auto pin = cwnd(txt_input).gettext();
		if (pin.length() != 4) {
			cwnd(lbl_status).enable().text(U("Pin number must be 4 digits!"));
			return;
		}
		state[U("pin")] = value(pin);
		cwnd(btn_next).disable();
		cwnd(btn_perv).disable();
		card_set();
		return;
	}
	if (page == CASH_WITHDRAW_PAGE) {
		auto str = cwnd(txt_confirm).gettext();
		if (str.empty() || str.find_first_not_of(U("0123456789")) != string_t::npos) {
			cwnd(lbl_status).enable().text(U("Amount must be an integer!"));
			return;
		}
		auto amount = std::stoi(str);
		cwnd(txt_confirm).text();
		cash_withdraw(amount);
		return;
	}
	if (page == PIN_CHANGE_PAGE) {
		auto oldpin = cwnd(txt_input).gettext();
		auto newpin = cwnd(txt_confirm).gettext();

		if (oldpin.length() != 4 || newpin.length() != 4) {
			cwnd(lbl_status).enable().text(U("Both pins should be 4 digits!"));
			return;
		}

		cwnd(txt_input).text();
		cwnd(txt_confirm).text();
		pin_change(oldpin, newpin);
		return;
	}
}

void controller::btn_perv_click() {
	if (page == PIN_PAGE) {
		page = CARDID_PAGE;

		cwnd(txt_input).text();
		cwnd(btn_perv).hide();
		cwnd(btn_next).enable().text(U("Next"));
		cwnd(lbl_first).enable().text(U("Enter your card id: "));
		cwnd(lbl_status).text(U("Enter you 4 digit card id to continue."));
		return;
	}
	else if (page == MAIN_PAGE) {
		cwnd(btn_balance_check).disable();
		cwnd(btn_cash_withdraw).disable();
		cwnd(btn_pin_change).disable();
		cwnd(btn_mini_statement).disable();
		cwnd(btn_perv).disable();
		cwnd(btn_next).disable();

		login_user();
	}
	else if (page == BALANCE_CHECK_PAGE || page == CASH_WITHDRAW_PAGE
		|| page == PIN_CHANGE_PAGE || page == MINI_STATEMENT_PAGE) {

		cwnd(lbl_first).hide();
		cwnd(lbl_second).hide();
		cwnd(txt_input).hide();
		cwnd(txt_confirm).hide();
		cwnd(lst).hide();
		cwnd(lbl_status).enable().text(U("Select you operation from the above menu."));
		cwnd(btn_balance_check).enable();
		cwnd(btn_cash_withdraw).enable();
		cwnd(btn_pin_change).enable();
		cwnd(btn_mini_statement).enable();
		cwnd(btn_next).hide();
		page = MAIN_PAGE;
	}
}


pplx::task<value> controller::mainmenu_helper(std::function<pplx::task<value>(void)> getter) {
	cwnd(btn_balance_check).disable();
	cwnd(btn_cash_withdraw).disable();
	cwnd(btn_pin_change).disable();
	cwnd(btn_mini_statement).disable();
	cwnd(btn_perv).disable();
	if (getter != nullptr) {
		return getter().then([this](value res) {
			if (res.has_field(U("error"))) {
				cwnd(btn_balance_check).enable();
				cwnd(btn_cash_withdraw).enable();
				cwnd(btn_pin_change).enable();
				cwnd(btn_mini_statement).enable();
				cwnd(btn_perv).enable();
				cwnd(lbl_status).enable().text(res[U("error")].to_string());

				return value::null();
			}

			cwnd(btn_balance_check).hide();
			cwnd(btn_cash_withdraw).hide();
			cwnd(btn_pin_change).hide();
			cwnd(btn_mini_statement).hide();
			cwnd(btn_perv).enable();
			return res;
		});
	}
	cwnd(btn_balance_check).hide();
	cwnd(btn_cash_withdraw).hide();
	cwnd(btn_pin_change).hide();
	cwnd(btn_mini_statement).hide();
	cwnd(btn_perv).enable();
	return pplx::task<value>([] { return value::null();  });
}

void controller::btn_balance_check_click() {
	cwnd(lbl_status).enable().text(U("Getting your balance infomation ..."));
	mainmenu_helper([this] {return repo.balance_check(); }).then([this](value res) {
		if (res.is_null()) return;
		page = BALANCE_CHECK_PAGE;

		string_t balance = U("Your balance: ");
		balance = balance + res[U("balance")].to_string();

		string_t currency = U("Currency ");
		currency = currency + res[U("currency")].to_string();

		cwnd(lbl_first).enable().text(balance);
		cwnd(lbl_second).enable().text(currency);
		cwnd(lbl_status).enable().text();
	});
}

void controller::btn_cash_withdraw_click() {
	mainmenu_helper(nullptr).then([this](value) {
		cwnd(lbl_status).enable().text(U("Amount must be less than your balance."));
		cwnd(lbl_second).enable().text(U("Enter withdrawal amount: "));
		cwnd(txt_confirm).enable().text();
		cwnd(btn_next).enable().text(U("Withdraw"));
		page = CASH_WITHDRAW_PAGE;
	});
}

void controller::btn_pin_change_click() {
	mainmenu_helper(nullptr).then([this](value) {
		cwnd(lbl_status).enable().text();
		cwnd(lbl_first).enable().text(U("Enter your current pin: "));
		cwnd(lbl_second).enable().text(U("Enter the new pin: "));
		cwnd(txt_input).enable().text();
		cwnd(txt_confirm).enable().text();
		cwnd(btn_next).enable().text(U("Change"));
		page = PIN_CHANGE_PAGE;
	});
}

void controller::btn_mini_statement_click() {
}

void controller::login_user() {
	cwnd(lbl_status).enable().text(U("Logging into rest server ..."));
	repo.authorize(config[U("name")].as_string(), config[U("password")].as_string())
		.then([this](value res) {
			if (res.has_field(U("error"))) {
				cwnd(lbl_status).text(res[U("error")].to_string());
				return;
			}

			page = CARDID_PAGE;
			cwnd(lbl_first).enable().text(U("Enter your card id: "));
			cwnd(txt_input).enable();
			cwnd(btn_next).enable().text(U("Next"));
			cwnd(lbl_status).enable().text(U("Login successfull. Enter your cardid now."));

			cwnd(btn_balance_check).hide();
			cwnd(btn_cash_withdraw).hide();
			cwnd(btn_pin_change).hide();
			cwnd(btn_mini_statement).hide();
			cwnd(btn_perv).hide();
		});
}

void controller::card_set() {
	cwnd(lbl_status).enable().text(U("Entering with your cardid and pin ..."));
	repo.set_card(state[U("cardid")].as_string(), state[U("pin")].as_string())
		.then([this](value res) {
			if (res.has_field(U("error"))) {
				cwnd(lbl_status).text(res[U("error")].to_string());
				cwnd(btn_next).enable();
				cwnd(btn_perv).enable();
				return;
			}

			page = MAIN_PAGE;
			cwnd(lbl_first).hide();
			cwnd(lbl_second).hide();
			cwnd(txt_input).hide();
			cwnd(txt_confirm).hide();
			cwnd(btn_perv).enable().text(U("Back"));
			cwnd(btn_next).hide();
			cwnd(lbl_status).enable().text(U("Select you operation from the above menu."));
			cwnd(btn_balance_check).show();
			cwnd(btn_cash_withdraw).show();
			cwnd(btn_pin_change).show();
			cwnd(btn_mini_statement).show();
		});
}

void controller::cash_withdraw(int amount) {
	cwnd(btn_next).disable();
	cwnd(lbl_status).enable().text(U("Performing the transaction ..."));
	repo.cash_withdraw(amount).then([this](value res) {
		if (res.has_field(U("error"))) {
			cwnd(lbl_status).text(res[U("error")].to_string());
			cwnd(btn_next).enable();
			return;
		}

		cwnd(lbl_notify).enable().text(U("Transaction was successfull."));
		complete_after(2000).then([this] {
			cwnd(lbl_notify).hide();
		});
		btn_perv_click();
	});
}

void controller::pin_change(utility::string_t oldpin, utility::string_t newpin) {
	cwnd(btn_next).disable();
	cwnd(lbl_status).enable().text(U("Changing your pin number ..."));
	repo.pin_change(oldpin, newpin).then([this](value res) {
		if (res.has_field(U("error"))) {
			cwnd(lbl_status).text(res[U("error")].to_string());
			cwnd(btn_next).enable();
			return;
		}

		cwnd(lbl_notify).enable().text(U("Pin number change successfull."));
		complete_after(2000).then([this] {
			cwnd(lbl_notify).hide();
		});
		btn_perv_click();
	});
}
