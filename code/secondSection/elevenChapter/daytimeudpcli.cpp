#include "udp_client.h"

#define MAX_LINE 1024

int main(int argc,char **argv){
	
	int sockfd,n;
	char recvline[MAX_LINE+1];
	socklen_t salen;
	struct sockaddr* sa;

	if(argc!=3){
		cout<<"usage:daytimeudpcli <hostname/Ip> <service/port>"<<endl;
		return -1;
	}

	sockfd=udp_client(argv[1],argv[2],&sa,&salen);
	char buf[100];
	cout<<"sending to:"<<inet_ntop(AF_INET,sa,buf,salen)<<endl;;
	sendto(sockfd,"",1,0,sa,salen);//send 1 byte datagram
	n=recvfrom(sockfd,recvline,MAX_LINE,0,NULL,NULL);
	recvline[n]='\0';
	fputs(recvline,stdout);

	exit(0);
	return 0;
}
