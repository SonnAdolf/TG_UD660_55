#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "fun.h"
#include "head.h"
#include "tg_main.h"
extern pthread_mutex_t mutex_spi; 	//spi mutex

/*****************************************************************	
* function: 	trans_encrypt
* description:	���ݼ���
* param1:		unsigned char * data : ����ܵ����� (input)
* param2:		int length			 : ���ݳ��� (input)
* return:		void				
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/
void trans_encrypt(unsigned char * data,int length)
{
	int i,j,temp1,temp2;
	unsigned char key[32] = {0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f};

	temp1 = length / 32;
	temp2 = length % 32;
	for(i = 0;i < temp1;i++)
	{
		for(j = 0;j < 32;j++)
		{
			data[i*32+j] ^= key[j];		
		}
	}
	for(j = 0;j < temp2;j++)
	{
		data[i*32+j] ^=  key[j];		
	}
}

/*****************************************************************	
* function: 	Make2DArray_uint8
* description:	���ٶ�Ӧ�ռ�Ķ���ָ��
* param1:		int row		 : ���� (input)
* param2:		int col		 : ���� (input)
* return:		unsigned char **�����ٵĶ���ָ��						
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/ 
unsigned char **Make2DArray_uint8(int row,int col){
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
* function: 	Free2DArray_uint8
* description:	�ͷŶ���ָ��
* param1:		unsigned char **a		 : ��Ҫ�ͷŵĶ���ָ�� (input)
* param2:		int row					 : ����ָ������ (input)
* return:		void						
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/ 
void Free2DArray_uint8(unsigned char **a,int row){
	
	int i;
	for(i=0;i<row;i++)
	{
		free(a[i]);
		a[i] = NULL;
	}
	free(a);
	a = NULL;
	//printf("part");
}

/*****************************************************************	
* function: 	read_data_hex
* description:	��ȡһ���ļ�������
* param1:		unsigned char * buf		 : ���� (output)
* param2:		int length				 : ���ݳ��� (input)
* param3:		char *string			 : ��ȡ·��(input)
* return:		void						
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/
void read_data_hex(unsigned char *buf,int length,char *string)
{
    int i = 0;
	int re;
    FILE *fp;
    fp = fopen(string,"rb");
    if(NULL == fp)
    {
        printf("file open Fail!\n");
    }
/*
    while(i < length)
    {     
		fread(buf+i,sizeof(unsigned char),1,fp);
		i++;
    }
*/
	fread(buf,sizeof(unsigned char),length,fp);
    fclose(fp);
}

/*****************************************************************	
* function: 	write_data_hex
* description:	���ݱ��浽һ���ļ�
* param1:		unsigned char * my_array : ���� (input)
* param2:		int length				 : ���ݳ��� (input)
* param3:		char *string			 : ����·��(input)
* return:		void							
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/ 
void write_data_hex(unsigned char * my_array,int length,char *string)
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
* function: 	tg_get_file_data
* description:	��ȡ�ļ�����
* param1:		char *file_path			 : �ļ�·�� (input)
* param2:		char *data				 : �������� (output)
* return:		void							
* other:
*
* date: 		2017/11/09			
* author:		
******************************************************************/ 
int tg_get_file_data(char *file_path,char *data)
{
	FILE*fp;
	int len;
	fp=fopen(file_path,"rb"); 
	if(NULL == fp)
		return -2;
	fseek(fp,0L,SEEK_END);
	len=ftell(fp);
	if(len > 20)
	{
		fclose(fp);
		return -1;
	}
	fseek(fp,0L,SEEK_SET);
	fread(data,len,1,fp);
	data[len] = 0;
	fclose(fp);
	return 0;
}


/*****************************************************************  
* function:		tg_dir_filenum
* description:  count filenum in a dir
* param1:     	char *dir_path	: dir path (input)
* return:    	filenum (int)  			            -   
* other:
*
*
* date:       	2017/04/14			
* author:     	Hangchang Xu
******************************************************************/ 
int tg_dir_filenum(char *dir_path)
{
	int num = 0;	
	DIR *dirptr = NULL;	
	struct dirent *entry;	
	if((dirptr = opendir(dir_path)) == NULL)  
	{  		
		printf("open dir failed! dir = %s\n",dir_path);  		
		return -1;  	
	} 					
	while (entry = readdir(dirptr))  	
	{ 		
		if(entry->d_type == 8)		//8:file
			{
				//printf("%s\n", entry->d_name);/* print all name in this dir  */ 			
				num ++;		
			}	
	}	
	closedir(dirptr);	
	return num;
}


/*****************************************************************  
* function:		tg_get_one_file
* description:  get one file in a dir
* param1:     	char *dir_path	: dir path (input)
* param2:     	char *file_name	: file_name (output)
* param3:     	unsigned char *data	: file data (output)
* return:    	success flag: 	1:success
*								-1:failed
* other:
*
*
* date:       	2017/04/22			
* author:     	Hangchang Xu
******************************************************************/ 
int tg_get_one_file(char *dir_path,char *file_name,unsigned char *data)
{
	int ret = -1 ;	
	DIR *dirptr = NULL;	
	struct dirent *entry;	
	if((dirptr = opendir(dir_path)) == NULL)  
	{  		
		printf("open dir failed! dir = %s\n",dir_path);  		
		return -1;  	
	} 					
	while (entry = readdir(dirptr))  	
	{ 		
		if(entry->d_type == 8)		//8:file
			{		
				//printf("%s\n", entry->d_name);/* print all name in this dir  */
				sprintf(file_name,"%s/%s",dir_path,entry->d_name);
				ret  = 1;
				break;
			}	
	}	
	closedir(dirptr);	
	return ret;
}


/*****************************************************************  
* function:		display_tgpackage
* description:  ��ӡ���ݰ�
* param1:     	TG_package* pack	: ָ��� (input)
* return:    	void
* other:
*
*
* date:       	2017/11/09			
* author:     	
******************************************************************/ 
void display_tgpackage(TG_package* pack)
{
#ifdef TG_DEBUG
	printf("--------------------------------------------------------\n");
	printf("|------------------------PACKAGE------------------------\n");
	printf("|device_name = %.32s\n",pack->device_name);
	printf("|id = %d\n",pack->id);
	printf("|cmd1 = %x\n",pack->cmd1);
	printf("|cmd2 = %d\n",pack->cmd2);
	printf("|length = %d\n",pack->length);
	printf("|cert_type = %d\n",pack->cert_type);	
	printf("|sup_mgr_num = %d\n",pack->sup_mgr_num);
	printf("|mgr_num = %d\n",pack->mgr_num);
	printf("|usr_num = %d\n",pack->usr_num);	
	printf("|serial_num = %.32s\n",pack->serial_num);
	printf("|random_num = %.32s\n",pack->random_num);
	printf("|cert_name = %.32s\n",pack->cert_name);
	printf("--------------------------------------------------------\n");
#endif

}

/*****************************************************************  
* function:		display_tgpackage
* description:  ��ӡ֤��
* param1:     	TG_cert* pack	: ֤�� (input)
* return:    	void
* other:
*
*
* date:       	2017/11/09			
* author:     	
******************************************************************/
void display_cert(TG_cert* pack)
{
#ifdef TG_DEBUG
	printf("--------------------------------------------------------\n");
	printf("|---------------------CERTIFICATION---------------------\n");
	printf("|cert user_name = %.32s\n",pack->user_name);//\u7528\u6237\u540D
	printf("|cert user_id = %d\n",pack->user_id);
	printf("|cert cert_type = %d\n",pack->cert_type);//\u7528\u6237\u7C7B\u578B
	printf("|cert cert_valid_date = %.32s\n",pack->cert_valid_date);
	printf("|cert crc1 = %x\n",pack->crc[0]);
	printf("|cert crc2 = %x\n",pack->crc[1]);
	printf("--------------------------------------------------------\n");
#endif
}


/*****************************************************************  
* function:		resolve_config
* description:  resolve h3 config file /etc/config
* param1:     	char *path	: config file path (input)
* param2:     	char *ser_num	: serial num (output)
* param3:     	char *ip_addr	: ip addr (output)
* param4:     	char *mac_addr	: mac addr (output)
* return:    	-1:error 			            -   
* others\uFF1A
*
* date:       	2017/04/18			
* author:     	Hangchang Xu
******************************************************************/ 
int resolve_config(char *path,char *ser_num,char *ip_addr,char *mac_addr)
{
	char *p;
	char n;	
	struct stat statbuf;
	char buf[256] = {0};

	stat(path,&statbuf);
	read_data_hex(buf,statbuf.st_size,path);
	 		  
	p=strstr(buf,"ID ");		  
	if(p!=NULL){			  
		n=0;p+=3; 		  		  
		memcpy(ser_num,p,20);	
		printf("serial_num = %.20s\n",ser_num);
	}
	else 
		return -1;	

	p=strstr(buf,"IP ");		  
	if(p!=NULL){			  
		n=0;p+=3; 
		while(*(p+n)!='\n'&&n<32) 
			n++;	
		printf("n = %d\n",n);
		memcpy(ip_addr,p,n);			
	}
	else 
		return -2;	

	p=strstr(buf,"MAC ");		  
	if(p!=NULL){			  
		n=0;p+=4; 		  		  
		memcpy(mac_addr,p,17);	
	}
	else 
		return -3;	

	return 1;
}

/*
uint16_t CrcCompute(uint8_t *src, uint16_t len, uint16_t crc)
{
//	#define CRC_DFE_POLY    0x8005
	uint8_t uc;
	for (uint16_t j = 0; j < len; j++)
	{
		uc = *(src + j);
		for(uint16_t i = 0; i < 8; i++)
		{
			crc = ((uc ^ (uint8_t)(crc >> 8)) & 0x80) ? ((crc << 1) ^ 0x8005) : (crc << 1);
			uc <<= 1;
		}
	}
	return crc;
}
*/


/*****************************************************************  
* function:		test_crc
* description:  �������ݴ�ĩβ�����ֽڵ�CRC�Ƿ���ȷ
* param1:     	uint8_t *src	: ���� (input)
* param2:     	uint16_t dat_len: crc֮ǰ�ĳ��� (output)
* return:    	0 : ʧ��
				1 : �ɹ�			            -   
* other:
*
* date:       	2017/11/09			
* author:     	
******************************************************************/ 
int test_crc(uint8_t *src,uint16_t dat_len)
{
	uint16_t crc;
	uint8_t crc1;
	uint8_t crc2;
	crc = CrcCompute(src, dat_len, 0);
	crc1 = (crc >> 8)  & 0xff;
	crc2 = crc  & 0xff;
	
#ifdef TG_DEBUG
	printf("test_crc crc1 = %x\n",crc1);
	printf("test_crc crc2 = %x\n",crc2);
	printf("test_crc *(src+dat_len) = %x\n",*(src+dat_len));
	printf("test_crc *(src+dat_len+1) = %x\n",*(src+dat_len+1));
#endif	
	if((*(src+dat_len) == crc1 )&&(*(src+dat_len+1) == crc2 ))
		return 1;
	else 
		return 0;
}

/*****************************************************************  
* function:		make_crc
* description:  �����ݴ�ĩβ������ֽڵ�CRC
* param1:     	uint8_t *src	: ���� (input)
* param2:     	uint16_t dat_len: crc֮ǰ�ĳ��� (input)
* return:    	void	            -   
* other:
*
* date:       	2017/11/09			
* author:     	
******************************************************************/
void make_crc(uint8_t *src,uint16_t dat_len)
{
	uint16_t crc;
	uint8_t crc1;
	uint8_t crc2;
	crc = CrcCompute(src, dat_len, 0);
	crc1 = (crc >> 8)  & 0xff;
	crc2 = crc  & 0xff;

	*(src+dat_len) = crc1;
	*(src+dat_len+1) = crc2;
	
#ifdef TG_DEBUG
	printf("make_crc *(src+dat_len) = %x\n",*(src+dat_len));
	printf("make_crc *(src+dat_len+1) = %x\n",*(src+dat_len+1));
#endif	
}

/*****************************************************************  
*
*						.dat operation
*
******************************************************************/ 


/*****************************************************************  
* function:		dat_get_local_num
* description:  get the file num in a dir
* param1:     	char* addr	: path of dir (input)
* return:    	-1		: open dir failed 
*				else	: file num            -   
* others:
* date:       	2017/03/03			
* author:     	Hangchang Xu
******************************************************************/ 
int dat_get_local_num(char* addr)
{
	int num = 0;
	DIR *dirptr = NULL;
	struct dirent *entry;
	if((dirptr = opendir(addr)) == NULL)  
	{  
		printf("open dir failed!\n");  
		return -1;  
	} 				
	while (entry = readdir(dirptr))  
	{ 
		if(entry->d_type == 8)
		{
//			printf("%s\n", entry->d_name);/* print all name in this dir  */ 
			num ++;
		}
	}
	closedir(dirptr);
	return num;
}

/*****************************************************************
* function:		dat_get_usr_dat
* description:  get the all file dat in a dir
* param1:     	char* addr	: path of dir	(input)
* param2:     	stu_usr *usr_addr	: all dat path	(output)
* param3:     	unsigned char *dat	: pointer of all dat	(output)
*
* return:    	1	: succeed 
* 				-1	: open dir failed            -   
* others:		one file  = TZD_LENGTH*3 Bytes
* date:       	2017/03/03			
* author:     	Hangchang Xu
******************************************************************/ 
int dat_get_usr_dat(char* addr,stu_usr *usr_addr,unsigned char *dat)
{
	int i = 0;
	int sizeFeature3 = TZD_LENGTH*3;
	char str_addr[256] = {0};
	DIR *dirptr = NULL;
	struct dirent *entry;
	unsigned char * tempData;
	tempData = (unsigned char *)malloc(sizeFeature3*sizeof(unsigned char));
	
	if((dirptr = opendir(addr)) == NULL)  
	{  
		printf("open dir failed!\n");  
		return -1;  
	} 				
	
	while (entry = readdir(dirptr))  
	{ 
		if(entry->d_type == 8)
		{
#ifdef TG_DEBUG
			printf("%s\n", entry->d_name);/* print all name in this dir  */ 
#endif
			memset(str_addr,0,256*sizeof(char));
			sprintf(str_addr,"%s/%s",addr,entry->d_name);
#ifdef TG_DEBUG
			printf("addr = %s,i = %d\n", str_addr,i);/* print all address*/ 
#endif
//			memset(tempData,0,sizeFeature3*sizeof(char));
			read_data_hex(tempData,sizeFeature3*sizeof(char),str_addr);
			memcpy(dat + i*sizeFeature3,tempData,sizeFeature3);

			memset((usr_addr+i)->addr,0,256*sizeof(char));
			strcpy((usr_addr+i)->addr,str_addr+7);
			(usr_addr+i)->num = i;
			i++;
		}
	}
	closedir(dirptr);	
	free(tempData);
	tempData = NULL;
	return 1;  
}


/*****************************************************************
* function:		dat_get_local_cert
* description:  get the all certs in a dir
* param1:     	int fd	: spi fd	(input)
* param2:     	char* addr	: path of dir	(input)
* param3:     	stu_usr *usr_addr	: all cert path	(output)
* param4:     	TG_cert *cert: pointer of all cert	(output)
*
* return:    	1	: succeed 
* 				-1	: open dir failed            -   
* others:		one cert size = 6116 Bytes
* date:       	2017/07/11		
* author:     	Hangchang Xu
******************************************************************/ 
int dat_get_local_cert(int fd,char* addr,stu_usr *usr_addr,TG_cert * cert)
{
	int i = 0;
	int sizeCert = sizeof(TG_cert);
	int ret_spi;
	char str_addr[256] = {0};
	DIR *dirptr = NULL;
	struct dirent *entry;
	TG_cert * tempCert;
	tempCert = (TG_cert *)malloc(sizeof(TG_cert));
	
	if((dirptr = opendir(addr)) == NULL)  
	{  
		printf("open dir failed!\n");  
		return -1;  
	} 				
	pthread_mutex_lock(&mutex_spi);
	tg_spi_key_req(fd);
	pthread_mutex_unlock(&mutex_spi);
	
	while (entry = readdir(dirptr))  
	{ 
		if(entry->d_type == 8)
		{
//			printf("%s\n", entry->d_name);/* print all name in this dir  */ 
			memset(str_addr,0,256*sizeof(char));
			sprintf(str_addr,"%s/%s",addr,entry->d_name);
#ifdef TG_DEBUG
			printf("cert addr = %s,i = %d\n", str_addr,i);/* print all address*/
#endif

//			memset(tempData,0,sizeFeature3*sizeof(char));
			read_data_hex(tempCert,sizeof(TG_cert),str_addr);

			pthread_mutex_lock(&mutex_spi);
			ret_spi = tg_spi_cert_decrypt(fd,tempCert);
			pthread_mutex_unlock(&mutex_spi);
			if (ret_spi < 0)
				printf("tg_spi_cert_decrypt  failed\n");

			memcpy(cert + i,tempCert,sizeof(TG_cert));
			memset((usr_addr+i)->addr,0,256*sizeof(char));
			strcpy((usr_addr+i)->addr,str_addr);
			(usr_addr+i)->num = i;
			i++;
		}
	}
	closedir(dirptr);	
	free(tempCert);
	tempCert = NULL;
	return 1;  
}

/*****************************************************************
* function:		compare_data
* description:  �Ƚ���������
* param1:     	unsigned char *str1	: ����1	(input)
* param2:     	unsigned char *str2	: ����2	(output)
* param3:     	int len				: ��Ƚϵĳ���	(output)
*
* return:    	����	: ���� 
* 				֤��	: ��ȷ            -   
* others:		
* date:       	2017/11/09			
* author:     	
******************************************************************/
int compare_data(unsigned char *str1,unsigned char *str2,int len)
{
	int i;
	int ret = 0;
	for(i = 0;i < len;i++)
	{
		if(*(str1 + i) != *(str2 + i))
		{
			ret = -len;
			break;
		}
	}
	return ret;
}


/*****************************************************************
* function:		make_localcert_to_dat
* description:  malloc local_cert/local_data/usr_addr
* param1:     	int fd	: spi fd;	(input)
* param2:     	unsigned char *local_data	: all local chara;	(output)
* param3:     	stu_usr *usr_addr: local chara list	(output)
* param4:     	int *num: user num	(output)
*
* return:    	1          
* others:		
* date:       	2017/07/11			
* author:     	Hangchang Xu
******************************************************************/ 
int make_localcert_to_dat(int fd,unsigned char **local_data,stu_usr **usr_addr,int *num)
{
	int i;
	int sizeFeature3 = TZD_LENGTH * 3;
	int mgr_num;
	int user_num;
	int ret_spi;
	TG_cert *local_cert;
	TG_cert *temp_cert;
	*num = 0;
	

	mgr_num = tg_dir_filenum(MGR_CERT_PATH);
	user_num = tg_dir_filenum(USER_CERT_PATH);
	*num =  mgr_num + user_num;

#ifdef REG_DEBUG
	printf("mgr_num = %d\n",mgr_num);
	printf("user_num = %d\n",user_num);
#endif	

	local_cert = (TG_cert *)malloc(*num *sizeof(TG_cert));
	temp_cert = local_cert;
	
	*local_data = (unsigned char *)malloc(*num * sizeFeature3 *sizeof(unsigned char));
	*usr_addr = (stu_usr *)malloc(*num * sizeof(stu_usr));

	dat_get_local_cert(fd,MGR_CERT_PATH,*usr_addr,local_cert);
	dat_get_local_cert(fd,USER_CERT_PATH,*usr_addr+mgr_num,local_cert+mgr_num);

	for(i = 0;i<*num;i++)
	{
		memcpy(*local_data+i*sizeFeature3,temp_cert->chara,sizeFeature3);
		temp_cert = temp_cert + 1;
	}

	free(local_cert);
	local_cert = NULL;
	return 1;

}

/*****************************************************************
* function:		test_make_localcert_to_dat
* description:  malloc local_cert/local_data/usr_addr
* param1:     	int fd	: spi fd;	(input)
* param2:     	unsigned char *local_data	: all local chara;	(output)
* param3:     	stu_usr *usr_addr: local chara list	(output)
* param4:     	int *num: user num	(output)
*
* return:    	1          
* others:		
* date:       	2017/07/11			
* author:     	Hangchang Xu
******************************************************************/ 
int test_make_localcert_to_dat(int fd,unsigned char **local_data,stu_usr **usr_addr,int *num)
{
	int i;
	int sizeFeature3 = TZD_LENGTH * 3;

	int ret_spi;
	TG_cert *local_cert;
	TG_cert *temp_cert;
	*num = 0;
	
	*num = tg_dir_filenum(TSET_USER_CERT_PATH);


#ifdef REG_DEBUG
	printf("test_num = %d\n",*num);
#endif	

	local_cert = (TG_cert *)malloc(*num *sizeof(TG_cert));
	temp_cert = local_cert;
	
	*local_data = (unsigned char *)malloc(*num * sizeFeature3 *sizeof(unsigned char));
	*usr_addr = (stu_usr *)malloc(*num * sizeof(stu_usr));

	dat_get_local_cert(fd,TSET_USER_CERT_PATH,*usr_addr,local_cert);
	for(i = 0;i<*num;i++)
	{
		memcpy(*local_data+i*sizeFeature3,temp_cert->chara,sizeFeature3);
		temp_cert = temp_cert + 1;
	}
	free(local_cert);
	local_cert = NULL;
	return 1;

}


void free_localcert_to_dat(unsigned char **local_data,stu_usr **usr_addr)
{
	free(*local_data);
	*local_data = NULL;

	free(*usr_addr);
	*usr_addr = NULL;
}


/*****************************************************************
* function:		uppercert_to_dat
* description:  uppercert to dat
* param1:     	int fd	:	spi fd;	(input)
* param2:     	stu_upper_data *upper_cert	: all upper stu;	(input)
* param3:     	unsigned char *upper_data	: all local chara;	(output)
* param4:     	int num: user num	(input)
*
* return:    	1          
* others:		
* date:       	2017/07/11			
* author:     	Hangchang Xu
******************************************************************/ 
int tg_uppercert_to_dat(int fd,stu_upper_data *upper_cert,unsigned char *upper_data,int num)
{
	int i;
	int sizeFeature3 = TZD_LENGTH * 3;
	int ret_spi;
	for(i = 0;i<num;i++)
	{
		/********************\u8BC1\u4E66FPGA\u89E3\u5BC6************************/

		upper_cert->cert_type = upper_cert->cert_data.cert_type;
		upper_cert->cert_id = upper_cert->cert_data.user_id;
		memcpy(upper_data+i*sizeFeature3,(upper_cert+i)->cert_data.chara,sizeFeature3);
	}
	return 1;
}


/*****************************************************************
* function:		tg_path_to_info
* description:  ��ȡ֤����Ϣ
* param1:     	char *path	:	֤��·��;	(input)
* param2:     	char *name  : 	֤����;	(output)
* param3:     	int *id 	:	֤��id;	(output)
* param4:     	int *type	:	֤������	(output)
*
* return:    	1   :�ɹ� 
				-1:ʧ��
* others:		
* date:       	2017/11/09			
* author:     	
******************************************************************/
int tg_path_to_info(char *path,char *name,int *id,int *type)
{
	char *temp;
	char *str_num;
	int i = 0,j;
	
	if(0 == strncmp("/etc/tg/cert/super/",path,19))
	{
		*type = 0;
		j = 19;
	}
	else if(0 == strncmp("/etc/tg/cert/mgr/",path,17))
	{
		*type = 1;
		j = 17;
	}
	else if(0 == strncmp("/etc/tg/cert/user/",path,18))
	{
		*type = 2;
		j = 18;
	}
	else 
		return -1; 

	temp = path +j;
	while('_' != *temp)
	{
		if('\0' == *temp)
			return -2;
		*(name+i) = *temp++;
		i++;
	}
	*(name+i) = '\0';
	printf("name = %s\n",name);
	str_num = strstr(path,"_");
	str_num ++;
	printf("str_num = %s\n",str_num);

	*id = atoi(str_num);
	printf("num = %d\n",*id);

	return 1;
}

/*****************************************************************
* function:		tg_delete_file
* description:  ɾ���ļ�
* param1:     	char *path	:	�ļ�·��;	(input)
*
* return:    	0:�ɹ� 
*				����:ʧ��
* others:		
* date:       	2017/11/09			
* author:     	
******************************************************************/
int tg_delete_file(char *path)
{
	int ret;
	FILE *p = fopen(path, "w");  
    if (NULL == p){
    	printf("tg_delete_file file open Fail!\n");
        return -2;        
    }
    fclose(path);
	ret = remove(path);
    return ret;
}





