

#include "udp_client.h"


int udp_client(const char* host,const char* serv,struct sockaddr **saptr,socklen_t *lenp ){
	int sockfd,n;
	struct addrinfo hints,*res,*ressave;

	bzero(&hints,sizeof(hints));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;

	if((n=getaddrinfo(host,serv,&hints,&res))!=0){
		cout<<"udp_client error for :"<<host<<serv<<gai_strerror(n)<<endl;
		return -1;
	}

	ressave=res;
	do{
		sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if(sockfd>=0){
			break;
		}

	}while((res=res->ai_next));

	if(res==NULL){
		cout<<"udp_client error for:"<<host<<serv<<endl;
	}

	*saptr=(struct sockaddr*)malloc(res->ai_addrlen);
	memcpy(*saptr,res->ai_addr,res->ai_addrlen);
	*lenp=res->ai_addrlen;

	freeaddrinfo(ressave);

	return sockfd;
}
