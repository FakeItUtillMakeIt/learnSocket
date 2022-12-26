
#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *_loop,int _fd){
	loop=_loop;
	
	fd=_fd;
	events=0;
	ready=0;
	inEpoll=false;

	useThreadPool=true;
}

Channel::~Channel(){
	if(fd!=-1){
		close(fd);
		fd=-1;
	}	
}

void Channel::handleEvent(){
	if(ready & (EPOLLIN | EPOLLPRI)){
		if(useThreadPool)
			loop->addThread(readCallback);
		else
			readCallback();
	}

	if(ready & (EPOLLOUT)){
		if(useThreadPool)
			loop->addThread(writeCallback);
		else
			writeCallback();
	}
}

void Channel::enableReading(){
	events|=EPOLLIN|EPOLLPRI;
	loop->updateChannel(this);
}

void Channel::useET(){
	events|=EPOLLET;
	loop->updateChannel(this);
}

int Channel::getfd(){
	return fd;
}

uint32_t Channel::getEvents(){
	return events;
}

uint32_t Channel::getReady(){
	return ready;
}

bool Channel::getInEpoll(){
	return inEpoll;
}

void Channel::setInEpoll(bool _in){
	inEpoll=_in;
}

void Channel::setReady(uint32_t _ev){
	revents=_ev;
}

void Channel::setReadCallback(std::function<void()> _cb){
	callback=_cb;
}

void Channel::setUseThreadPool(bool use){
	useThreadPool=use;
}
