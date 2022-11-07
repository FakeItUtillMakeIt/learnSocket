#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string.h>

using namespace std;

#define MAX__LEN 1024

int main(int argc,char** argv){
	int sockfd,n;
	char recvline[MAX__LEN+1];
	struct in_addr **pptr;
	struct in_addr *inetaddrp[2];
	struct in_addr inetaddr;
	struct hostent *hp;
	struct servent *sp;
	struct sockaddr_in servaddr;

	if(argc!=3)
	{
		cout<<"usage: hostent2 <hostname> <service>"<<endl;
		return -1;
	}

	if((hp=gethostbyname(argv[1]))==NULL){
		if((inet_aton(argv[1],&inetaddr))==0){
			cout<<"hostname error for "<<argv[1]<<hstrerror(h_errno)<<endl;
			return -1;
		}
		else{
			inetaddrp[0]=&inetaddr;
			inetaddrp[1]=NULL;
			pptr=inetaddrp;
		}
	}
	else{
		pptr=(struct in_addr**)hp->h_addr_list;
	}

	if((sp=getservbyname(argv[2],"tcp"))==NULL){
		cout<<"getservbyname error for "<<argv[2]<<endl;
		return -1;
	}

	for(;*pptr!=NULL;pptr++){
		sockfd=socket(AF_INET,SOCK_STREAM,0);
		memset(&servaddr,0,sizeof(servaddr));
		servaddr.sin_family=AF_INET;
		servaddr.sin_port=sp->s_port;
		cout<<"port:"<<htons(servaddr.sin_port)<<endl;
		//memcpy(&servaddr.sin_addr,*pptr,sizeof(struct in_addr));
		inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
		char buf[100];
		cout<<"trying ip address:"<<inet_ntop(AF_INET,(struct sockaddr*)&servaddr.sin_addr,buf,sizeof(servaddr))<<endl;

		if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(struct sockaddr_in))==0){
			break;
		}
		cout<<"connect error"<<endl;
		close(sockfd);
		return -1;
	}

	if(*pptr==NULL){
		cout<<"unable to connect"<<endl;
		return -1;
	}

	while((n=read(sockfd,recvline,MAX__LEN))>0){
		recvline[n]=0;
		fputs(recvline,stdout);
	}

	exit(0);
	return 0;
}
