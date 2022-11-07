#include "tcp_connect.h"

#define MAX_LEN 1024

int main(int argc,char** argv){
	
	int sockfd,n;
	char recvline[MAX_LEN+1];
	socklen_t len;
	struct sockaddr_storage ss;

	if(argc!=3){
		cout<<"usage:daytimetcpcli <hostname/IPaddress> <service/Port>"<<endl;
		return -1;
	}

	sockfd=tcp_connect(argv[1],argv[2]);

	len=sizeof(ss);
	getpeername(sockfd,(struct sockaddr*)&ss,&len);
	char buf[100];
	cout<<"connected to: "<<inet_ntop(AF_INET,(struct sockaddr*)&ss,buf,len)<<endl;

	while((n=read(sockfd,recvline,MAX_LEN))>0){
		recvline[n]=0;
		fputs(recvline,stdout);
	}

	exit(0);
	return 0;
}
