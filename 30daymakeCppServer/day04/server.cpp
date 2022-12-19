#include "Socket.h"
#include "Epoll.h"

#define MAX_BUF 1024


int main(int argc,const char **argv){

	Socket* serv_sock=new Socket();
	InetAddress* serv_addr=new InetAddress("127.0.0.1",8888);

	serv_sock->bind_(serv_addr);
	serv_sock->listen_();
	

	Epoll *ep=new Epoll();
	serv_sock->setnonblocking();
	ep->addfd(serv_sock->getfd(),EPOLLIN|EPOLLET);

	char *buf[MAX_BUF];

	InetAddress* clnt_addr=NULL;
	Socket* clnt_sock=NULL;

	while(true){
		std::vector<epoll_event> events=ep->poll();
		int nfds=events.size();
		for(int i=0;i<nfds;i++){
			if(events[i].data.fd == serv_sock->getfd()){//new client
				
				//InetAddress* clnt_addr=new InetAddress();
				//Socket* clnt_sock=new Socket(serv_sock->accept_(clnt_addr));
				
				clnt_addr=new InetAddress();
				clnt_sock=new Socket(serv_sock->accept_(clnt_addr));

				printf("new client fd %d,IP:%s,port:%d\n",clnt_sock->getfd(),inet_ntoa(clnt_addr->sock_addr.sin_addr),ntohs(clnt_addr->sock_addr.sin_port));
				clnt_sock->setnonblocking();
				ep->addfd(clnt_sock->getfd(),EPOLLIN | EPOLLET);

			}
			else if(events[i].events & EPOLLIN){
				//read
				int read_bytes=read(events[i].data.fd,buf,MAX_BUF);
				if(read_bytes>0){
					printf("read from client:%s\n",buf);
					write(events[i].data.fd,buf,MAX_BUF);
				}
				else if(read_bytes<=0){
					close(events[i].data.fd);
					break;
				}
			}
			else{
				printf("other thing");
			}
		}
	}


	delete clnt_addr;
	delete clnt_sock;

	delete serv_sock;
	delete serv_addr;

	return 0;
}
