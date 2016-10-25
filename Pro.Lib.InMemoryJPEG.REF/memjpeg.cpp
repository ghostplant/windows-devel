#include <string.h>
#include "jpeglib.h"
#include "memjpeg.h"

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef long				LONG;

// ��ɫ����
typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;
// bmpͷ��Ϣ
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
// bmp��Ϣ
typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO;

//bmp�ļ�ͷ
typedef struct tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} BITMAPFILEHEADER;

void bmp2jpg(char *src,int sSize, char *des,int *dSize, int qual /* ����=60 */)
{
	BITMAPFILEHEADER bfh;		// bmp�ļ�ͷ
	BITMAPINFOHEADER bih;		// bmpͷ��Ϣ
	RGBQUAD rq[256];			// ��ɫ��
	int nAdjust;		// �����ֽڶ���

	BYTE *data= NULL;//new BYTE[bih.biWidth*bih.biHeight];
	//BYTE *pDataConv = NULL;//new BYTE[bih.biWidth*bih.biHeight];
	int nComponent = 0;
	int offset=0;

	// ��ͼ���ļ�
	// ��ȡ�ļ�ͷ
	memcpy(&bfh,src+offset,sizeof(bfh));
	offset+=sizeof(bfh);
	// ��ȡͼ����Ϣ
	memcpy(&bih,src+offset,sizeof(bih));
	offset+=sizeof(bih);

	int bsize;
	switch (bih.biBitCount) 
	{
	case 8:
		if (bfh.bfOffBits-1024<54) 
			return;
		// 8λ�ֽڶ���
		nAdjust = bih.biWidth&3;
		if (nAdjust) nAdjust = 4-nAdjust;

		bsize=(bih.biWidth+nAdjust)*bih.biHeight;
		data= new BYTE[bsize];
		//pDataConv = new BYTE[bih.biWidth*bih.biHeight];

		// ��λ��ɫ�壬����ȡ��ɫ��
		offset=bfh.bfOffBits-1024;
		memcpy(rq,src+offset,sizeof(RGBQUAD)<<8);
		offset+=sizeof(RGBQUAD)<<8;
		// ��ȡλͼ
		memcpy(data,src+offset,bsize);
		offset+=bsize;
		nComponent = 1;
		break;
	case 24:
		{
		// 8λ�ֽڶ���
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
	jcs.image_width = bih.biWidth; 			// Ϊͼ�Ŀ�͸ߣ���λΪ���� 
	jcs.image_height = bih.biHeight;
	jcs.input_components = nComponent;			// 1,��ʾ�Ҷ�ͼ�� ����ǲ�ɫλͼ����Ϊ3 
	if (nComponent==1)
		jcs.in_color_space = JCS_GRAYSCALE; //JCS_GRAYSCALE��ʾ�Ҷ�ͼ��JCS_RGB��ʾ��ɫͼ�� 
	else 
		jcs.in_color_space = JCS_RGB;

	jpeg_set_defaults(&jcs);	
	jpeg_set_quality (&jcs, qual, true);

	jpeg_start_compress(&jcs, TRUE);

	JSAMPROW row_pointer[1];			// һ��λͼ
	int row_stride;						// ÿһ�е��ֽ��� 

	row_stride = jcs.image_width*nComponent;		// �����������ͼ,�˴���Ҫ����3

	// ��ÿһ�н���ѹ��
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
	BITMAPFILEHEADER bfh;		// bmp�ļ�ͷ

	BITMAPINFOHEADER bih;		// bmpͷ��Ϣ
	RGBQUAD rq[256];			// ��ɫ��
	int nAdjust; // �����ֽڶ���

	BYTE *data= NULL;//new BYTE[bih.biWidth*bih.biHeight];
	//BYTE *pDataConv = NULL;//new BYTE[bih.biWidth*bih.biHeight];
	int nComponent = 0;

	// ������ѹ�����󼰴�����Ϣ������
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

	// д�ļ�ͷ
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
	// дͼ����Ϣ
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
	// д��ɫ��
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
		// ����rgb˳��
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
