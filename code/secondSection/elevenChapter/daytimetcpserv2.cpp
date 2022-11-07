#include "tcp_listen.h"
#include <time.h>

#define MAX_LEN 1024

int main(int argc,char** argv){
	
	int listenfd,connfd;
	socklen_t len;
	char buff[MAX_LEN];
	time_t ticks;
	struct sockaddr_storage cliaddr;

	if(argc==2){	
		listenfd=tcp_listen(NULL,argv[1],NULL);
	}else if(argc==3){
		listenfd=tcp_listen(argv[1],argv[2],NULL);
	}
	else{
		cout<<"usage: daytimetcpserv <IPaddress/hostname> <service/port>"<<endl;
		return -1;
	}


	while(true){
		len=sizeof(cliaddr);
		connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&len);
		char buf[100];
		cout<<"connection from :"<<inet_ntop(AF_INET,(struct sockaddr*)&cliaddr,buf,len)<<endl;
		
		ticks=time(NULL);
		snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));

		write(connfd,buff,strlen(buff));

		close(connfd);
	}

	return 0;
}
