#include <iostream>
#include <Windows.h>
#include <io.h>
#include "ximage.h"
#include "ImageLoader.h"
#include "ImageWriter.h"
#include "Image.h"


using namespace std;

typedef long LONG;
typedef unsigned long DWORD;
typedef unsigned short WORD;

typedef struct {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BMPFILEHEADER_T;

typedef struct {
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BMPINFOHEADER_T;

void savebmp(uint8 * pdata, char * bmp_file, int width, int height)
{      //分别为rgb数据，要保存的bmp文件名，图片长宽  
	int size = width*height * 3 * sizeof(char); // 每个像素点3个字节  
												// 位图第一部分，文件信息  
	BMPFILEHEADER_T bfh;
	bfh.bfType = (WORD)0x4d42;  //bm  
	bfh.bfSize = size  // data size  
		+ sizeof(BMPFILEHEADER_T) // first section size  
		+ sizeof(BMPINFOHEADER_T) // second section size  
		;
	bfh.bfReserved1 = 0; // reserved  
	bfh.bfReserved2 = 0; // reserved  
	bfh.bfOffBits = sizeof(BMPFILEHEADER_T) + sizeof(BMPINFOHEADER_T);//真正的数据的位置  

																	  // 位图第二部分，数据信息  
	BMPINFOHEADER_T bih;
	bih.biSize = sizeof(BMPINFOHEADER_T);
	bih.biWidth = width;
	bih.biHeight = -height;//BMP图片从最后一个点开始扫描，显示时图片是倒着的，所以用-height，这样图片就正了  
	bih.biPlanes = 1;//为1，不用改  
	bih.biBitCount = 24;
	bih.biCompression = 0;//不压缩  
	bih.biSizeImage = size;
	bih.biXPelsPerMeter = 2835;//像素每米  
	bih.biYPelsPerMeter = 2835;
	bih.biClrUsed = 0;//已用过的颜色，24位的为0  
	bih.biClrImportant = 0;//每个像素都重要  
	FILE * fp = fopen(bmp_file, "wb");
	if (!fp) return;

	fwrite(&bfh, 8, 1, fp);//由于linux上4字节对齐，而信息头大小为54字节，第一部分14字节，第二部分40字节，所以会将第一部分补齐为16自己，直接用sizeof，打开图片时就会遇到premature end-of-file encountered错误  
	fwrite(&bfh.bfReserved2, sizeof(bfh.bfReserved2), 1, fp);
	fwrite(&bfh.bfOffBits, sizeof(bfh.bfOffBits), 1, fp);
	fwrite(&bih, sizeof(BMPINFOHEADER_T), 1, fp);
	fwrite(pdata, size, 1, fp);
	fclose(fp);
}

int main()
{


	wstring file = L"111.jpg";
	if (_waccess(file.c_str(), 0) != -1)
	{
		CxImage image;
		image.Load(file.c_str());
		long size = 0;
		uint8* buf = NULL;
		long size_2 = image.GetSize();
		//bool ret = image.Encode(buf, size, CXIMAGE_FORMAT_BMP);
		buf = image.GetBits();  //获取像素内存数据
		ulong w = image.GetWidth();
		ulong h = image.GetHeight();
		ulong bpp = image.GetBpp();
		CxImage image2;
		bool ret = image2.CreateFromArray(buf, w, h, 24, w * 3, false);
		image2.GetBits();
		cout << size_2 << endl;
	     
	 
		//image.Resample(800, 600, 1, &dst);
		image2.Save(L"222.bmp", CXIMAGE_FORMAT_BMP);
	}

	//}
	//cout << sizeof(char) << endl;

	//Image* img = ImageLoader::LoadImageW(L"111.jpg", EIF_JPEG);
	//ImageWriter::ImageWriterW(img, L"333.jpg", EIF_JPEG);
	//delete img;


	return 0;
}