
#include "udp_server.h"


int udp_server(const char *host,const char *serv,socklen_t *lenp){
	int sockfd,n;
	struct addrinfo hints,*res,*ressave;

	bzero(&hints,sizeof(hints));

	hints.ai_flags=AI_PASSIVE;
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

		if(bind(sockfd,res->ai_addr,res->ai_addrlen)==0){
			break;
		}

		close(sockfd);
	}while((res=res->ai_next)!=NULL);

	if(res==NULL){
		cout<<"udp_connect error for :"<<host<<serv<<endl;
	}
	
	if(lenp){
		*lenp=res->ai_addrlen;
	}

	freeaddrinfo(ressave);
	return sockfd;
	
}
