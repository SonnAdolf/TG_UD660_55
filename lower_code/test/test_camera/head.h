#ifndef HEAD_H 
#define HEAD_H 

#define TG_DEBUG
//#define REG_DEBUG



//CMD1;no date package
#define DEV_KEY 0x0001				//设备密钥； 		PC->ARM
#define DEV_KEY_SET_SUCCESS 0x0002			//设备密钥设置成功；  ARM->PC
#define DEV_KEY_SET_FAILED 0x0003		//设备密钥设置失败；  ARM->PC


#define	RAN_NUM_REQ			0x0006		//随机数请求；	PC->ARM
#define	RAN_NUM				0x0007	//随机数；	ARM->PC

#define	RESEND_CERT_REQ		0x0008	//请求重新传输证书，注册完的超管或管理员证书；	PC->ARM
#define	CERT_INFO_REQ		0x0020	//请求传输h3上存储的各类证书个数,信息，对应 CERT_NUM；	PC->ARM
#define	CERT_DATA_REQ		0x0021	//请求传输h3上存储的各类证书；	PC->ARM



#define	UPPER_CERT_CONFIRM			0x0022	//上位机收到证书且CRC正确，删除原来的证书；	PC->ARM
#define	UPPER_CERT_NO_CONFIRM		0x0023	//上位机没收到证书或者CRC错误，重新传输验证成功的本地证书；	PC->ARM
/*************************************/
#define	REG_CERT_RESEND		0x0024	//下位机注册完传给上位机的证书CRC错误，重新注册的证书；	PC->ARM


#define INFO_DATA_CRC_ERR		0x0025	//下位机收到的 证书密钥+人员信息 的CRC错误	ARM->PC
#define CERT_DATA_B_CRC_RIGHT	0x0027	//上位机导入到H3中的证书CRC正确；ARM->PC
#define CERT_DATA_B_CRC_ERR		0x0028	//上位机导入到H3中的证书CRC错误；ARM->PC


#define	CERT_NUM			0x0030	//h3上存储的各类证书个数,对应 CERT_INFO_REQ；	ARM->PC


#define ENROLL_REQ 			0x0100		//注册请求；		PC->ARM
#define	ENROLL_SUCCESS		0x0101		//注册成功；		ARM->PC
#define ENROLL_FAILED		0x0102		//注册失败；		ARM->PC	

#define VALIDATE_LOCAL_REQ 		0x0110		//本地证书(H3)验证请求；		PC->ARM
#define	VALIDATE_LOCAL_SUCCESS	0x0111		//本地证书(H3)验证成功；		ARM->PC
#define VALIDATE_LOCAL_FAILED		0x0112		//本地证书(H3)验证失败；		ARM->PC	

#define	VALIDATE_UPPER_SUCCESS	0x0114		//上位机证书验证成功；		ARM->PC
#define VALIDATE_UPPER_FAILED		0x0115		//上位机证书验证失败；		ARM->PC	

#define VALIDATE_UPPER_CERT_START 		0x0116		//上位机证书验证,上位机开始传输证书		PC->ARM
#define VALIDATE_UPPER_CERT_END 		0x0117		//上位机证书验证,上位机结束传输证书,传输结束自动开始验证		PC->ARM
#define VALIDATE_UPPER_CERT_DATA 		0x0118		//上位机证书验证,上位机传输证书(单个证书传输)		PC->ARM
#define VALIDATE_UPPER_END				0x0119		//上位机证书验证结束；		PC->ARM

#define DELETE_REQ 		0x0120		//删除请求；		PC->ARM
#define	DELETE_SUCCESS	0x0121		//删除成功；		ARM->PC
#define DELETE_FAILED		0x0122		//删除失败；		ARM->PC	



#define HEART_BEAT 0x6666			//心跳包；ARM->PC

//CMD1;date package
#define	INFO_DATA	0x1000				//证书密钥+人员信息+CRC；PC->ARM
#define CERT_DATA_A	0x1001				//管理员注册完的证书+随机数；ARM->PC
#define CERT_DATA_B	0x1002				//上位机的证书(导入到H3中)；PC->ARM
#define CERT_DATA_C	0x1003				//存在H3上的证书(新证书导入到PC中)；ARM->PC



//CERT_TYPE
#define SUPER_MGR	0x00	//超级管理员
#define MGR		0x01		//管理员
#define USR		0x02		//用户




#define DEV_KEY_LENGTH 32
#define RAN_NUM_LENGTH 32

#define CERT_LENGTH 6114

#define DEV_NAME "UD55"	
#define SERIAL_NUM "04005035c2185831020e"

#define INFO_FILE "/etc/tg/info"

#define SUP_MGR_CERT_PATH "/etc/tg/cert/super"
#define MGR_CERT_PATH "/etc/tg/cert/mgr"
#define USER_CERT_PATH "/etc/tg/cert/user"

#define CONFIG_PATH "/etc/config" 
#define DEV_KEY_PATH "/etc/tg/dev_key" 
#define RAN_NUM_PATH "/etc/tg/ran_num" 


#define DB_ADDR "../dat"






#endif  

