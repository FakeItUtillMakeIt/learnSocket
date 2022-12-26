#pragma once

#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server{
	private:
		EventLoop *loop;
		Acceptor *acceptor;

		std::map<int,Connection*> connection;
	public:
		Server(EventLoop*);
		~Server();

		void handleReadEvent(int);
		void newConnection(Socket *serv_sock);
		void deleteConnection(Socket *sock);
};
