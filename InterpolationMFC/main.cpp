//#include "Interpolation.h"
//#include <crtdbg.h> 
//#include <iostream>
//#define originRaw "07_gLenna_512_512.raw"
//#define downRaw "myLenna_128_128.raw"
//
//#ifndef _DEBUG
//#define new new(_CLIENT_BLOCK,__FILE__,__LINE)
//#endif
//
//int main(){
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	ush Width = 128;//origin size
//	uch Scale = 4;//scale
//	ush ExtWidth = Width*Scale;//extension size
//	
//	Image downsize(ExtWidth, Width, originRaw,0,0);//make down size image
//	downsize.writeFile(downRaw);//make file: myLenna_128_128.raw
//
//	Interpolation bilinear(Width, ExtWidth, downRaw,1, Scale);
//	Interpolation largrange(Width, ExtWidth, downRaw, 1, Scale);
//	Interpolation sixtap(Width, ExtWidth, downRaw, 1, Scale);
//
//	bilinear.Bilinear_f();//interpolation		
//	largrange.Lagrange_f();	
//	sixtap.Sixtap_f();
//
//	bilinear.writeFile("Bilinear_Lena_512_512.raw");//result file
//	largrange.writeFile("Largrange_Lena_512_512.raw");
//	sixtap.writeFile("Sixtap_Lena_512_512.raw");
//
//	bilinear.setError(downsize.getBuf());//get RMS, PSNR
//	largrange.setError(downsize.getBuf());
//	sixtap.setError(downsize.getBuf());
//
//	std::cout << "Bilinear-RMS\t" << bilinear.getRMSE() << "\t ,PSNR\t" << bilinear.getPSNR() <<std::endl;
//	std::cout << "Lagrange-RMS\t" <<largrange.getRMSE() << "\t ,PSNR\t" << largrange.getPSNR() <<std::endl;
//	std::cout << "SixTap-RMS\t" << sixtap.getRMSE() << "\t ,PSNR\t" << sixtap.getPSNR() <<std::endl;
//	return 0;
//}
//
