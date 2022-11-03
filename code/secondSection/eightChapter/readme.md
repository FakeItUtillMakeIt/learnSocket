第八章、基本UDP套接字编程
    UDP是无连接不可靠的数据报协议，客户只管发（对端IP），服务器只管收

    recvfrom和sendto函数：
        类似于recv和send函数，需要额外带flags，sockaddr*地址和地址长度
        返回值为接受或发送数据报的数据长度

    UDP回射服务器程序：udpserv01.cpp
    
    UDP客户端程序： udpcli01.cpp

    UDP的connect函数：
        UDP套接字也可以调用connect函数，已连接的UDP套接字，使用sendto函数时第5个参数需为NULL，可以使用send和write函数，不使用recvfrom，而使用recv，read或recvmsg
    
    在一个未连接的UDP套接字上给两个数据报调用sendto函数时涉及的步骤：
        1.连接套接字
        2.输出第一个数据报
        3.断开套接字连接
        4.连接套接字
        5.输出第二个数据报
        6.断开套接字连接

    UDP套接字接收缓冲区：
        由UDP给某个特定套接字排队的UDP数据报数目受限于该套接字接收缓冲区的大小，可以使用SO_RECVBUF套接字选项修改

    使用select函数的TCP和UDP回射服务器程序：tcpudpservselect01.cpp 

        
