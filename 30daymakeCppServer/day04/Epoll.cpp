

#include "Epoll.h"
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


std::vector< epoll_event> Epoll::poll(int timeout){
	std::vector<epoll_event> activateEvent;
	int nfds=epoll_wait(epollfd,events,MAX_EVENTS,timeout);
	errif(nfds==-1,"epoll_wait error");
	for(int i=0;i<nfds;i++){
		activateEvent.push_back(events[i]);
	}
	return activateEvent;
}

