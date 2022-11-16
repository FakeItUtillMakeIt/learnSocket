#include "tcp_listen.h"


int tcp_listen(const char* hostname,const char* service,socklen_t* addrlenp){
	int listenfd,n;
	const int on=1;
	struct addrinfo hints,*res,*ressave;
	
	bzero(&hints,sizeof(hints));
	hints.ai_flags=AI_PASSIVE;
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;

	if((n=getaddrinfo(hostname,service,&hints,&res))!=0){
		cout<<"tcp listen error for :"<<hostname<<service<<gai_strerror(n)<<endl;;
		return -1;
	}

	ressave=res;

	do{
		listenfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if(listenfd<0){
			continue;
		}

		setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
		if(bind(listenfd,res->ai_addr,res->ai_addrlen)==0){
			break;//success
		}

		close(listenfd);
	}while((res=res->ai_next)!=NULL);

	if(res==NULL){
		cout<<"tcp listen error for :"<<hostname<<service<<endl;
	}

	listen(listenfd,5);
	if(addrlenp){
		*addrlenp=res->ai_addrlen;
	}

	freeaddrinfo(ressave);

	return listenfd;
}
