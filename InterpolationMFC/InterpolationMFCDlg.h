
// InterpolationMFCDlg.h : 헤더 파일
//
#include "Image.h"
#include "Interpolation.h"
#pragma once


// CInterpolationMFCDlg 대화 상자

class CInterpolationMFCDlg : public CDialogEx
{
private:
	CImage m_Image;
	uch* p_src_pos= nullptr;
	uch* p_dest_pos=nullptr;
	void makeBitMap(uch * p_dest_image, uch ** p_src_image);
	
// 생성입니다.
public:	
	CInterpolationMFCDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTERPOLATIONMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	Image downsize;
	Interpolation bilinear;
	Interpolation largrange;
	Interpolation sixtab;
	bool isPrint = 0;
	uch state = 0;//print DlgItemText
	ush Width = 128;//origin size
	uch Scale = 4;//scale
	ush ExtWidth = Width*Scale;//extension size

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbilinear();
	afx_msg void OnBnClickedlargrange();
	afx_msg void OnBnClickedsixtap();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnDestroy();
};
