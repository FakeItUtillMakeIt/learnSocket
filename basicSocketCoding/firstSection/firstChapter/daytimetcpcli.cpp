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
    char recvline[1024];
    struct sockaddr_in sockaddr1;

    memset(&sockaddr1,0,sizeof(sockaddr));
    
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
      printf("error in socket\n");
      return -1;
    }
    sockaddr1.sin_family=AF_INET;
    sockaddr1.sin_port=htons(10002);//linux主机字节序为小端，网络字节序规定为大端传输，需要进行转换

    if(inet_pton(AF_INET,"127.0.0.1",&sockaddr1.sin_addr)<=0){
        printf("error in pton\n");
        return -1;
    }

   //sockaddr1.sin_addr.s_addr=inet_addr("127.0.0.1");//已废弃
   
   if((connect(sockfd,(struct sockaddr*)&sockaddr1,sizeof(sockaddr1)))<0){
		  printf("error in connect\n");
      return -1;

   }

  while((n=recv(sockfd,recvline,1024,0))>0){
    printf("server input:%s\n",recvline);
  //	scanf("cli input:%s",&recvline);
    std::cout<<"client input:";
    std::cin>>recvline;
    send(sockfd,recvline,sizeof(recvline),0);
   }

   if(n<0){
		  printf("error in recv\n");
      return -1;
   }
    exit(0);
    return 0;
}
