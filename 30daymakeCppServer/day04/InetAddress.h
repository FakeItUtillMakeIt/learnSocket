#ifndef INET_ADDRESS_H
#define INET_ADDRESS_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



class InetAddress{
	public:
		InetAddress();
		InetAddress(const char *ip,int port);
		~InetAddress();

	public:
		socklen_t addr_len;
		struct sockaddr_in sock_addr;
		
};

#endif
