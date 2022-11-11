#include "daemon_init.h"

extern int daemon_proc;

int daemon_init(const char *pname,int facility){
	int i;
	pid_t pid;
	
	if((pid=fork())<0)
		return -1;
	else if(pid)
		_exit(0); //parent terminates

	//child 1 continue
	if(setsid()<0)
		return -1;
	
	signal(SIGHUP,SIG_IGN);
	if((pid=fork())<0)
		return -1;
	else if(pid)
		_exit(0);

	//child 2 continue
	daemon_proc=1;//for err_xxx() functions
	chdir("/");//change working directory
	//close off file description
	for(i=0;i<MAXFD;i++){
		close(i);
	}

	//redirect stdin stdout stderr to /dev/null
	open("/dev/null",O_RDONLY);
	open("/dev/null",O_RDWR);
	open("/dev/null",O_RDWR);

	openlog(pname,LOG_PID,facility);

	return 0;
}

