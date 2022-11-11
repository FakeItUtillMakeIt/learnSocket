#include "udp_server.h"
#include <time.h>

#define MAX_LINE 1024

int main(int argc,char **argv){
	
	int sockfd;
	ssize_t n;
	char buff[MAX_LINE];
	time_t ticks;

	socklen_t len;
	struct sockaddr_storage cliaddr;

	if(argc==2){
		sockfd=udp_server(NULL,argv[1],NULL);
	}
	else if(argc==3){
		sockfd=udp_server(argv[1],argv[2],NULL);
	}
	else{
		cout<<"usage:daytimeudpserv [<host>] <port/service>"<<endl;
		return -1;
	}

	while(true){
		len=sizeof(cliaddr);
		n=recvfrom(sockfd,buff,MAX_LINE,0,(struct sockaddr*)&cliaddr,&len);
		char buf[100];
		cout<<"datagram from:"<<inet_ntop(AF_INET,(struct sockaddr*)&cliaddr,buf,len);
		ticks=time(NULL);
		snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
		sendto(sockfd,buff,strlen(buff),0,(struct sockaddr*)&cliaddr,len);
	}

	return 0;
}
