#include "controller.h"

struct state {
	state() : _visible(false), _active(true) { }

	state& active(bool active = true) {
		_visible = true;
		_active = active;
		return *this;
	}
	state& visible(bool visible = true) {
		_active = true;
		_visible = visible;
		return *this;
	}


	/* helper class for be get a better flow of function calls.
	 * exampe: state(true, false).apply_to(cwnd).text(U("Next");  */
	struct state_helper {
		state_helper(CWnd* cwnd, state* state) : _cwnd(cwnd), _state(state) { }
		state& text(const utility::string_t& text = U("")) {
			_cwnd->SetWindowTextW(text.c_str());
			return *_state;
		}
		CWnd* _cwnd; state* _state;
	};

	state_helper apply_to(CWnd* wnd) {
		wnd->EnableWindow(_active);
		wnd->ShowWindow(_visible ? SW_SHOW : SW_HIDE);
		return state_helper(wnd, this);
	}
private:
	bool _visible;
	bool _active;
};
