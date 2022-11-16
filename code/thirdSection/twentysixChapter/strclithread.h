
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>

#define MAXLINE 1024


static int sockfd;
static FILE *fp;
void *copyto(void*);
void str_cli(FILE* fp_arg,int sockfd_arg);
