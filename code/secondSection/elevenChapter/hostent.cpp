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

	while(--argc>0){
		ptr=*++argv;
		if((hptr=gethostbyname(ptr))==NULL){
			cout<<"gethostbyname error for host:"<<ptr<<hstrerror(h_errno)<<endl;
			continue;
		}
		cout<<"official hostname:"<<hptr->h_name<<endl;

		for(pptr=hptr->h_aliases;*pptr!=NULL;pptr++){
			cout<<"alias:"<<*pptr<<endl;
		}

		switch(hptr->h_addrtype){
			case AF_INET:
				pptr=hptr->h_addr_list;
				for(;*pptr!=NULL;pptr++){
					cout<<"address:"<<inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str))<<endl;;
				}
				break;
			default:
				cout<<"unknown address type";
				break;
		}
	}

	exit(0);
	return 0;
}
