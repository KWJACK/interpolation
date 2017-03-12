// InterpolationMFCDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "InterpolationMFC.h"
#include "InterpolationMFCDlg.h"
#include "afxdialogex.h"
#include "Image.h"
#include "Interpolation.h"

#define originRaw "07_gLenna_512_512.raw"
#define downRaw "myLenna_128_128.raw"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInterpolationMFCDlg ��ȭ ����
void CInterpolationMFCDlg::makeBitMap(uch *p_dest_image, uch ** p_src_image)
{
	uch* temp = new uch[ExtWidth * ExtWidth];
	for (int i = 0; i < ExtWidth; i++) {
		memcpy(temp + i * ExtWidth, p_src_image[i], ExtWidth);
	}
	p_dest_pos = p_dest_image;
	p_src_pos = temp;
	for (int i = 0; i < ExtWidth * ExtWidth; i++) {//8->32bit ��ȯ
		//RGB���� �����ϰ� �����Ͽ� ��鿵������
		*p_dest_pos++ = *p_src_pos;
		*p_dest_pos++ = *p_src_pos;
		*p_dest_pos++ = *p_src_pos++;
		*p_dest_pos++ = 0xFF;
	}
	//32bit �̹������� CImage��ü�� ����
	::SetBitmapBits(m_Image, ExtWidth * ExtWidth * 4, p_dest_image);
	delete[] temp;
}

CInterpolationMFCDlg::CInterpolationMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INTERPOLATIONMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInterpolationMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInterpolationMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_BN_CLICKED(IDC_bilinear, &CInterpolationMFCDlg::OnBnClickedbilinear)
	ON_BN_CLICKED(IDC_largrange, &CInterpolationMFCDlg::OnBnClickedlargrange)
	ON_BN_CLICKED(IDC_sixtap, &CInterpolationMFCDlg::OnBnClickedsixtap)
	ON_EN_CHANGE(IDC_EDIT3, &CInterpolationMFCDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CInterpolationMFCDlg::OnEnChangeEdit4)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CInterpolationMFCDlg �޽��� ó����

BOOL CInterpolationMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.	
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Image.Create(512, 512, 32);//32bit�� ����
	downsize.makeImage(ExtWidth, Width, originRaw, 0, 0);
	downsize.writeFile(downRaw);//make file: myLenna_128_128.raw
		
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CInterpolationMFCDlg::OnPaint()
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{	
		if (state == 0) {
			//default
		}
		else {
			uch *p_outImage = new uch[ExtWidth * ExtWidth * 4];
			switch (state)
			{
			case 1: {				
				makeBitMap(p_outImage, bilinear.outBuf);break;
			}
			case 2: {				
				makeBitMap(p_outImage, largrange.outBuf);break;
			}
			case 3: {			
				makeBitMap(p_outImage, sixtab.outBuf);break;
			}
			default:
				break;
			}
			m_Image.Draw(dc, 10, 10);
			delete[] p_outImage;
		}
	}
		//CDialogEx::OnPaint();
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CInterpolationMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CInterpolationMFCDlg::OnBnClickedbilinear()
{
	
	bilinear.makeImage(Width, ExtWidth, downRaw, 1, Scale);
	bilinear.Bilinear_f();//interpolation		
	bilinear.writeFile("Bilinear_Lena_512_512.raw");//result file
	bilinear.setError(downsize.getInBuf());//get RMS, PSNR
	state = 1;
	OnEnChangeEdit3();
	OnEnChangeEdit4();
	Invalidate(TRUE);
}


void CInterpolationMFCDlg::OnBnClickedlargrange()
{
	largrange.makeImage(Width, ExtWidth, downRaw, 1, Scale);
	largrange.Lagrange_f();
	largrange.writeFile("Largrange_Lena_512_512.raw");
	largrange.setError(downsize.getInBuf());
	state = 2;
	OnEnChangeEdit3();
	OnEnChangeEdit4();
	Invalidate(TRUE);
}


void CInterpolationMFCDlg::OnBnClickedsixtap()
{
	sixtab.makeImage(Width, ExtWidth, downRaw, 1, Scale);
	sixtab.Sixtab_f();
	sixtab.writeFile("Sixtab_Lena_512_512.raw");
	sixtab.setError(downsize.getInBuf());
	state = 3;
	OnEnChangeEdit3();
	OnEnChangeEdit4();
	Invalidate(TRUE);
}


void CInterpolationMFCDlg::OnEnChangeEdit3()//RMSE ���
{
	CString result;
	if (1 == state)result.Format(L"%f", bilinear.getRMSE());
	else if (2 == state)result.Format(L"%f", largrange.getRMSE());
	else result.Format(L"%f", sixtab.getRMSE());
	SetDlgItemText(IDC_EDIT3, result);
}


void CInterpolationMFCDlg::OnEnChangeEdit4()//PSNR ���
{
	CString result;
	if (1 == state)result.Format(L"%f", bilinear.getPSNR());
	else if (2 == state)result.Format(L"%f", largrange.getPSNR());
	else result.Format(L"%f", sixtab.getPSNR());
	SetDlgItemText(IDC_EDIT4, result);
	isPrint = 2;
}


void CInterpolationMFCDlg::OnDestroy()//��������� �޸𸮹�ȯ
{
	CDialogEx::OnDestroy();
	downsize.deleteInOut2D();
	bilinear.deleteInOut2D();
	largrange.deleteInOut2D();
	sixtab.deleteInOut2D();
}