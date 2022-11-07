#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

using namespace std;

#define INET_ADDR_LEN 16

int main(int argc,char** argv){
	char *ptr,**pptr;
	char str[INET_ADDR_LEN];
	struct hostent *hptr;
	struct in_addr inaddr;

	while(--argc>0){
		ptr=*++argv;
		if((hptr=gethostbyaddr(ptr,sizeof(inaddr),AF_INET))==NULL){
			cout<<"gethostbyaddr error for address:"<<ptr<<hstrerror(h_errno)<<endl;
			continue;
		}
		cout<<"official hostname:"<<hptr->h_name<<endl;

		for(pptr=hptr->h_aliases;*pptr!=NULL;pptr++){
			cout<<"alias:"<<*pptr<<endl;
		}
	}

	exit(0);
	return 0;
}
