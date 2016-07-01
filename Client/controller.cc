#include "controller.h"
#include "utils.h"

controller::controller() :
	form(nullptr),
	btn_next(nullptr), 
	txt_cardid(nullptr),
	lbl_status(nullptr) { }

void controller::init(CDialog * dialog) {
	form = get_item<CDialog>(dialog, MAIN_FORM);
	btn_next = get_item<CButton>(dialog, BTN_NEXT);
	txt_cardid = get_item<CEdit>(dialog, TXT_INPUT);
	lbl_status = get_item<CStatic>(dialog, LBL_STATUS);

}
void controller::btn_next_click() {
	CString cpin;
	txt_cardid->GetWindowTextW(cpin);
	std::wstring pin(cpin);
	lbl_status->SetWindowTextW(pin.c_str());
	btn_next->ShowWindow(SW_HIDE);
}
