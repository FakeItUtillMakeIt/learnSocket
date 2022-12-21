#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>

#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop,Socket *_sock):loop(_loop),sock(_sock),channel(NULL){
	channel=new Channel(loop,sock->getfd());
	std::function<void()> cb=std::bind(&Connection::echo,this,sock->getfd());
	channel->setCallback(cb);
	channel->enableReading();
}

Connection::~Connection(){
	delete channel;
	delete sock;
}

void Connection::echo(int sockfd) {
	char buf[READ_BUFFER];
	while(true){
		bzero(buf,READ_BUFFER);
		ssize_t bytes_read=read(sockfd,buf,READ_BUFFER);
		if(bytes_read>0){
			printf("message from fd:%d,data:%s\n",sockfd,buf);
			write(sockfd,buf,READ_BUFFER);

		}
		else if(bytes_read==-1 && errno==EINTR){
			printf("continue reading\n");
			continue;
		}
		else if(bytes_read==-1 &&((errno==EAGAIN)||(errno==EWOULDBLOCK))){
			printf("finish reading once,errno:%d\n",errno);
			break;
		}
		else if(bytes_read==0){
			printf("EOF,client fd %d disconnected\n",sockfd);
			deleteConnectionCallback(sock);//guan bi sockfd will remove fd from epoll trees
			break;
		}
	}
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb){
	deleteConnectionCallback=_cb;
}
