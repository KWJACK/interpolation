// InterpolationMFC.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "InterpolationMFC.h"
#include "InterpolationMFCDlg.h"


#ifndef _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__,__LINE)
#endif

// CInterpolationMFCApp

BEGIN_MESSAGE_MAP(CInterpolationMFCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CInterpolationMFCApp ����

CInterpolationMFCApp::CInterpolationMFCApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CInterpolationMFCApp ��ü�Դϴ�.

CInterpolationMFCApp theApp;


// CInterpolationMFCApp �ʱ�ȭ

BOOL CInterpolationMFCApp::InitInstance()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	CWinApp::InitInstance();
	CInterpolationMFCDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

