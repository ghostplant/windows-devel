#include <string.h>
#include "jpeglib.h"
#include "memjpeg.h"

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef long				LONG;

// 调色板项
typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;
// bmp头信息
typedef struct tagBITMAPINFOHEADER{
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
} BITMAPINFOHEADER;
// bmp信息
typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO;

//bmp文件头
typedef struct tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} BITMAPFILEHEADER;

void bmp2jpg(char *src,int sSize, char *des,int *dSize, int qual /* 比如=60 */)
{
	BITMAPFILEHEADER bfh;		// bmp文件头
	BITMAPINFOHEADER bih;		// bmp头信息
	RGBQUAD rq[256];			// 调色板
	int nAdjust;		// 用于字节对齐

	BYTE *data= NULL;//new BYTE[bih.biWidth*bih.biHeight];
	//BYTE *pDataConv = NULL;//new BYTE[bih.biWidth*bih.biHeight];
	int nComponent = 0;
	int offset=0;

	// 打开图像文件
	// 读取文件头
	memcpy(&bfh,src+offset,sizeof(bfh));
	offset+=sizeof(bfh);
	// 读取图像信息
	memcpy(&bih,src+offset,sizeof(bih));
	offset+=sizeof(bih);

	int bsize;
	switch (bih.biBitCount) 
	{
	case 8:
		if (bfh.bfOffBits-1024<54) 
			return;
		// 8位字节对齐
		nAdjust = bih.biWidth&3;
		if (nAdjust) nAdjust = 4-nAdjust;

		bsize=(bih.biWidth+nAdjust)*bih.biHeight;
		data= new BYTE[bsize];
		//pDataConv = new BYTE[bih.biWidth*bih.biHeight];

		// 定位调色板，并读取调色板
		offset=bfh.bfOffBits-1024;
		memcpy(rq,src+offset,sizeof(RGBQUAD)<<8);
		offset+=sizeof(RGBQUAD)<<8;
		// 读取位图
		memcpy(data,src+offset,bsize);
		offset+=bsize;
		nComponent = 1;
		break;
	case 24:
		{
		// 8位字节对齐
		nAdjust = (bih.biWidth*3)&3;
		if (nAdjust) nAdjust = 4-nAdjust;
		bsize=(bih.biWidth*3+nAdjust)*bih.biHeight;
		data= new BYTE[bsize];
		//pDataConv = new BYTE[bih.biWidth*bih.biHeight*3];
		offset=bfh.bfOffBits;
		memcpy(data,src+offset,bsize);
		offset+=bsize;
		for (int j=0;j<bih.biHeight;j++){
			for (int i = 0;i<bih.biWidth;i++)
			{
				BYTE red = data[j*(bih.biWidth*3+nAdjust)+i*3];
				data[j*(bih.biWidth*3+nAdjust)+i*3] = data[j*(bih.biWidth*3+nAdjust)+i*3+2];
				data[j*(bih.biWidth*3+nAdjust)+i*3+2] = red;
			}
		}
		nComponent = 3;
		break;
		}
	default:
		return;
	}

	struct jpeg_compress_struct jcs;
	struct jpeg_error_mgr jem;
	jcs.err = jpeg_std_error(&jem);

	jpeg_create_compress(&jcs);

	jpeg_stdio_dest(&jcs, des, dSize);
	jcs.image_width = bih.biWidth; 			// 为图的宽和高，单位为像素 
	jcs.image_height = bih.biHeight;
	jcs.input_components = nComponent;			// 1,表示灰度图， 如果是彩色位图，则为3 
	if (nComponent==1)
		jcs.in_color_space = JCS_GRAYSCALE; //JCS_GRAYSCALE表示灰度图，JCS_RGB表示彩色图像 
	else 
		jcs.in_color_space = JCS_RGB;

	jpeg_set_defaults(&jcs);	
	jpeg_set_quality (&jcs, qual, true);

	jpeg_start_compress(&jcs, TRUE);

	JSAMPROW row_pointer[1];			// 一行位图
	int row_stride;						// 每一行的字节数 

	row_stride = jcs.image_width*nComponent;		// 如果不是索引图,此处需要乘以3

	// 对每一行进行压缩
	while (jcs.next_scanline < jcs.image_height) {
	    row_pointer[0] = & data[(jcs.image_height-jcs.next_scanline-1) * (row_stride+nAdjust)];
	    jpeg_write_scanlines(&jcs, row_pointer, 1);
	}

	jpeg_finish_compress(&jcs);

	jpeg_destroy_compress(&jcs);

	delete [] data;
}

void jpg2bmp(char *src,int sSize, char *des,int *dSize)
{
	BITMAPFILEHEADER bfh;		// bmp文件头

	BITMAPINFOHEADER bih;		// bmp头信息
	RGBQUAD rq[256];			// 调色板
	int nAdjust; // 用于字节对齐

	BYTE *data= NULL;//new BYTE[bih.biWidth*bih.biHeight];
	//BYTE *pDataConv = NULL;//new BYTE[bih.biWidth*bih.biHeight];
	int nComponent = 0;

	// 声明解压缩对象及错误信息管理器
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, src, sSize);
	jpeg_read_header(&cinfo, TRUE);
	nAdjust = cinfo.image_width*cinfo.num_components%4;
	if (nAdjust) nAdjust = 4-nAdjust;
	data = new BYTE[(cinfo.image_width*cinfo.num_components+nAdjust)*cinfo.image_height];	

	jpeg_start_decompress(&cinfo);

	JSAMPROW row_pointer[1];
	while (cinfo.output_scanline < cinfo.output_height)
	{
		row_pointer[0] = &data[(cinfo.output_height - cinfo.output_scanline-1)*(cinfo.image_width*cinfo.num_components+nAdjust)];
		jpeg_read_scanlines(&cinfo,row_pointer,1);
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	// 写文件头
	memset(&bfh,0,sizeof(bfh));
	bfh.bfSize = sizeof(bfh)+sizeof(bih);
	bfh.bfOffBits = sizeof(bfh)+sizeof(bih);
	if (cinfo.num_components==1)
	{
		bfh.bfOffBits += 1024;
		bfh.bfSize += 1024;
	}

	int offset=0;
	bfh.bfSize += (cinfo.image_width*cinfo.num_components+nAdjust)*cinfo.image_height;
	bfh.bfType = 0x4d42;
	memcpy(des+offset,&bfh,sizeof(bfh));
	offset+=sizeof(bfh);
	// 写图像信息
	bih.biBitCount = cinfo.num_components*8;
	bih.biSize = sizeof(bih);
	bih.biWidth = cinfo.image_width;
	bih.biHeight = cinfo.image_height;
	bih.biPlanes = 1;
	bih.biCompression = 0;
	bih.biSizeImage = (cinfo.image_width*cinfo.num_components+nAdjust)*cinfo.image_height;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
	memcpy(des+offset,&bih,sizeof(bih));
	offset+=sizeof(bih);
	// 写调色板
	if (cinfo.num_components ==1)
	{
		for (int i=0;i<256;i++)
		{
			rq[i].rgbBlue =i;
			rq[i].rgbGreen = i;
			rq[i].rgbRed = i;
			rq[i].rgbReserved = 0;
		}
		memcpy(des+offset,rq,1024);
		offset+=1024;
	}

	if (cinfo.num_components==3)
	{
		// 调整rgb顺序
		for (int j=0;j<bih.biHeight;j++)
		for (int i = 0;i<bih.biWidth;i++)
		{
			BYTE red = data[j*(cinfo.image_width*cinfo.num_components+nAdjust)+i*3];
			data[j*(cinfo.image_width*cinfo.num_components+nAdjust)+i*3] = data[j*(cinfo.image_width*cinfo.num_components+nAdjust)+i*3+2];
			data[j*(cinfo.image_width*cinfo.num_components+nAdjust)+i*3+2] = red;
		}
	}
	int endsize=(cinfo.image_width*cinfo.num_components+nAdjust)*cinfo.image_height;
	memcpy(des+offset,data,endsize);
	offset+=endsize;
	*dSize=offset;
	delete [] data;
}
