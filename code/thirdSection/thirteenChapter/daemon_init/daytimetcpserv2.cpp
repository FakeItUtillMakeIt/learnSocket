#include "tcp_listen.h"
#include "daemon_init.h"
#include "error1.h"
#include <time.h>

#define MAX_LEN 1024

int main(int argc,char** argv){
	
	int listenfd,connfd;
	socklen_t len;
	char buff[MAX_LEN];
	time_t ticks;
	struct sockaddr_storage cliaddr;

	if(argc==2){	
		daemon_init(argv[0],0);
		listenfd=tcp_listen(NULL,argv[1],NULL);
	}else if(argc==3){
		daemon_init(argv[0],0);
		listenfd=tcp_listen(argv[1],argv[2],NULL);
	}
	else{
		cout<<"usage: daytimetcpserv <IPaddress/hostname> <service/port>"<<endl;
		return -1;
	}


	while(true){
		len=sizeof(cliaddr);
		connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&len);
		char buf1[100];
		//cout<<"connection from :"<<inet_ntop(AF_INET,(struct sockaddr*)&cliaddr,buf,len)<<endl;
		err_msg("connection from %s",inet_ntop(AF_INET,(struct sockaddr*)&cliaddr,buf1,len));
		ticks=time(NULL);
		snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));

		write(connfd,buff,strlen(buff));

		close(connfd);
	}

	return 0;
}
