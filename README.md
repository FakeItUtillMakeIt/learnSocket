套接字网络编程

    套接字API

        socket:套接字，创建一个网际(AF_INET)字节流(SOCK_STREAM)，并返回一个整数描述符
        sockaddr_in:网际套接字地址结构(IP和port)
        htons:主机网络到短整数
        inet_pton:呈现形式到数值
        connect:使用套接字建立服务器连接
        read:读取
        bind:绑定
        listen:监听
        accept:获取连接fd
        close:关闭

        服务器:socket bind listen accept
        客户端:socket connect

    IPV4数据报能传输的最大字节数为65535，最小传输单元为68字节
    以太网的MTU(最大传输单元)为1500字节，如果传输数据报的大小超过最大MTU，就数据报分片传输
    TCP必须保存一个发送缓冲区的副本，直至对端确认接收后才清除
    每个数据链路都有一个输出队列，若队列满，则会从数据链路-->IP-->TCP返回一个错误，TCP处理该错误，并在队列不满时进行重传

    UDP:简单、无连接、不可靠的协议
    TCP:复杂、可靠、面向连接的协议  TIME_WAIT状态：实现TCP的全双工连接终止(处理最终ACK流失的情况)，并允许老的重复分节数据从网络中消逝。


    并发服务器有多种编写方式:
        1.调用fork函数为每个客户创建一个子进程，多进程
        2.使用线程代替fork，多线程
    网络并发服务器套接字/描述符状态挂载到等待队列：select/poll，epoll
