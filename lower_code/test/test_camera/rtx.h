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
	char 	reserve_char_32[32];//Ԥ������
	char	serial_num[32];		//���к�
	char	ip_addr[32];		//IP��ַ
	char	mac_addr[32];		//MAC��ַ
	char	md5[32];			//���ݼ���õ���MD5
	char	random_num[32];		//�����
	char	cert_name[32];		//֤������
	char 	reserve_char[736];	//Ԥ������
}TG_package;//ָ���


typedef struct {
	char	user_name[32];				//�û���
	int 	user_id;				//�û�ID
	int 	cert_type;				//֤������
	char	reserve[24];			//Ԥ�� �����ֽڶ���
	char 	cert_valid_date[32];		//֤����Ч��
	unsigned char chara[4096];			//������ָ������ 3*1296=3888 Ԥ��4k
	char		key[20*96];				//��̬������Կ+���������Կ  (96+96)*10
	char	crc[2];						//crc
}TG_cert;//֤�飻6116B	//��Ϊ��int���ͣ����԰�4�ֽڶ��룬�˽ṹ���С��Ϊ6116


typedef struct {
	char	dev_key[32];				//�豸��Կ
	char	crc[2];						//crc
}TG_dev_key;//�豸��Կ��34B


typedef struct {
	unsigned char enroll_chara[4096];
	char new_reg_path[50];					//crc
}TG_enroll_data;//new user data��


extern int wd_fd;

int writeComm(int fd, char* buf, int len);

int readComm(int fd, char* buf, int len, int timeout );

int Compute_string_md5(unsigned char *dest_str, unsigned int dest_len, char *md5_str);

int file_size(char* filename) ;

int sendPackage(int fd,TG_package *pack);

int sendDataPackage(int fd,char *data,int len);

int recvPackage(int fd,TG_package *pack,int timeout);

int recvDataPackage(int fd,char *data,int len,int timeout);

int init_watchdog(int timeout);

int feed_watchdog();

int release_watchdog();

int TG_NetSendPackage(int fd,TG_package* pack,char* buf);

int TG_NetRecvPackage(int fd,TG_package* pack,char* buf);

int net_client_connect(char *addr,int port);



#endif  
