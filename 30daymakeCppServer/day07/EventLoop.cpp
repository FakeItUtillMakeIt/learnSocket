#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>


EventLoop::EventLoop():ep(NULL),quit(false){
	ep=new Epoll();
}

EventLoop::~EventLoop(){
	delete ep;
}

void EventLoop::loop(){
	while(!quit){
		std::vector<Channel*> chs=ep->poll();
		for(auto it=chs.begin();it!=chs.end();it++){
			(*it)->handleEvent();
		}
	}
}


void EventLoop::updateChannel(Channel *ch){
	ep->updateChannel(ch);
}
