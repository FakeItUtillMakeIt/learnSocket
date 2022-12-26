#pragma  once

#include <functional>

class Epoll;
class Channel;
class ThreadPool;

class EventLoop{
	private:
		Epoll *ep;
		bool quit;

		ThreadPool *threadPool;
	public:
		EventLoop();
		~EventLoop();

		void loop();
		void updateChannel(Channel*);

		void addThread(std::function<void()>);
};
