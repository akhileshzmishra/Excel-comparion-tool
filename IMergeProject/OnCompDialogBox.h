#pragma once
#include "XLEvisMsgBox.h"

class OnCompDialogBox :
	public CXLEvisMsgBox
{
public:
	OnCompDialogBox(CWnd* pParent);
	virtual ~OnCompDialogBox(void);

	void t_Run();
};
