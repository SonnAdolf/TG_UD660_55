/*完整的bmp位图文件，可以分为文件信息头，位图信息头和RGB颜色阵列三个部分。
文件信息头主要包含“是否是BMP文件”，文件的大小等信息。而位图信息头则主要
包含bmp文件的位图宽度，高度，位平面，通道数等信息。而RGB颜色阵列，里面才
真正包含我们所需要的bmp位图的像素数据。需要提醒的是，bmp位图的颜色阵列部
分，像素数据的存储是以左下角为原点。也就是说，当你打开一个bmp图片并显示在
电脑屏幕上的时，实际在存储的时候，这个图片的最左下角的像素是首先被存储在
bmp文件中的。之后，按照从左到右，从下到上的顺序,依次进行像素数据的存储。
如果，你存储的是3通道的位图数据（也就是我们通常说的彩图），那么它是按照
B0G0R0B1G1R1B2G2R2...的顺序进行存储的，同时，还要考虑到4字节对齐的问题。
*/

#include <stdio.h>   
#include <stdlib.h>  
#include "Bmp_LoadSave.h" 


/*****************************************************************  
* function:		TG_LoadImage
* description:  璇诲彇鍥惧儚
* param1:     	Image* bmpImg	: 鍥惧儚鏁版嵁 (output)
* param2:     	char* path		: 璇诲彇璺緞 (input)
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
  
    pFile = fopen(path, "rb"); //读写打开一个二进制文件，只允许读写数据;文件顺利打开后，指向该流的文件指针就会被返回。
	                           // 如果文件打开失败则返回NULL，并把错误代码存在errno 中 
    if (!pFile)  
    {  
        free(bmpImg);  
//		printf("图片打开失败，检查路径！！！\n");
 		bmpImg = NULL;
		exit(0);
    }  
  
    fread(&fileType, sizeof(uint16_t), 1, pFile);//1.用于接收数据的内存地址
	                                                   //2.要读取的每个数据项的字节数，单位是字节
	                                                   //3.要读count个数据项，每个数据项size个字节
	                                                   //4.输入流
                                                       //返回值：实际读取的元素个数
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

            step = channels*width; //宽度 
  
            quad = (RgbQuad*)malloc(sizeof(RgbQuad)*256);  
            fread(quad, sizeof(RgbQuad), 256, pFile);  
            free(quad);  
			quad = NULL;
            //读入像素数据保存在imageData
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
 * description:  淇濆瓨鍥惧儚
 * param1:		 char* path 	 : 淇濆瓨璺緞 (input)
 * param2:		 Image* bmpImg	 : 鍥惧儚鏁版嵁 (input)
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
   
     fileType = 0x4D42; // "BM"，用十六进制的数表示是0x4d42
     fwrite(&fileType, sizeof(uint16_t), 1, pFile);  
   
    if (bmpImg->channels == 1)//8位，单通道，灰度图   
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
* description:	鏁版嵁淇濆瓨鍒颁竴涓枃浠�
* param1:		unsigned char * my_array : 鏁版嵁 (input)
* param2:		int length				 : 鏁版嵁闀垮害 (input)
* param3:		char *string			 : 淇濆瓨璺緞(input)
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
* description:	璇诲彇涓�涓枃浠剁殑鏁版嵁
* param1:		unsigned char * buf		 : 鏁版嵁 (output)
* param2:		int length				 : 鏁版嵁闀垮害 (input)
* param3:		char *string			 : 璇诲彇璺緞(input)
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
* description:	寮�杈熷搴旂┖闂寸殑浜岀骇鎸囬拡
* param1:		int row		 : 琛屾暟 (input)
* param2:		int col		 : 鍒楁暟 (input)
* return:		unsigned char **锛氬紑杈熺殑浜岀骇鎸囬拡						
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
* description:	閲婃斁浜岀骇鎸囬拡
* param1:		unsigned char **a		 : 闇�瑕侀噴鏀剧殑浜岀骇鎸囬拡 (input)
* param2:		int row					 : 鑰虫満鎸囬拡琛屾暟 (input)
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
* description:	淇濆瓨鍥惧儚锛堢浉瀵逛簬TG_SaveImage锛屽皝瑁呬簡涓�灞傦級
* param1:		unsigned char *src		 : 鍘熷鍥惧儚鏁版嵁 (input)
* param2:		int height				 : 鍥惧儚楂樺害 (input)
* param3:		int width				 : 鍥惧儚瀹藉害 (input)
* param4:		char *bmp_addr			 : 淇濆瓨鍥惧儚鐨勮矾寰� (input)
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
* description:	淇濆瓨鏃嬭浆鍚庣殑鍥惧儚锛堢浉瀵逛簬TG_SaveImage锛屽皝瑁呬簡涓�灞傦級
* param1:		unsigned char *src		 : 鍘熷鍥惧儚鏁版嵁 (input)
* param2:		int height				 : 淇濆瓨鍚庣殑鍥惧儚楂樺害 (input)
* param3:		int width				 : 淇濆瓨鍚庣殑鍥惧儚瀹藉害 (input)
* param4:		char *bmp_addr			 : 淇濆瓨鍥惧儚鐨勮矾寰� (input)
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



