#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

void sig_child(int signo){
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0){//3.using waitpid
		std::cout<<"child "<<pid<<" terminated"<<std::endl;
	}
	return ;
}

void str_echo(int sockfd){
	ssize_t n;
	char buf[1024];
again:
	while((n=read(sockfd,buf,1024))>0){
		write(sockfd,buf,n);
	}	
	if(n<0 && errno==EINTR){
		goto again;
	}
	else if(n<0){
		std::cout<<"str_echo :read error"<<std::endl;
	}
}

int main(int argc,char** argv){
	int listenfd,connfd;

	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr,servaddr;
	
	listenfd=socket(AF_INET,SOCK_STREAM,0);
		
	memset(&servaddr,0,sizeof(struct sockaddr_in));

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(10003);
	if(inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr.s_addr)<0){
		std::cout<<"error in inet_pton"<<std::endl;
		return -1;
	}
	
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){
		std::cout<<"error in bind"<<std::endl;
		return -1;
	}

	if(listen(listenfd,10)<0){
		std::cout<<"error in listen"<<std::endl;
		return -1;
	}
	signal(SIGCHLD,sig_child);//1.catch SIGCHLD signal
	while(true){
		clilen=sizeof(cliaddr);
		if((connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&clilen))<0)
		{
			if(errno==EINTR){//2.deal system using when catching signal
				continue;
			}
			std::cout<<"error in bind"<<std::endl;
			continue;
		}
		std::cout<<ntohs(cliaddr.sin_port)<<std::endl;
		if((childpid=fork()==0)){
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}	
		close(connfd);	
	}

	return 0;
}
