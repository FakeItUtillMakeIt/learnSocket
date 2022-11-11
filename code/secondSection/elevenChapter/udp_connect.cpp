
#include "udp_connect.h"


int udp_connect(const char *host,const char *serv){
	int sockfd,n;
	struct addrinfo hints,*res,*ressave;

	bzero(&hints,sizeof(hints));

	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;

	if((n=getaddrinfo(host,serv,&hints,&res))!=0){
		cout<<"udp_connect error for :"<<host<<serv<<gai_strerror(n)<<endl;
		return -1;
	}
	ressave=res;

	do{
		sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if(sockfd<0){
			continue;
		}
		if(connect(sockfd,res->ai_addr,res->ai_addrlen)==0){
			break;
		}

		close(sockfd);
	}while((res=res->ai_next)!=NULL);

	if(res==NULL){
		cout<<"udp_connect error for :"<<host<<serv<<endl;
	}
	
	freeaddrinfo(ressave);
	return sockfd;
	
}
