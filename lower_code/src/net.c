#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_ntoa()������ͷ�ļ�
#include <unistd.h>   
#include <pthread.h>
#include <signal.h>

#include "rtx.h"


char send_buf[BUFSIZE],recv_buf[BUFSIZE];

/*****************************************************************
* function:		net_client_connect
* description:  ���ӷ�����
* param1:     	char *path	:	ip��ַ;	(input)
* param2:		int port	:	�˿ں�;(input)
* return:    	-1  :ʧ�� 
*				����:�ɹ�
* others:		
* date:       	2017/11/09			
* author:     	
******************************************************************/ 
int net_client_connect(char *addr,int port)
{
	  int sockfd;

	  struct sockaddr_in server_addr; 
	  struct hostent *host;
	  int nbytes;
	
	  if((host=gethostbyname(addr))==NULL)
	  {
		  fprintf(stderr,"Gethostname error\n");
		  return -1;
	  }
	
	  if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:Internet;SOCK_STREAM:TCP
	  {
		  fprintf(stderr,"Socket Error:%s\a\n",strerror(errno));
		  return -1;
	  }
	
	  bzero(&server_addr,sizeof(server_addr)); 
	  server_addr.sin_family=AF_INET; // IPV4
	  server_addr.sin_port=htons(port); 
	  server_addr.sin_addr=*((struct in_addr *)host->h_addr); // 

	  if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
	  {
		  fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));
		  close(sockfd);
		  return -1;
	  }

	  return sockfd;

}

int net_service_init(char *addr,int port)
{
	int sockfd;
	struct sockaddr_in server_addr; //

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:IPV4;SOCK_STREAM:TCP
    {
        fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
        exit(1);
    }

    bzero(&server_addr,sizeof(struct sockaddr_in)); // ��ʼ��,��0
    server_addr.sin_family=AF_INET; // Internet
    if(addr!=NULL)
		server_addr.sin_addr.s_addr=inet_addr(addr); //���ڰ󶨵�һ���̶�IP,inet_addr���ڰ����ּӸ�ʽ��ipת��Ϊ����ip
	else
    	server_addr.sin_addr.s_addr=htonl(INADDR_ANY); // (���������ϵ�long����ת��Ϊ�����ϵ�long����)���κ�����ͨ�� 
    	//INADDR_ANY ��ʾ���Խ�������IP��ַ�����ݣ����󶨵����е�IP

    server_addr.sin_port=htons(port); // (���������ϵ�short����ת��Ϊ�����ϵ�short����)�˿ں�

	unsigned int value = 0x1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(void *)&value,sizeof(value)); 

    if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
        exit(1);
    }

    if(listen(sockfd,5)==-1)
    {
        fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
        exit(1);
    }

	return sockfd;

}
int net_service_accept(int sockfd)
{
    int new_fd;
    struct sockaddr_in client_addr; 
    int sin_size;

	sin_size=sizeof(struct sockaddr_in);
	if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)
	{
		fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
		return -1;
	}
	fprintf(stderr,"Server get connection from %s\n",inet_ntoa(client_addr.sin_addr)); // �������ַת����.�ַ���������ӡ������ն�
	
	return new_fd;
}

//--------------------------------------------------------------------------------------------

/*****************************************************************
* function:		TG_NetSendPackage
* description:  ���͸�PC����
* param1:     	int fd				:	�����豸�ļ�������	(input)
* param2:		TG_package* pack	:	ָ��� (input)
* param3:		char* buf			:	���ݰ� (input)

* return:    	1   : �ɹ�
*				����: ʧ��
* others:		
* date:       	2017/11/09			
* author:     	
******************************************************************/ 
int TG_NetSendPackage(int fd,TG_package* pack,char* buf)
{
	int ret=1;
    if(fd<0) 
		return -5;
	if(0 == pack->length ){//normal cmd data
		if(sendPackage(fd,pack)<0)
		ret=-4;
	}
	else{//send file or data block
		if(sendPackage(fd,pack)>=0){
			ret=sendDataPackage(fd,buf,pack->length); 
		}
		else ret=-4;
	}
	return ret;
}


/*****************************************************************
* function:		TG_NetRecvPackage
* description:  ��������PC������
* param1:     	int fd				:	�����豸�ļ�������	(input)
* param2:		TG_package* pack	:	ָ��� (output)
* param3:		char* buf			:	���ݰ� (output)

* return:    	1   : �ɹ�
*				����: ʧ��
* others:		
* date:       	2017/11/09			
* author:     	
******************************************************************/
int TG_NetRecvPackage(int fd,TG_package* pack,char*buf)
{
	int len,ret=1;
	memset(pack,0,sizeof(TG_package));
    if(fd<0) 
		return -5;
	len=recvPackage(fd,pack,200);
//	printf("len1 = %d \n",len);
	if(len==0) 
		return 0;
	if(len<0) 
		return -2;
//	printf("pack->length = %d\n",pack->length);
	if(pack->length>0){//file or data block receive
			len = recvDataPackage(fd,buf,pack->length,500);
			if(len == pack->length){//success
				ret=1;
				printf("success!len = %d \n",len);
			} 
			else {//fail
				ret=-3;
				printf("fail! len = %d !\n",len);
			}
	}

//	printf("pack->id = %d\n",pack->id);
//	printf("pack->cmd1 = %d\n",pack->cmd1);
	if(pack->id==0 && pack->cmd1==0)
		ret=-6;
	return ret;			
}


int TG_HidSendPackage(int fd,TG_package* pack,char* buf)
{
	int ret=1;
	if(fd<0) 
		return -5;
	if(0 == pack->length ){//normal cmd data
		if(sendPackage(fd,pack)<0)
		ret=-4;
	}
	else{//send file or data block
		if(sendPackage(fd,pack)>=0){
			ret=sendDataPackage(fd,buf,pack->length); 
		}
		else 
			ret=-4;
	}
	return ret;
}




int TG_HidRecvPackage(int fd,TG_package* pack,char*buf)
{

//pack.length ��?????= ????????2
	int len,ret=1;
	char *tmp = NULL;
	char tmp_pack[2052] = {0};
	memset(pack,0,sizeof(TG_package));
	if(fd<0) 
		return -5;
//	len=recvPackage(fd,pack,200);
	len=recvPackage(fd,tmp_pack,100);
	StrToHex(pack,tmp_pack,sizeof(TG_package));

//	printf("hid cmd recvPackage len = %d \n",len);
	if(len==0) 
		return 0;
	if(len<0) 
		return -2;
	if(pack->length>0){//file or data block receive
			printf("recvpack->length = %d\n",pack->length);
			tmp=(char *)malloc(pack->length*2*2+4);
			if(tmp==NULL) 
				return -1;
			recvDataPackage(fd,tmp,pack->length*2,500);
			StrToHex(buf,tmp,pack->length);
			write_data_hex(buf,pack->length,"recv2.dat");
			printf("success!len = %d \n",len);
			ret=1;		
	}
	
//	printf("pack->id = %d\n",pack->id);
//	printf("pack->cmd1 = %d\n",pack->cmd1);
	if(pack->id==0 && pack->cmd1==0)
		ret=-6;
	free(tmp);
	tmp = NULL;
	
	return ret; 		
}
	



