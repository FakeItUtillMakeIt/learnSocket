#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "Buffer.h"
#include <unistd.h>
#include <string.h>

#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop,Socket *_sock):loop(_loop),sock(_sock),channel(NULL),readBuffer(NULL),inBuffer(new std::string()){
	channel=new Channel(loop,sock->getfd());
	std::function<void()> cb=std::bind(&Connection::echo,this,sock->getfd());
	channel->setReadCallback(cb);
	channel->enableReading();
	channel->useET();
	channel->setUseThreadPool(true);

	readBuffer=new Buffer();
}

Connection::~Connection(){
	delete channel;
	delete sock;
	delete readBuffer;
}

void Connection::echo(int sockfd) {
	char buf[READ_BUFFER];
	while(true){
		bzero(buf,READ_BUFFER);
		ssize_t bytes_read=read(sockfd,buf,READ_BUFFER);
		if(bytes_read>0){
			//printf("message from fd:%d,data:%s\n",sockfd,buf);
			//write(sockfd,buf,READ_BUFFER);
			readBuffer->append(buf,bytes_read);
		}
		else if(bytes_read==-1 && errno==EINTR){
			printf("continue reading\n");
			continue;
		}
		else if(bytes_read==-1 &&((errno==EAGAIN)||(errno==EWOULDBLOCK))){
			printf("finish reading once,errno:%d\n",errno);
			printf("message from client fd:%d,data:%s\n",sockfd,readBuffer->c_str());
			write(sockfd,readBuffer->c_str(),readBuffer->size());
			readBuffer->clear();
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


void Connection::send(int sockfd){
	char buf[readBuffer.size()];
	strcpy(buf,readBuffer.c_str());
	int data_size=readBuffer.size();
	int data_left=data_size;

	while(data_left >0){
		ssize_t bytes_write=write(sockfd,buf+data_size - data_left,data_left);
		if(bytes_write == -1 && errno == EAGAIN){
			break;
		}
		data_left-=bytes_write;
	}
}
