#include "utils.h"
#include <iostream>

#ifdef MACOS
unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader) {
    FILE *filePtr;
    BITMAPFILEHEADER bitmapFileHeader;
    unsigned char *bitmapImage;
    int imageIdx = 0;
    unsigned char tempRGB;

    filePtr = fopen(filename, "rb");
    if (filePtr == NULL) return NULL;

    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
    if (bitmapFileHeader.bfType != BITMAP_ID) {
        fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
        fclose(filePtr);
        return NULL;
    }

    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    bitmapImage = (unsigned char *)malloc(bitmapInfoHeader->biSizeImage);
    if (!bitmapImage) {
        fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
        fclose(filePtr);
        return NULL;
    }

    fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
    if (bitmapImage == NULL) {
        fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
        fclose(filePtr);
        return NULL;
    }

	if (bitmapInfoHeader->biBitCount == 24) {
		for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
			tempRGB = bitmapImage[imageIdx];
			bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
			bitmapImage[imageIdx + 2] = tempRGB;
		}
	}

    fclose(filePtr);
    return bitmapImage;
}
#else
unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader)
{
	FILE* filePtr;	// �ļ�ָ��
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap�ļ�ͷ
	unsigned char* bitmapImage;		// bitmapͼ������
	int	imageIdx = 0;		// ͼ��λ������
	unsigned char	tempRGB;	// ��������

								// �ԡ�������+����ģʽ���ļ�filename 
	fopen_s(&filePtr, filename, "rb");
	if (filePtr == NULL) return NULL;
	// ����bitmap�ļ�ͼ
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// ��֤�Ƿ�Ϊbitmap�ļ�
	if (bitmapFileHeader.bfType != BITMAP_ID) {
		fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
		return NULL;
	}

	// ����bitmap��Ϣͷ
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	// ���ļ�ָ������bitmap����
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Ϊװ��ͼ�����ݴ����㹻���ڴ�
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
	// ��֤�ڴ��Ƿ񴴽��ɹ�
	if (!bitmapImage) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	// ����bitmapͼ������
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	// ȷ�϶���ɹ�
	if (bitmapImage == NULL) {
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}

	//����bitmap�б���ĸ�ʽ��BGR�����潻��R��B��ֵ���õ�RGB��ʽ
	for (imageIdx = 0;
		imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// �ر�bitmapͼ���ļ�
	fclose(filePtr);
	return bitmapImage;
}
#endif

void texload(int i, char* filename)
{

	BITMAPINFOHEADER bitmapInfoHeader;                                 // bitmap��Ϣͷ
	unsigned char* bitmapData;                                       // ��������

	bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	// ָ����ǰ����ķŴ�/��С���˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
		0, 	    //mipmap���(ͨ��Ϊ0����ʾ���ϲ�) 
		GL_RGB,	//����ϣ���������к졢�̡�������
		bitmapInfoHeader.biWidth, //��������������n�����б߿�+2 
		bitmapInfoHeader.biHeight, //����߶ȣ�������n�����б߿�+2 
		0, //�߿�(0=�ޱ߿�, 1=�б߿�) 
		GL_RGB,	//bitmap���ݵĸ�ʽ
		GL_UNSIGNED_BYTE, //ÿ����ɫ���ݵ�����
		bitmapData);	//bitmap����ָ��
}

bool SaveBitmapFile(const char* filename, const unsigned char* data, int width, int height) {
    BITMAPFILEHEADER bmpFileHeader;
    BITMAPINFOHEADER bmpInfoHeader;
    FILE* filePtr;
    int imageSize = width * height * 3; // ÿ������RGB�����ֽ�

    // ����BITMAPINFOHEADER
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biHeight = height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24; // 24λλͼ
    bmpInfoHeader.biCompression = BI_RGB;
    bmpInfoHeader.biSizeImage = 0; // 0 for BI_RGB
    bmpInfoHeader.biXPelsPerMeter = 0; // ������
    bmpInfoHeader.biYPelsPerMeter = 0; // ������
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;

    // ����BITMAPFILEHEADER
    bmpFileHeader.bfType = BITMAP_ID;
    bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageSize;
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;

#ifdef MACOS
    // ����MACOS���ļ���
    filePtr = fopen(filename, "wb");
#else
    fopen_s(&filePtr, filename, "wb");
#endif
    if (!filePtr) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return false;
    }

    // д��λͼ�ļ�ͷ
    fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

    // д��λͼ��Ϣͷ
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

    // ����RGB��ɫͨ��˳����ƥ��BGR��ʽ
    unsigned char* bmpBuffer = new unsigned char[imageSize];
    if (!bmpBuffer) {
        std::cerr << "Error: Unable to allocate memory for the BMP buffer." << std::endl;
        fclose(filePtr);
        return false;
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int index = (i * width + j) * 3;
            bmpBuffer[index + 0] = data[index + 2]; // R
            bmpBuffer[index + 1] = data[index + 1]; // G
            bmpBuffer[index + 2] = data[index + 0]; // B
        }
    }

    // д��ͼ������
    for (int i = 0; i < height; ++i) {
        fwrite(bmpBuffer + (width * (height - i - 1) * 3), 3, width, filePtr);
    }

    // �ͷ��ڴ�͹ر��ļ�
    delete[] bmpBuffer;
    fclose(filePtr);

    return true;
}