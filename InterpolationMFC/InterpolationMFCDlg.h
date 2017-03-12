
// InterpolationMFCDlg.h : ��� ����
//
#include "Image.h"
#include "Interpolation.h"
#pragma once


// CInterpolationMFCDlg ��ȭ ����

class CInterpolationMFCDlg : public CDialogEx
{
private:
	CImage m_Image;
	uch* p_src_pos= nullptr;
	uch* p_dest_pos=nullptr;
	void makeBitMap(uch * p_dest_image, uch ** p_src_image);
	
// �����Դϴ�.
public:	
	CInterpolationMFCDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTERPOLATIONMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
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

	// ������ �޽��� �� �Լ�
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
