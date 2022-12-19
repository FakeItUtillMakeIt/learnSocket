
#include <sys/epoll.h>
#include <vector>

class Epoll{
	public:
	
		Epoll();
		~Epoll();
	public:
		void addfd(int fd,uint32_t op);
		std::vector<struct epoll_event> poll(int timeout=-1);

	private:
		int epollfd;
		struct epoll_event* events;



};
