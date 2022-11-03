#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <memory.h>

int main(){

    int sockfd,n;
	int counter=0;
    char recvline[1024];
    struct sockaddr_in sockaddr1;
	struct sockaddr_storage tmpsock;

    memset(&sockaddr1,0,sizeof(sockaddr));
	memset(&tmpsock,0,sizeof(tmpsock));
	
    
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
      printf("error in socket\n");
      return -1;
    }
    sockaddr1.sin_family=AF_INET;
    sockaddr1.sin_port=htons(10002);
   
  // if(inet_pton(AF_INET,"127.0.0.1",&sockaddr1.sin_addr)<=0){
//		  printf("error in pton\n");
//	   	return -1;
  // }
   sockaddr1.sin_addr.s_addr=inet_addr("127.0.0.1");
   
   if((connect(sockfd,(struct sockaddr*)&sockaddr1,sizeof(sockaddr1)))<0){
		  printf("error in connect\n");
      return -1;

   }

  while(counter==0 || (n=recv(sockfd,recvline,1024,0))>0){
	counter++;
	if(counter!=1)
		{printf("server input:%s\n",recvline);}
//	scanf("cli input:%s",&recvline);
	std::cout<<"client input:"<<counter;
	std::cin>>recvline;
	send(sockfd,recvline,sizeof(recvline),1);
   }

   if(n<0){
		  printf("error in recv\n");
      return -1;
   }
    
    return 0;
}
