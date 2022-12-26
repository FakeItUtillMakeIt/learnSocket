
#include "InetAddress.h"

InetAddress::InetAddress():
	addr_len(sizeof(sock_addr))
{
	bzero(&this->sock_addr,sizeof(this->sock_addr));
}

InetAddress::InetAddress(const char *ip,int port){
	bzero(&this->sock_addr,sizeof(this->sock_addr));
	this->sock_addr.sin_family=AF_INET;
	this->sock_addr.sin_addr.s_addr=inet_addr(ip);
	this->sock_addr.sin_port=htons(port);

	this->addr_len=sizeof(this->sock_addr);
}

InetAddress::~InetAddress(){

}

