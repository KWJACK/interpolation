#include "stdafx.h"
#include "Image.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

Image::Image(){ }//not used

void Image::makeImage(ush width, ush extWidth, char * fileName, char opt, uch scale)
{
	m_Width = width, m_ExtWidth = extWidth, m_Scale = scale;
	allocation2D();//inbuf, outbuf create
	readFile(fileName);
	if (0 == opt) {//opt zero -> extension
		for (int h = 0; h < extWidth; h++) {//downsizeimage make
			for (int w = 0; w < extWidth; w++) {
				int h_new = h * 4;
				int w_new = w * 4;
				outBuf[h][w] = inBuf[h_new][w_new];
			}
		}
	}
}

Image::~Image()
{ }//not used

void Image::allocation2D()
{
	
	inBuf = new uch*[m_Width];
	outBuf = new uch*[m_ExtWidth];
	
	for (ush i = 0; i < m_Width; i++)
		*(inBuf+i) = new uch[m_Width];
	for (ush i = 0; i < m_ExtWidth; i++) {
		*(outBuf + i) = new uch[m_ExtWidth];	
	}
}

void Image::deleteInOut2D()
{//for destory message, using this func
	if (inBuf != nullptr) {
		for (ush i = 0; i < m_Width; i++)
			delete[] * (inBuf + i);
		delete inBuf;
	}
	if( outBuf != nullptr){
		for (ush i = 0; i < m_ExtWidth; i++)
			delete[] * (outBuf + i);
		delete outBuf;
	}
}

void Image::readFile(char * fileName)
{
	FILE *fp;
	fopen_s(&fp, fileName, "rb");
	if (!fp)cout << "Can not open file " << endl;
	for (int i = 0; i < m_Width; i++)
		fread(*(inBuf+i), sizeof(uch), m_Width, fp);
	fclose(fp);
}
void Image::writeFile(char *fileName)
{
	FILE *fp;
	fopen_s(&fp, fileName, "wb");
	if (!fp)cout << "Can not open file " << endl;
	for (int i = 0; i < m_ExtWidth; i++)
		fwrite(*(outBuf+i), sizeof(uch), m_ExtWidth, fp);
	fclose(fp);
}