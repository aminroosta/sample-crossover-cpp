#pragma once
#include <afxwin.h>
#include <cpprest/json.h>

template<typename CType>
void get_item(CType** pctype, CDialog* dialog, int ID) {
	*pctype = (CType*)dialog->GetDlgItem(ID);
}

concurrency::task<web::json::value> get_app_config();
