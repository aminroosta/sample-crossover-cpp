#pragma once
#include <afxwin.h>

template<typename T>
T* get_item(CDialog* dialog, int ID) {
	return (T*) dialog->GetDlgItem(ID);
}
