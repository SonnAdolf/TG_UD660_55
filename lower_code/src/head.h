#ifndef HEAD_H 
#define HEAD_H 



/*********************ָ��**********************/
//CMD1;no date package
#define DEV_KEY 0x0001				//�豸��Կ�� 		PC->ARM
#define DEV_KEY_SET_SUCCESS 0x0002			//�豸��Կ���óɹ���  ARM->PC
#define DEV_KEY_SET_FAILED 0x0003		//�豸��Կ����ʧ�ܣ�  ARM->PC

#define	RAN_NUM_REQ			0x0006		//���������	PC->ARM
#define	RAN_NUM_SUCCESS		0x0007	//�������ȡ�ɹ���	ARM->PC
#define	RAN_NUM_FAIL		0x0008	//�������ȡʧ�ܣ�	ARM->PC

#define	CERT_INFO_REQ		0x0020	//������h3�ϴ洢�ĸ���֤�����,��Ϣ����Ӧ CERT_NUM��	PC->ARM
#define	CERT_DATA_REQ		0x0021	//������h3�ϴ洢�ĸ���֤�飻	PC->ARM

#define	UPPER_CERT_CONFIRM			0x0022	//��λ���յ�֤����CRC��ȷ��ɾ��ԭ����֤�飻	PC->ARM
#define	UPPER_CERT_NO_CONFIRM		0x0023	//��λ��û�յ�֤�����CRC�������´�����֤�ɹ��ı���֤�飻	PC->ARM
/*************************************/
#define	REG_CERT_RESEND		0x0024	//��λ��ע���괫����λ����֤��CRC��������ע���֤�飻	PC->ARM

#define INFO_DATA_CRC_ERR		0x0025	//��λ���յ��� ֤����Կ+��Ա��Ϣ ��CRC����	ARM->PC
#define CERT_DATA_B_CRC_RIGHT	0x0027	//��λ�����뵽H3�е�֤��CRC��ȷ��ARM->PC
#define CERT_DATA_B_CRC_ERR		0x0028	//��λ�����뵽H3�е�֤��CRC����ARM->PC
#define	FLOW_CANCEL				0x0029	//�������ڽ����е�ע��ȶ����̣�	PC->ARM


#define	CERT_NUM			0x0030	//h3�ϴ洢�ĸ���֤�����,��Ӧ CERT_INFO_REQ��	ARM->PC
#define	CERT_ENCRYPT		0x0031	//��λ������֤���������	PC->ARM
#define	CERT_DECRYPT		0x0032	//��λ������֤���������	PC->ARM
#define	CERT_ENCRYPT_DONE	0x0033	//��λ������֤�������ɣ�	ARM->PC
#define	CERT_DECRYPT_DONE	0x0034	//��λ������֤�������ɣ�	ARM->PC
#define	CERT_CRYPT_FAIL		0x0035	//��λ������֤��ӽ���ʧ�ܣ�	ARM->PC

#define	GET_LOGINING_CERT	0x0035	//�����ȡ�յ�¼�û���֤�飻			PC->ARM  	ARM->PC

#define ENROLL_REQ 			0x0100		//ע������		PC->ARM
#define	ENROLL_SUCCESS		0x0101		//ע��ɹ���		ARM->PC
#define ENROLL_FAILED		0x0102		//ע��ʧ�ܣ�		ARM->PC	

#define VALIDATE_LOCAL_REQ 		0x0110		//����֤��(H3)��֤����		PC->ARM
#define	VALIDATE_LOCAL_SUCCESS	0x0111		//����֤��(H3)��֤�ɹ���		ARM->PC
#define VALIDATE_LOCAL_FAILED		0x0112		//����֤��(H3)��֤ʧ�ܣ�		ARM->PC	

#define	VALIDATE_UPPER_SUCCESS	0x0114		//��λ��֤����֤�ɹ���		ARM->PC
#define VALIDATE_UPPER_FAILED		0x0115		//��λ��֤����֤ʧ�ܣ�		ARM->PC	

#define VALIDATE_UPPER_CERT_START 		0x0116		//��λ��֤����֤,��λ����ʼ����֤��		PC->ARM
#define VALIDATE_UPPER_CERT_END 		0x0117		//��λ��֤����֤,��λ����������֤��,��������Զ���ʼ��֤		PC->ARM
#define VALIDATE_UPPER_CERT_DATA 		0x0118		//��λ��֤����֤,��λ������֤��(����֤�鴫��)		PC->ARM
#define VALIDATE_UPPER_END				0x0119		//��λ��֤����֤������		PC->ARM

/*
#define VALIDATE_UPPER_CERT_NAME 		0x011A		//��λ��֤����֤,��λ����ʼ����֤��		PC->ARM
#define VALIDATE_UPPER_CERT_DATA 		0x011B		//��λ��֤����֤,��λ����ʼ����֤��		PC->ARM
*/

#define DELETE_REQ 		0x0120		//ɾ������		PC->ARM
#define	DELETE_SUCCESS	0x0121		//ɾ���ɹ���		ARM->PC
#define DELETE_FAILED		0x0122		//ɾ��ʧ�ܣ�		ARM->PC	

#define HEART_BEAT 0x6666			//��������ARM->PC

//CMD1;date package
#define	INFO_DATA	0x1000				//֤����Կ+��Ա��Ϣ+CRC��PC->ARM
#define CERT_DATA_A	0x1001				//����Աע�����֤��+�������ARM->PC
#define CERT_DATA_B	0x1002				//��λ����֤��(���뵽H3��)��PC->ARM
#define CERT_DATA_C	0x1003				//����H3�ϵ�֤��(��֤�鵼�뵽PC��)��ARM->PC
#define CERT_DATA_A_FAIL	0x1004				//����Աע�����֤�����ʧ�ܣ�ARM->PC


//test;date package
#define TEST_REG_ON_DEV    0x0200    //��λ��ע������
#define ONCE_VALIDATING_ON_DEV    0x0201    //��λ��֤��һ����֤����
#define KEEP_VALIDATING_ON_DEV    0x0202    //��λ��֤��������֤����


/*********************�û�����**********************/
#define MGR		0x01		//����Ա
#define USR		0x02		//�û�
#define TEST_USER		0x04		//����


/*********************��Կ���������֤�鳤��**********************/
#define DEV_KEY_LENGTH 32
#define RAN_NUM_LENGTH 32
#define CERT_LENGTH 6148 //6144+2+2



/*********************�ļ���Ŀ¼·��**********************/
#define INFO_FILE "/etc/tg/info"
#define MGR_CERT_PATH "/etc/tg/cert/mgr"
#define USER_CERT_PATH "/etc/tg/cert/user"
#define TSET_USER_CERT_PATH "/etc/tg/cert/test"
#define LOGIN_CERT_PATH "/etc/tg/cert/login_cert.dat"
#define CONFIG_PATH "/etc/config" 
#define DEV_KEY_PATH "/etc/tg/dev_key" 
#define RAN_NUM_PATH "/etc/tg/ran_num" 
#define SN_PATH "/etc/tg/tg_sn.dat" 
#define DB_ADDR "../dat"


#endif  

