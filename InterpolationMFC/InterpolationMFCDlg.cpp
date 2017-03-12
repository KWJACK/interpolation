// InterpolationMFCDlg.cpp : 구현 파일
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


// CInterpolationMFCDlg 대화 상자
void CInterpolationMFCDlg::makeBitMap(uch *p_dest_image, uch ** p_src_image)
{
	uch* temp = new uch[ExtWidth * ExtWidth];
	for (int i = 0; i < ExtWidth; i++) {
		memcpy(temp + i * ExtWidth, p_src_image[i], ExtWidth);
	}
	p_dest_pos = p_dest_image;
	p_src_pos = temp;
	for (int i = 0; i < ExtWidth * ExtWidth; i++) {//8->32bit 전환
		//RGB값을 동일하게 복사하여 흑백영상으로
		*p_dest_pos++ = *p_src_pos;
		*p_dest_pos++ = *p_src_pos;
		*p_dest_pos++ = *p_src_pos++;
		*p_dest_pos++ = 0xFF;
	}
	//32bit 이미지값을 CImage객체에 적용
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


// CInterpolationMFCDlg 메시지 처리기

BOOL CInterpolationMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.	
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_Image.Create(512, 512, 32);//32bit맵 생성
	downsize.makeImage(ExtWidth, Width, originRaw, 0, 0);
	downsize.writeFile(downRaw);//make file: myLenna_128_128.raw
		
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CInterpolationMFCDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 아이콘을 그립니다.
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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


void CInterpolationMFCDlg::OnEnChangeEdit3()//RMSE 출력
{
	CString result;
	if (1 == state)result.Format(L"%f", bilinear.getRMSE());
	else if (2 == state)result.Format(L"%f", largrange.getRMSE());
	else result.Format(L"%f", sixtab.getRMSE());
	SetDlgItemText(IDC_EDIT3, result);
}


void CInterpolationMFCDlg::OnEnChangeEdit4()//PSNR 출력
{
	CString result;
	if (1 == state)result.Format(L"%f", bilinear.getPSNR());
	else if (2 == state)result.Format(L"%f", largrange.getPSNR());
	else result.Format(L"%f", sixtab.getPSNR());
	SetDlgItemText(IDC_EDIT4, result);
	isPrint = 2;
}


void CInterpolationMFCDlg::OnDestroy()//강제종료시 메모리반환
{
	CDialogEx::OnDestroy();
	downsize.deleteInOut2D();
	bilinear.deleteInOut2D();
	largrange.deleteInOut2D();
	sixtab.deleteInOut2D();
}