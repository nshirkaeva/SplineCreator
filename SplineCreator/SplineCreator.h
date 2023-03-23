// SplineCreator.h : main header file for the SplineCreator DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSplineCreatorApp
// See SplineCreator.cpp for the implementation of this class
//

class CSplineCreatorApp : public CWinApp
{
public:
	CSplineCreatorApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
