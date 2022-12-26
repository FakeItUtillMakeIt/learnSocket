#pragma once

#include <sys/epoll.h>
#include <functional>

class EventLoop;

class Channel{
	private:
		EventLoop* loop;
		int fd;
		uint32_t events;
		uint32_t ready;
		bool inEpoll;
		bool useThreadPool;

		std::function<void()> readCallback;
		std::function<void()> writeCallback;

	public:
		Channel(EventLoop *_loop,int _fd);
		~Channel();

		void handleEvent();
		void enableReading();

		int getfd();
		uint32_t getEvents();
		uint32_t getReady();
		bool getInEpoll();
		void setInEpoll(bool _in=true);

		void setReady(uint32_t);

		void useET();

		void setReadCallback(std::function<void()>);
		void setUseThreadPool(bool use=true);
};
