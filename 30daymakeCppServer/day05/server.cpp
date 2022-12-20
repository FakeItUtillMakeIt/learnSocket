#include "Socket.h"
#include "Epoll.h"
#include "Channel.h"
#include "errno.h"


#define MAX_BUF 1024

void handleReadEvent(int sockfd);

int main(int argc,const char **argv){

	Socket* serv_sock=new Socket();
	InetAddress* serv_addr=new InetAddress("127.0.0.1",8888);

	serv_sock->bind_(serv_addr);
	serv_sock->listen_();
	

	Epoll *ep=new Epoll();
	serv_sock->setnonblocking();
	Channel *servChannel=new Channel(ep,serv_sock->getfd());
	servChannel->enableReading();

	char *buf[MAX_BUF];


	while(true){
		std::vector<Channel*> events=ep->poll();
		int nfds=events.size();
		for(int i=0;i<nfds;i++){
			if(events[i]->getfd() == serv_sock->getfd()){//new client
				
				InetAddress* clnt_addr=new InetAddress();
				Socket* clnt_sock=new Socket(serv_sock->accept_(clnt_addr));
				
				printf("new client fd %d,IP:%s,port:%d\n",clnt_sock->getfd(),inet_ntoa(clnt_addr->sock_addr.sin_addr),ntohs(clnt_addr->sock_addr.sin_port));
				clnt_sock->setnonblocking();
				Channel *clntChannel=new Channel(ep,clnt_sock->getfd());
				clntChannel->enableReading();

			}
			else if(events[i]->getRevents() & EPOLLIN){//readable event
				handleReadEvent(events[i]->getfd());	
			}
			else{
				printf("something else happened\n");
			}
		}
	}



	delete serv_sock;
	delete serv_addr;

	return 0;
}


void handleReadEvent(int sockfd){
	char buf[MAX_BUF];
	while(true){
		bzero(buf,MAX_BUF);
		ssize_t bytes_read=read(sockfd,buf,MAX_BUF);
		if(bytes_read>0){
			printf("message from client fd %d: data:%s\n",sockfd,buf);
			write(sockfd,buf,MAX_BUF);
		}
		else if(bytes_read==-1 &&errno==EINTR){//kehuduan zhengchang zhongduan
			printf("coutinue reading\n");
			continue;	
		}
		else if(bytes_read==-1 && ((errno==EAGAIN) || (errno==EWOULDBLOCK))){//nonblocking io,represent read over
		printf("finish reading once,errno:%d\n",errno);
		break;
		}
		else if(bytes_read==0){//EOF terminate close connection
			printf("EOF,client fd :%d disconnected",sockfd);
			close(sockfd);
			break;
		}
	}
}
