#include "login.h"
#include "utils.h"

login::login() :
	dialog(nullptr),
	btn_next(nullptr), 
	txt_cardid(nullptr),
	lbl_status(nullptr) { }

void login::init(CDialog * pdialog) {
	dialog = get_item<CDialog>(pdialog, DLG_CARD_ID);
	btn_next = get_item<CButton>(pdialog, BTN_NEXT);
	txt_cardid = get_item<CEdit>(pdialog, TXT_INPUT);
	lbl_status = get_item<CStatic>(pdialog, LBL_STATUS);

}
void login::btn_next_click() {
	CString cpin;
	txt_cardid->GetWindowTextW(cpin);
	std::wstring pin(cpin);
	lbl_status->SetWindowTextW(pin.c_str());
	btn_next->ShowWindow(SW_HIDE);
}
