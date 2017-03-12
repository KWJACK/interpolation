#include "stdafx.h"
#include "Interpolation.h"
#include <stdlib.h>
#include <math.h> // floor
#include <string.h>
#define scaled_1 1
#define scaled_2 2
Interpolation::Interpolation(){}

Interpolation::~Interpolation(){}

void Interpolation::allocation2D(int width)
{
	tempBuf = new int*[width];
	memScale = new uch*[width];
	for (int i = 0; i < width; i++) {
		*(tempBuf + i) = new int[width]();//initailize
		*(memScale + i) = new uch[width]();//initailize
	}	
}
void Interpolation::delete2D(int width)
{
	if (outBuf != nullptr) {
		for (int i = 0; i < width; i++) {
			delete[] * (tempBuf + i);
			delete[] * (memScale + i);
		}
		delete tempBuf;
		delete memScale;
	}
}
uch Interpolation::roundingOff(int in, int div, uch count)
{
	double value = (double)in;
	double divideValue = (double)div;
	if (3 == count) {//3 is option for six-tap
		value = (double)in / 2.0;
		count = 2;//this reason related to div 24
	}
	for (int i = 0; i<count; i++)value /= (double)divideValue;
	if (value > 255.0)return (uch)255;	
	else return (uch)floor(value + 0.5);
}
void Interpolation::setError(uch ** origin)
{//http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche61.html	
	double MSE=0.0, HxW, temp;//height*width
	HxW = (double)m_ExtWidth*(double)m_ExtWidth;
	for (int h = 0; h<m_ExtWidth; h++) {
		for (int w = 0; w<m_ExtWidth; w++) {
			temp = (double)outBuf[h][w] - (double)origin[h][w];
			MSE += (temp*temp);
		}
	}
	MSE /= HxW;
	m_RMSE = (double)sqrt(MSE);
	m_PSNR = 10.0 * (log10((255 * 255) / MSE));	//255=max 
}
void Interpolation::Bilinear_f()
{
	ush virtualBoundary = m_ExtWidth - m_Scale;
	allocation2D(m_ExtWidth);
	for (int h = 0; h<m_Width; h++) {//origin buf
		for (int w = 0; w<m_Width; w++) {
			tempBuf[h*m_Scale][w*m_Scale]= (ush)inBuf[h][w];			
		}
	}
	//row interpolation
	for (int h = 0; h<m_ExtWidth; h += m_Scale) {//h = height, w= width
		for (int w = 0; w<m_ExtWidth; w += m_Scale) {
			if (w < virtualBoundary) {//til boundary
				for (int k = 1; k < m_Scale; k++)
				{// ( (scaled -k)Y  - kX ) / scaled num
					tempBuf[h][w + k]
						= ((m_Scale - k)*tempBuf[h][w]) + (k*tempBuf[h][w + m_Scale]);
					memScale[h][w + k] = scaled_1;
				}
			}
			tempBuf[h][w] *= m_Scale;//scaling each value
			memScale[h][w] = scaled_1;
		}
	}
	//colum interpolation
	for (int w = 0; w<virtualBoundary + 1; w++) {
		for (int h = 0; h<virtualBoundary; h += m_Scale) {
			for (int k = 1; k < m_Scale; k++)
			{
				tempBuf[h + k][w]
					= ((m_Scale - k)*tempBuf[h][w]) + (k*tempBuf[h+ m_Scale][w]);
				memScale[h + k][w] = scaled_2;//
			}			
		}
	}
	//rounding --> (value / scalednum +mid)-> original
	for (int h = 0; h<virtualBoundary + 1; h++) {//til 508
		for (int w = 0; w<virtualBoundary + 1; w++) {
			outBuf[h][w] = roundingOff(tempBuf[h][w], m_Scale, memScale[h][w]);
		}
	}
	//boundary processing
	for (int w = virtualBoundary + 1; w<m_ExtWidth; w++){
		for (int h = 0; h<virtualBoundary + 1; h++) {
			outBuf[h][w] = outBuf[h][virtualBoundary];//right
			outBuf[w][h] = outBuf[virtualBoundary][h];//bottom
		 }
	}	
	for (int h = virtualBoundary + 1; h<m_ExtWidth; h++) {//509,510,511
		for (int w = virtualBoundary + 1; w<m_ExtWidth; w++) {//509,510,511
			outBuf[h][w] = inBuf[m_Width - 1][m_Width - 1];//copy last array in inbuf to three
		}
	}
	delete2D(m_ExtWidth);
}

void Interpolation::Lagrange_f()
{	
	int padWidth = m_Scale + m_ExtWidth + m_Scale;//scale(4)+extwidth(512)+scale(4)
	allocation2D(padWidth);//tempbuf, memscale
	//original value in range of virtualBound
	for (int h = 0; h < m_Width; h++) {//copy original
		for (int w = 0; w < m_Width; w++) {
			tempBuf[m_Scale*(h+1)][m_Scale*(w+1)] = (int)inBuf[h][w];
		}
	}	
	int virtualBound = padWidth - (m_Scale * 2);//512--for calculating
	int realBound = padWidth - m_Scale;//516-->outbuf's boundary

	for (int k = m_Scale; k < padWidth; k += m_Scale) {//padding virtual->real range
		tempBuf[0][k] = tempBuf[m_Scale][k];//N-bound
		tempBuf[k][0] = tempBuf[k][m_Scale];//W-bound
		tempBuf[realBound][k] = tempBuf[virtualBound][k];//S-bound
		tempBuf[k][realBound] = tempBuf[k][virtualBound];//E-bound
	}
	//row interpolation
	for (int h = m_Scale; h < realBound + 1; h+=m_Scale) {
		for (int w = m_Scale; w < virtualBound; w += m_Scale) {
			for (int a = 1, k = 5; a < m_Scale; a++, k++)
			{
				tempBuf[h][w + a]
					= tempBuf[h][w - m_Scale] * (k - 4) * (k - 8) * (k - 12) * -1
					+ tempBuf[h][w			] * (k) * (k - 8) * (k - 12) * 3
					+ tempBuf[h][w + m_Scale] * (k) * (k - 4) * (k - 12) * -3
					+ tempBuf[h][w + m_Scale * 2] * (k) * (k - 4) * (k - 8) * 1;
				memScale[h][w + a] = scaled_1;//two time scaled
			}
		}
		for (int a = 1, k = 9; a < m_Scale; a++, k++)//boundary processing
		{
			tempBuf[h][virtualBound + a]
				= tempBuf[h][virtualBound - (m_Scale * 2)] * (k - 4) * (k - 8) * (k - 12) * -1
				+ tempBuf[h][virtualBound - m_Scale] * (k) * (k - 8) * (k - 12) * 3
				+ tempBuf[h][virtualBound		   ] * (k) * (k - 4) * (k - 12) * -3
				+ tempBuf[h][virtualBound + m_Scale] * (k) * (k - 4) * (k - 8) * 1;
			memScale[h][virtualBound + a] = scaled_1;//two time scaled
		}
	}	
	//scaling(not include 
	int div = 6 * m_Scale*m_Scale*m_Scale;//(0-4)(0-8)(0-12)-> 6*4*4*4
	for (int h = 0; h < padWidth; h += m_Scale) {//all per 4 dot
		for (int w = 0; w < padWidth; w += m_Scale) {
			tempBuf[h][w] *= div;//scaling factor
			memScale[h][w] = scaled_1;
		}
	}
	//column interpolation
	for (int h = m_Scale; h < virtualBound; h += m_Scale) {//4<= h <512
		for (int w = m_Scale; w <= realBound; w ++) {//4<= w <=516			
			for (int a = 1, k = 5; a <m_Scale; a++, k++)
			{//k= 5 6 7
				tempBuf[h + a][w]
					= tempBuf[h - m_Scale][w] * (k - 4) * (k - 8) * (k - 12) * -1//0
					+ tempBuf[h			 ][w] * (k) * (k - 8) * (k - 12) * 3//4
					+ tempBuf[h + m_Scale][w] * (k) * (k - 4) * (k - 12) * -3//8
					+ tempBuf[h + (m_Scale * 2)][w] * (k) * (k - 4) * (k - 8) * 1;//12
				memScale[h + a][w] = scaled_2;
			}
			for (int a = 1, k = 9; a < m_Scale; a++, k++)//real boundary processing
			{//k=9 10 11
				tempBuf[virtualBound + a][w]
					= tempBuf[virtualBound - (m_Scale * 2)][w] * (k - 4) * (k - 8) * (k - 12) * -1
					+ tempBuf[virtualBound - m_Scale][w] * (k) * (k - 8) * (k - 12) * 3
					+ tempBuf[virtualBound			][w] * (k) * (k - 4) * (k - 12) * -3
					+ tempBuf[virtualBound + m_Scale][w] * (k) * (k - 4) * (k - 8) * 1;
				memScale[virtualBound + a][w] = scaled_2;
			}
		}
	}
	//round-off 	
	for (int y = 0, h = m_Scale; y < m_ExtWidth; y++, h++) {//0~511
		for (int x = 0, w = m_Scale; x < m_ExtWidth; x++, w++) {
			outBuf[y][x] = roundingOff(tempBuf[h][w], div, memScale[h][w]);
		}
	}
	delete2D(padWidth);
}

void Interpolation::Sixtab_f()
{
	int sixTab[6] = { 1, -5, 20, 20, -5, 1 };	
	int padWidth =m_ExtWidth + (m_Scale * 2 * 2);//8 + 512 + 8
	allocation2D(padWidth);
	//origin file copy to 0(8) ~512(520) range except padding area
	for (int h = 0; h<m_Width; h++) {
		for (int w = 0; w<m_Width; w++) {
			tempBuf[m_Scale*(2 + h)][m_Scale*(2 + w)] = (int)inBuf[h][w];
		}
	}
	int virtualBoundUP = padWidth - (m_Scale * 3);
	int virtualBound = padWidth - (m_Scale * 2);
	int realBound = padWidth - m_Scale;
	//padding-diagonal
	for (int h = 0; h<m_Scale+1; h += m_Scale) {
		for (int w = 0; w<m_Scale+1; w += m_Scale) {
			tempBuf[h				][w				  ] = (int)inBuf[0			][0			 ];//left up diagonal
			tempBuf[h				][virtualBound + w] = (int)inBuf[0			][m_Width - 1];//right up diagonal
			tempBuf[virtualBound + h][w				  ] = (int)inBuf[m_Width - 1][0			 ];//left down diagonal
			tempBuf[virtualBound + h][virtualBound + w] = (int)inBuf[m_Width - 1][m_Width - 1];//right down diagonal
		}
	}
	//padding side
	for (int h = 0; h<m_Scale+1; h += m_Scale) {
		for (int w = m_Scale * 2; w<virtualBound; w += m_Scale) {
			tempBuf[h				][w] = (int)tempBuf[2 * m_Scale   ][w];//N-boundary
			tempBuf[virtualBound + h][w] = (int)tempBuf[virtualBoundUP][w];//S-boundary
			tempBuf[w][h			   ] = (int)tempBuf[w][2 * m_Scale   ];//W-boundary
			tempBuf[w][virtualBound + h] = (int)tempBuf[w][virtualBoundUP];//E-boundary
		}
	}//padding complete

	//half-pel column interplolation
	for (int h = 2 * m_Scale; h<virtualBound; h += m_Scale) {//origin 512, except padding area
		for (int w = 0; w<padWidth; w += m_Scale) {
			int sum = 0;
			for (int k = 0; k<6; k++)//6tap
			{	
				if(h + (k - 2)*(int)m_Scale ==528)sum += sixTab[k] * tempBuf[527][w];
				else sum += sixTab[k] * tempBuf[h + (k - 2)*(int)m_Scale][w];
			}
			tempBuf[h + (m_Scale / 2)][w] = sum;
			memScale[h + (m_Scale / 2)][w] = scaled_1;
			//boundary 6tap
			sum = sixTab[0] * tempBuf[virtualBound - 2 * m_Scale][w]
				+ sixTab[1] * tempBuf[virtualBound - m_Scale	][w]
				+ sixTab[2] * tempBuf[virtualBound				][w]
				+ sixTab[3] * tempBuf[virtualBound + m_Scale	][w]
				+ sixTab[4] * tempBuf[virtualBound + m_Scale	][w]
				+ sixTab[5] * tempBuf[virtualBound + m_Scale	][w];			
			tempBuf[virtualBound + (m_Scale / 2)][w] = sum;
			memScale[virtualBound + (m_Scale / 2)][w] = scaled_1;
		}
	}	
	//scaling
	for (int h = 0; h<padWidth; h += m_Scale) {
		for (int w = 0; w<padWidth; w += m_Scale) {
			tempBuf[h][w] = tempBuf[h][w] <<5;//*32
			memScale[h][w] = scaled_1;
		}
	}
	//half-pel row interplolation
	for (int w = 2 * m_Scale; w < virtualBound - 1; w += m_Scale) {
		for (int h = 2 * m_Scale; h < realBound; h += (m_Scale / 2)) {
			int sum = 0;
			for (int k = 0; k < 6; k++)//6tap
			{
				sum += sixTab[k] * tempBuf[h][w + (k - 2)*(int)m_Scale];
			}
			tempBuf[h][w + (m_Scale / 2)] = sum;
			memScale[h][w + (m_Scale / 2)] = scaled_2;
			//boundary 6tap
			sum = sixTab[0] * tempBuf[h][virtualBoundUP - 2 * m_Scale]
				+ sixTab[1] * tempBuf[h][virtualBoundUP - m_Scale	 ]
				+ sixTab[2] * tempBuf[h][virtualBoundUP				 ]
				+ sixTab[3] * tempBuf[h][virtualBoundUP + m_Scale	 ]
				+ sixTab[4] * tempBuf[h][virtualBoundUP + 2 * m_Scale]
				+ sixTab[5] * tempBuf[h][virtualBoundUP + 2 * m_Scale];
			tempBuf[h][virtualBoundUP + (m_Scale / 2)] = sum;
			memScale[h][virtualBoundUP + (m_Scale / 2)] = scaled_2;
		}
	}
	//sacaling
	for (int h = 2 * m_Scale; h<padWidth; h += (m_Scale / 2)) {
		for (int w = 2 * m_Scale; w<padWidth; w += m_Scale) {
			tempBuf[h][w] = tempBuf[h][w] << 5;//*32
			memScale[h][w] = scaled_2;
		}
	}
	//convert RGB range
	for (int h = 0; h<padWidth; h++) {
		for (int w = 0; w<padWidth; w++) {
			if (0>tempBuf[h][w]) tempBuf[h][w] = 0;
		}
	}
	//quater-pel interpolation
	for (int h = 2 * m_Scale; h<padWidth - 4 * (m_Scale / 2); h += (m_Scale / 2)) {
		for (int w = 2 * m_Scale; w<padWidth - 4 * (m_Scale / 2); w += (m_Scale / 2)) {			
			tempBuf[h	 ][w + 1] = tempBuf[h][w] + tempBuf[h	 ][w + 2];			
			tempBuf[h + 1][w	] = tempBuf[h][w] + tempBuf[h + 2][w	];
			tempBuf[h + 1][w + 1] = tempBuf[h + 2][w] + tempBuf[h][w + 2];			
			memScale[h	  ][w + 1] = 3;//option
			memScale[h + 1][w	 ] = 3;
			memScale[h + 1][w + 1] = 3;
		}
	}

	//round-off, copy outbuf
	for (int y = 0, h = 2 * m_Scale; y<m_ExtWidth; y++, h++) {
		for (int x = 0, w = 2 * m_Scale; x<m_ExtWidth; x++, w++) {
			//indata, div, count
			outBuf[y][x] = roundingOff(tempBuf[h][w], 32 ,memScale[h][w]);
		}
	}
	delete2D(padWidth);
}
