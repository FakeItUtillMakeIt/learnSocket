

#include "Epoll.h"
#include "Channel.h"
#include <string.h>
#include <unistd.h>
#include "errif.h"

#define MAX_EVENTS 1000

Epoll::Epoll():epollfd(-1),events(NULL){
	epollfd=epoll_create(MAX_EVENTS);
	errif(epollfd==-1,"epoll create error");
	events=new epoll_event[MAX_EVENTS];
	bzero(events,sizeof(*events)*MAX_EVENTS);
}

Epoll::~Epoll(){

	if(epollfd!=-1){
		close(epollfd);
		epollfd=-1;
	}
	delete[] events;
}

void Epoll::addfd(int fd,uint32_t op){
	struct epoll_event ev;
	bzero(&ev,sizeof(ev));
	ev.data.fd=fd;
	ev.events=op;
	errif(epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev)==-1,"epoll_ctl error");
}


std::vector<Channel*> Epoll::poll(int timeout){
	std::vector<Channel*> activateEvent;
	int nfds=epoll_wait(epollfd,events,MAX_EVENTS,timeout);
	errif(nfds==-1,"epoll_wait error");
	for(int i=0;i<nfds;i++){
		Channel *ch=(Channel*)events[i].data.ptr;
		ch->setRevents(events[i].events);
		activateEvent.push_back(ch);
	}
	return activateEvent;
}


void Epoll::updateChannel(Channel *_ch){
	int fd=_ch->getfd();
	struct epoll_event ev;
	bzero(&ev,sizeof(ev));
	ev.data.ptr=_ch;
	ev.events=_ch->getEvents();
	if(!_ch->getInEpoll()){
		errif(epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev)==-1,"epoll add error");
		_ch->setInEpoll();

	}
	else{
		errif(epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev)==-1,"epoll modify error");
	}
}

void Epoll::deleteChannel(Channel *_ch){
	int fd=channel->getfd();
	errif(epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL)==-1,"epoll delete error");
	channel->setInEpoll(false);
}
