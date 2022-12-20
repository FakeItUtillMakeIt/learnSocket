
#include "Channel.h"
#include "Epoll.h"

Channel::Channel(Epoll *_ep,int _fd){
	ep=_ep;
	fd=_fd;
	events=0;
	revents=0;
	inEpoll=false;

	callback=NULL;
}

Channel::~Channel(){
	
}

void Channel::handleEvent(){

}

void Channel::enableReading(){
	events=EPOLLIN|EPOLLET;
	ep->updateChannel(this);
}

int Channel::getfd(){
	return fd;
}

uint32_t Channel::getEvents(){
	return events;
}

uint32_t Channel::getRevents(){
	return revents;
}

bool Channel::getInEpoll(){
	return inEpoll;
}

void Channel::setInEpoll(){
	inEpoll=true;
}

void Channel::setRevents(uint32_t _ev){
	revents=_ev;
}

