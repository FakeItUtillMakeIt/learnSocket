套接字编程简介

    地址转换函数：
        地址文本表达和存放套接字地址结构的二进制值之间进行转换，如inet_addr,inet_ntoa,inet_pton和inet_ntop,inet_aton
        每个协议族都有自己的套接字地址结构，结构名称均以 sockaddr_开头，并以每个协议族的唯一后缀结尾，如sockaddr_in代表网际套接字地址结构

    通用套接字地址结构：    
        struct sockaddr

    进程到内核的套接字函数 bind,connect,sendto
    内核到进程的套接字函数 accpet,recvfrom,getsockname,getpeername

    字节序:
        变量地址开始地址存储低字节为小端
        变量地址开始地址存储高字节为大端
        网际协议使用大端字节序传输多字节数据
    主机字节序和网络字节序转换函数：
        返回网络字节序:htons，htonl  (host to net)
        返回主机字节序:ntohs，ntohl  (net to host)

    