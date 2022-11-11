
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string.h>

using namespace std;

int tcp_listen(const char* hostname,const char* service,socklen_t* addrlenp);
