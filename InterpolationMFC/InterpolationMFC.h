
// InterpolationMFC.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CInterpolationMFCApp:
// �� Ŭ������ ������ ���ؼ��� InterpolationMFC.cpp�� �����Ͻʽÿ�.
//

class CInterpolationMFCApp : public CWinApp
{
public:
	CInterpolationMFCApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CInterpolationMFCApp theApp;