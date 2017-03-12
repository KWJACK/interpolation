// InterpolationMFC.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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


// CInterpolationMFCApp 생성

CInterpolationMFCApp::CInterpolationMFCApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CInterpolationMFCApp 개체입니다.

CInterpolationMFCApp theApp;


// CInterpolationMFCApp 초기화

BOOL CInterpolationMFCApp::InitInstance()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	CWinApp::InitInstance();
	CInterpolationMFCDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

