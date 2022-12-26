#pragma once

#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll{
	public:
	
		Epoll();
		~Epoll();
	public:
		void addfd(int fd,uint32_t op);
//		std::vector<struct epoll_event> poll(int timeout=-1);
		std::vector<Channel*> poll(int timeout=-1);
		void updateChannel(Channel*);
		void deleteChannel(Channel*);
	private:
		int epollfd;
		struct epoll_event* events;



};
