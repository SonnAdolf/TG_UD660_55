#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	int fd;
	char pwm_cmd[10] = {0};
	int num;
	
	fd = open("/dev/gpio_ctl",O_RDWR);
	printf("fd = %d\n",fd);

	while(1)
	{
		printf("please input sound_ctl:\n");
		scanf("%x",&num);
//		buf = num;
		*pwm_cmd = 's';
		*(pwm_cmd + 1) = num;
			
//		printf("pwm_cmd = %s\n",pwm_cmd);
		write(fd,&pwm_cmd,sizeof(pwm_cmd));
		memset(pwm_cmd,0,10);
//		sleep(5);
	}
	close(fd);
	
	printf("---------end---------\n");
	return 0;
}
