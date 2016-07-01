#include "controller.h"

/* helper class to manage state of Cwnd objects */
struct cwnd {
	cwnd(CWnd* _cwnd) : wnd(_cwnd) { }

	cwnd& active(bool active = true) {
		wnd->EnableWindow(active);
		wnd->ShowWindow(SW_SHOW);
		return *this;
	}

	cwnd& visible(bool visible = true) {
		wnd->EnableWindow(true);
		wnd->ShowWindow(visible ? SW_SHOW : SW_HIDE);
		return *this;
	}

	cwnd& hide() { return this->visible(false); }
	cwnd& show() { return this->visible(true); }
	cwnd& disable() { return this->active(false); }
	cwnd& enable() { return this->active(true); }

	cwnd& text(const utility::string_t& text = U("")) {
		wnd->SetWindowTextW(text.c_str());
		return *this;
	}

	utility::string_t gettext() {
		CString cpin;
		wnd->GetWindowTextW(cpin);
		CT2A wcpin(cpin);
		return utility::conversions::to_string_t((LPSTR)wcpin);
	}

private:
	CWnd* wnd;
};
