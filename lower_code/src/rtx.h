#ifndef RTX_H 
#define RTX_H 

#define BUFSIZE 1024*1024*16
#define PACKAGE_SIZE 1024

#define PORT 3333
#define SERVICE_ADDR "192.168.1.111"


typedef struct {
	char	device_name[32];	//�豸��
	int 	id;					//ID
	int		cmd1;				//ָ��1
	int		cmd2;				//ָ��2
	int 	length;				//���ݳ���
	int		cert_type;			//֤������
	int		sup_mgr_num;		//���س���֤�����
	int		mgr_num;			//���ع���Ա֤�����
	int		usr_num;			//�����û�֤�����
	char	serial_num[32];		//���к�
	char	random_num[32];		//�����
	char	cert_name[32];		//֤������
	char 	reserve_char[96];	//Ԥ������
	char 	reserve_char_768[768];	//Ԥ������
}TG_package;//ָ���  1024B	

typedef struct {
	char	user_name[56];				//�û���
	int 	cert_type;				//֤������
	int 	user_id;				//�û�ID
	char 	cert_valid_date[32];		//֤����Ч��
	unsigned char chara[4096];			//������ָ������ 3*1296=3888 Ԥ��4k
	char		key[20*96];				//��̬������Կ+���������Կ  (96+96)*10
	char	reserve_32[32];					//�ֽڶ���
	char	crc[2];						//crc
	char	reserve_2[2];					//�ֽڶ���
}TG_cert;//֤�飻6148B	//��Ϊ��int���ͣ����԰�4�ֽڶ��룬�˽ṹ���С��Ϊ6148


typedef struct {
	char	dev_key[32];				//�豸��Կ
	char	crc[2];						//crc
}TG_dev_key;//�豸��Կ��34B


typedef struct {
	unsigned char enroll_chara[4096];
	char new_reg_path[256];					//crc
}TG_enroll_data;//new user data��


extern int wd_fd;

int writeComm(int fd, char* buf, int len);

int readComm(int fd, char* buf, int len, int timeout );

int file_size(char* filename) ;

int sendPackage(int fd,TG_package *pack);

int sendDataPackage(int fd,char *data,int len);

int recvPackage(int fd,TG_package *pack,int timeout);

int recvDataPackage(int fd,char *data,int len,int timeout);

void StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen);

void HexToStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen);

int init_watchdog(int timeout);

int feed_watchdog();

int release_watchdog();

int TG_NetSendPackage(int fd,TG_package* pack,char* buf);

int TG_NetRecvPackage(int fd,TG_package* pack,char* buf);

int TG_HidSendPackage(int fd,TG_package* pack,char* buf);

int TG_HidRecvPackage(int fd,TG_package* pack,char*buf);

int net_client_connect(char *addr,int port);

int init_Device();

int release_Device(int fd);




#endif  
