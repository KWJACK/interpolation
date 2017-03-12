#pragma once
#include "Image.h"
class Interpolation : public Image
{
private:
	int **tempBuf;
	uch **memScale;
	double m_RMSE;
	double m_PSNR;
public:
	Interpolation();
	~Interpolation();
	double getRMSE() { return m_RMSE; }
	double getPSNR() { return m_PSNR; }
	void setError(uch **origin);//get noise ratio
	
	void allocation2D(int width);
	void delete2D(int width);
	void Bilinear_f();
	void Lagrange_f();	
	void Sixtab_f();	
	uch roundingOff(int in, int div, uch count);	
};