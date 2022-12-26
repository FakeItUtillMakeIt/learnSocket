#pragma once

#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "InetAddress.h"

class InetAddress;

class Socket{
	public:

		Socket();
		Socket(int fd);
		~Socket();
	public:
		void bind_(InetAddress* inetaddr);
		void listen_();
		int accept_(InetAddress* inetaddr);

		int getfd();
		void setMaxfd(int num);

		void setnonblocking();

	private:
		int sockfd;
		int maxfd=1;
};

#endif 
