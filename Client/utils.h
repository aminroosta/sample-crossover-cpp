#pragma once
#include <afxwin.h>
#include <cpprest/json.h>

template<typename CType>
void get_item(CType** pctype, CDialog* dialog, int ID) {
	*pctype = (CType*)dialog->GetDlgItem(ID);
}

concurrency::task<web::json::value> get_app_config();

// Returns a task that completes after the specified delay.
pplx::task<void> complete_after(unsigned int timeout);

/*--------------- asynchronously trys to take a picture -------------------*/
pplx::task<utility::string_t> try_take_picture();

