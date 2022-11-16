
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
#include <pthread.h>
#include <thread>

#include "tcp_connect.h"

#define MAXFILES 20
#define SERV "80"
#define MAXLINES 1024

#define F_CONNECTING 1
#define F_READING 2
#define F_DONE 4

#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

struct file{
	char *f_name;
	char *f_host;
	int f_fd;
	int f_flags;
	pthread_t f_tid;
}file[MAXFILES];

int nconn,nfiles,nlefttoconn,nlefttoread;

void *do_get_read(void *);
void home_page(const char *,const char *);
void write_get_cmd(struct file *);

int main(int argc,char **argv){

	int i,n,maxnconn;
	pthread_t yid;
	struct file *fptr;

	if(argc<5){
		std::cout<<"usage:webthread01 <#maxconns> <ip> <homapage> file1 ..."<<std::endl;
		return -1;
	}

	maxnconn=atoi(argv[1]);
	
	nfiles=std::min(argc-4,MAXFILES);
	for(i=0;i<nfiles;i++){
		file[i].f_name=argv[i+4];
		file[i].f_host=argv[2];
		file[i].f_flags=0;
	}

	std::cout<<"nfiles="<<nfiles<<std::endl;

	home_page(argv[2],argv[3]);
	nlefttoread=nlefttoconn=nfiles;
	nconn=0;

	while(nlefttoconn>0){
		while(nconn<maxnconn && nlefttoconn>0){
			//find a file to read
			for(i=0;i<nfiles;i++){
				if(file[i].f_flags==0){
					break;
				}
			}
			if(i==nfiles){
				std::cout<<"nleftconn="<<nlefttoconn<<",but nothing found"<<std::endl;
				return -1;
			}

			file[i].f_flags=F_CONNECTING;
			pthread_create(&yid,NULL,&do_get_read,&file[i]);
			file[i].f_tid=yid;
			nconn++;
			nlefttoconn--;
		}
		if((n=thr_join(0,&yid,(void **)&fptr))!=0){//wait anyone thread terminate
			errno=n,std::cout<<"thr_join error"<<std::endl;
		}

		nconn--;
		nlefttoread--;
		std::cout<<"thread id :"<<yid<<"for "<<fptr->f_name<<"done"<<std::endl;

	}

	return 0;
}

void *do_get_read(void *vptr){
	int fd,n;
	char line[MAXLINES];
	struct file *fptr;

	fptr=(struct file *)vptr;

	fd=tcp_connect(fptr->f_host,SERV);
	fptr->f_fd=fd;
	std::cout<<"do_get_read for "<<fptr->f_name<<"fd:"<<fptr->f_fd<<std::endl;

	write_get_cmd(fptr);

	//read server's reply
	while(true){
		if((n=read(fd,line,MAXLINES))==0)
			break;//server clost conn
		std::cout<<"read "<<n<<"bytes from"<<fptr->f_name<<std::endl;
	}

	std::cout<<"end of file on "<<fptr->f_name<<std::endl;

	close(fd);
	fptr->f_flags=F_DONE;
	return fptr;//terminate thread
}

//write request to server
//http GET command
void write_get_cmd(struct file *fptr){
	int n;
	char line[MAXLINES];

	n=snprintf(line,sizeof(line),GET_CMD,fptr->f_name);
	write(fptr->f_fd,line,n);
	std::cout<<"write "<<n<<"bytes for "<<fptr->f_name<<std::endl;

	fptr->f_flags=F_READING;
}


