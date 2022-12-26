#pragma once

#include <functional>
#include <string.h>
#include <string>

class EventLoop;
class Socket;
class Channel;
class Buffer;

class Connection{
	private:
		EventLoop *loop;
		Socket *sock;
		Channel *channel;

		std::string *inBuffer;
		Buffer *readBuffer;

		std::function<void(Socket*)> deleteConnectionCallback;

	public:
		Connection(EventLoop *_loop,Socket* _sock);
		~Connection();

		void echo(int sockfd);
		void setDeleteConnectionCallback(std::function<void(Socket*)>);
};

