#pragma once
typedef unsigned char uch;
typedef unsigned short ush;

class Image
{
protected:
	uch **inBuf=nullptr;		

	ush m_Width = 0;//VS2015 support this expression
	ush m_ExtWidth = 0;
	uch m_Scale = 0;	
public:
	Image();	
	~Image();
	uch **outBuf=nullptr;
	void makeImage(ush width, ush extWidth, char * fileName, char opt, uch scale);
	uch ** getInBuf() { return inBuf; }
	void allocation2D();
	void deleteInOut2D();
	void readFile(char *fileName);//copy to inBuf
	void writeFile(char * fileName);//write using outBuf
};
