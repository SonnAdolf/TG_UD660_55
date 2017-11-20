/*������bmpλͼ�ļ������Է�Ϊ�ļ���Ϣͷ��λͼ��Ϣͷ��RGB��ɫ�����������֡�
�ļ���Ϣͷ��Ҫ�������Ƿ���BMP�ļ������ļ��Ĵ�С����Ϣ����λͼ��Ϣͷ����Ҫ
����bmp�ļ���λͼ��ȣ��߶ȣ�λƽ�棬ͨ��������Ϣ����RGB��ɫ���У������
����������������Ҫ��bmpλͼ���������ݡ���Ҫ���ѵ��ǣ�bmpλͼ����ɫ���в�
�֣��������ݵĴ洢�������½�Ϊԭ�㡣Ҳ����˵�������һ��bmpͼƬ����ʾ��
������Ļ�ϵ�ʱ��ʵ���ڴ洢��ʱ�����ͼƬ�������½ǵ����������ȱ��洢��
bmp�ļ��еġ�֮�󣬰��մ����ң����µ��ϵ�˳��,���ν����������ݵĴ洢��
�������洢����3ͨ����λͼ���ݣ�Ҳ��������ͨ��˵�Ĳ�ͼ������ô���ǰ���
B0G0R0B1G1R1B2G2R2...��˳����д洢�ģ�ͬʱ����Ҫ���ǵ�4�ֽڶ�������⡣
*/

#include <stdio.h>   
#include <stdlib.h>  
#include "Bmp_LoadSave.h" 


/*****************************************************************  
* function:		TG_LoadImage
* description:  读取图像
* param1:     	Image* bmpImg	: 图像数据 (output)
* param2:     	char* path		: 读取路径 (input)
* return:    	void 			               
* other:
*
* date:       	2017/11/09			
* author:     	
******************************************************************/ 
void TG_LoadImage(Image* bmpImg,char* path)  
{  
  //Image* bmpImg;  
    FILE* pFile;  
    uint16_t fileType;  
    BitMapFileHeader bmpFileHeader;  
    BitMapInfoHeader bmpInfoHeader;  
    int channels = 1;  
    int width = 0;  
    int height = 0;  
    int step = 0;  
    int offset = 0;  
    unsigned char pixVal;  
    RgbQuad* quad;  
    int i, j;  
	//int k;
  
    pFile = fopen(path, "rb"); //��д��һ���������ļ���ֻ�����д����;�ļ�˳���򿪺�ָ��������ļ�ָ��ͻᱻ���ء�
	                           // ����ļ���ʧ���򷵻�NULL�����Ѵ���������errno �� 
    if (!pFile)  
    {  
        free(bmpImg);  
//		printf("ͼƬ��ʧ�ܣ����·��������\n");
 		bmpImg = NULL;
		exit(0);
    }  
  
    fread(&fileType, sizeof(uint16_t), 1, pFile);//1.���ڽ������ݵ��ڴ��ַ
	                                                   //2.Ҫ��ȡ��ÿ����������ֽ�������λ���ֽ�
	                                                   //3.Ҫ��count�������ÿ��������size���ֽ�
	                                                   //4.������
                                                       //����ֵ��ʵ�ʶ�ȡ��Ԫ�ظ���
	if (fileType == 0x4D42)  
    {  
        //printf("bmp file! \n");   
  
        fread(&bmpFileHeader, sizeof(BitMapFileHeader), 1, pFile);  

        fread(&bmpInfoHeader, sizeof(BitMapInfoHeader), 1, pFile);  
  
        if (bmpInfoHeader.biBitCount == 8)  
        {  
            channels = 1;  
            width = bmpInfoHeader.biWidth;  
            height = bmpInfoHeader.biHeight;  
            offset = (channels*width)%4; //??? 
            if (offset != 0)  
            {  
                offset = 4 - offset;  
            }  
            bmpImg->width = width;  
            bmpImg->height = height;  
            bmpImg->channels = 1;  

            step = channels*width; //��� 
  
            quad = (RgbQuad*)malloc(sizeof(RgbQuad)*256);  
            fread(quad, sizeof(RgbQuad), 256, pFile);  
            free(quad);  
			quad = NULL;
            //�����������ݱ�����imageData
            for (i=0; i<height; i++)  
            {  
                for (j=0; j<width; j++)  
                {  
                    fread(&pixVal, sizeof(unsigned char), 1, pFile);  
                    //bmpImg->imageData[(height-1-i)*step+j] = pixVal; 
					*(*(bmpImg->imageData+(height-1-i))+j) = pixVal;
                }  
                if (offset != 0)  
                {  
                    for (j=0; j<offset; j++)  
                    {  
                        fread(&pixVal, sizeof(unsigned char), 1, pFile);  
                    }  
                }  
            }             
        }  
     
    }  
	
	fclose(pFile);
}  


 /*****************************************************************  
 * function:	 TG_SaveImage
 * description:  保存图像
 * param1:		 char* path 	 : 保存路径 (input)
 * param2:		 Image* bmpImg	 : 图像数据 (input)
 * return:		 void						 -	 
 * other:
 *
 * date:		 2017/11/09 		 
 * author:		 
 ******************************************************************/ 
 int TG_SaveImage(char* path, Image* bmpImg)  
 {  
     FILE *pFile;  
     uint16_t fileType;  
     BitMapFileHeader bmpFileHeader;  
     BitMapInfoHeader bmpInfoHeader;  
     int step;  
     int offset;  
     unsigned char pixVal = '\0';  
     int i, j;  
     RgbQuad* quad;  
   
     pFile = fopen(path, "wb");  
     if (!pFile)  
     {  
         return 0;  
     }  
   
     fileType = 0x4D42; // "BM"����ʮ�����Ƶ�����ʾ��0x4d42
     fwrite(&fileType, sizeof(uint16_t), 1, pFile);  
   
    if (bmpImg->channels == 1)//8λ����ͨ�����Ҷ�ͼ   
     {  
         step = bmpImg->width;  
         offset = step%4;  
         if (offset != 4)  
         {  
             step += 4-offset;  
         }  
   
         bmpFileHeader.bfSize = 54 + 256*4 + bmpImg->width;  
         bmpFileHeader.bfReserved1 = 0;  
         bmpFileHeader.bfReserved2 = 0;  
         bmpFileHeader.bfOffBits = 54 + 256*4;  
         fwrite(&bmpFileHeader, sizeof(BitMapFileHeader), 1, pFile);  
   
         bmpInfoHeader.biSize = 40;  
         bmpInfoHeader.biWidth = bmpImg->width;  
         bmpInfoHeader.biHeight = bmpImg->height;  
         bmpInfoHeader.biPlanes = 1;  
         bmpInfoHeader.biBitCount = 8;  
         bmpInfoHeader.biCompression = 0;  
         bmpInfoHeader.biSizeImage = bmpImg->height*step;  
         bmpInfoHeader.biXPelsPerMeter = 0;  
         bmpInfoHeader.biYPelsPerMeter = 0;  
         bmpInfoHeader.biClrUsed = 256;  
         bmpInfoHeader.biClrImportant = 256;  
         fwrite(&bmpInfoHeader, sizeof(BitMapInfoHeader), 1, pFile);  
   
         quad = (RgbQuad*)malloc(sizeof(RgbQuad)*256);  
         for (i=0; i<256; i++)  
         {  
             quad[i].rgbBlue = i;  
             quad[i].rgbGreen = i;  
             quad[i].rgbRed = i;  
             quad[i].rgbReserved = 0;  
         }  
         fwrite(quad, sizeof(RgbQuad), 256, pFile);  
         free(quad);  
   
         for (i=bmpImg->height-1; i>-1; i--)  
//		 for (i=0; i < bmpImg->height; i++) 
         {  
             for (j=0; j<bmpImg->width; j++)  
             {  
                 //pixVal = bmpImg->imageData[i*bmpImg->width+j];
 				pixVal = *(*(bmpImg->imageData+i)+j);
                 fwrite(&pixVal, sizeof(unsigned char), 1, pFile);  
             }  
             if (offset!=0)  
             {  
                 for (j=0; j<offset; j++)  
                 {  
                     pixVal = 0;  
                     fwrite(&pixVal, sizeof(unsigned char), 1, pFile);  
                 }  
             }  
         }  
     }  
     fclose(pFile);  
   
     return 1;  
 } 


/*****************************************************************	
* function: 	bmp_write_data_hex
* description:	数据保存到一个文件
* param1:		unsigned char * my_array : 数据 (input)
* param2:		int length				 : 数据长度 (input)
* param3:		char *string			 : 保存路径(input)
* return:		void							
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/ 
void bmp_write_data_hex(unsigned char * my_array,int length,char *string)
{
    int i = 0;
    FILE *fp;
    fp = fopen(string,"wb+");
    if(NULL == fp)
    {
        printf("file open Fail!\n");
    }
    while(i < length)
    {        
        fwrite(&my_array[i],sizeof(unsigned char ),1,fp);
        i++;
    }
    fclose(fp);
}
 

/*****************************************************************	
* function: 	bmp_read_data_hex
* description:	读取一个文件的数据
* param1:		unsigned char * buf		 : 数据 (output)
* param2:		int length				 : 数据长度 (input)
* param3:		char *string			 : 读取路径(input)
* return:		void						
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/ 
void bmp_read_data_hex(unsigned char *buf,int length,char *string)
{
  int i = 0;
	int re;
	FILE *fp;
	fp = fopen(string,"rb");
	if(NULL == fp)
	{
	    printf("file open Fail!\n");
	}
	fread(buf,sizeof(unsigned char),length,fp);
  fclose(fp);
}

/*****************************************************************	
* function: 	bmp_Make2DArray_uint8
* description:	开辟对应空间的二级指针
* param1:		int row		 : 行数 (input)
* param2:		int col		 : 列数 (input)
* return:		unsigned char **：开辟的二级指针						
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/ 
unsigned char **bmp_Make2DArray_uint8(int row,int col){
	unsigned char **a;
	int i;
	a=(unsigned char **)calloc(row,sizeof(unsigned char *));
	for(i=0;i<row;i++)
	{
		a[i]=(unsigned char *)calloc(col,sizeof(unsigned char));
	}
	return a;

}

/*****************************************************************	
* function: 	bmp_Free2DArray_uint8
* description:	释放二级指针
* param1:		unsigned char **a		 : 需要释放的二级指针 (input)
* param2:		int row					 : 耳机指针行数 (input)
* return:		void						
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/ 
void bmp_Free2DArray_uint8(unsigned char **a,int row){
	int i;
	for(i=0;i<row;i++)
	{
		free(a[i]);
		a[i] = NULL;
	}
	free(a);
	a = NULL;
}
 
 
 
/*****************************************************************	
* function: 	TG_SaveBmp
* description:	保存图像（相对于TG_SaveImage，封装了一层）
* param1:		unsigned char *src		 : 原始图像数据 (input)
* param2:		int height				 : 图像高度 (input)
* param3:		int width				 : 图像宽度 (input)
* param4:		char *bmp_addr			 : 保存图像的路径 (input)
* return:		void						
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/ 
void TG_SaveBmp(unsigned char *src,int height,int width,char *bmp_addr)
{
	int i,j,k;
	Image *myfingerImg;
	myfingerImg = (Image *)malloc(sizeof(Image));
	myfingerImg->channels = 1;
	myfingerImg->height = height;
	myfingerImg->width = width;
	myfingerImg->imageData = bmp_Make2DArray_uint8(height,width);
	
	k = 0;
	for(i = 0;i<height;i++)
	{
		for(j = 0;j<width;j++)
		{
			*(*(myfingerImg->imageData+i)+j) = src[k++];
		}
	}
	TG_SaveImage(bmp_addr,myfingerImg) ;
	bmp_Free2DArray_uint8(myfingerImg->imageData,height);
	free(myfingerImg);
	myfingerImg = NULL;
}

 
/*****************************************************************	
* function: 	TG_SaveRotateBmp
* description:	保存旋转后的图像（相对于TG_SaveImage，封装了一层）
* param1:		unsigned char *src		 : 原始图像数据 (input)
* param2:		int height				 : 保存后的图像高度 (input)
* param3:		int width				 : 保存后的图像宽度 (input)
* param4:		char *bmp_addr			 : 保存图像的路径 (input)
* return:		void						
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/ 
void TG_SaveRotateBmp(unsigned char *src, int height, int width, char *bmp_addr)
{
	int i,j,k;
	Image *myfingerImg;
	myfingerImg = (Image *)malloc(sizeof(Image));
	myfingerImg->channels = 1;
	myfingerImg->height = height;
	myfingerImg->width = width;
	myfingerImg->imageData = bmp_Make2DArray_uint8(height,width);
	
	k = 0;

	for (j = 0;j < width;j++)
	{
		for (i = height - 1; i >= 0; i--)
		{
			*(*(myfingerImg->imageData + i) + j) = src[k++];
		}
	}
	TG_SaveImage(bmp_addr,myfingerImg) ;
	bmp_Free2DArray_uint8(myfingerImg->imageData,height);
	free(myfingerImg);
	myfingerImg = NULL;

}



