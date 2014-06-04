// DeepikaProject.h : main header file for the DeepikaProject application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// IMergeProjectApp:
// See DeepikaProject.cpp for the implementation of this class
//

class IMergeProjectApp : public CWinApp
{
public:
	IMergeProjectApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern IMergeProjectApp theApp;