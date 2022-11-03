第六章、I/O复用：select和poll函数
    TCP客户同时处理两种输入：标准输入和TCP套接字时会阻塞在标准输入上，服务器进程被杀死时发送FIN，客户端阻塞与标准输入读入过程，看不到这个EOF，直到套接字读时为止（可能已经过去很长时间了），这样的进程需要一种预先告知内核的能力，使得内核一旦发现进程指定的一个或多个ID条件就绪，它就通知进程，这个能力叫做IO复用，是由select和poll两个函数支持的。

    IO复用常使用的场景：
        1.客户处理多个描述符时
        2.一个客户处理多个套接字时
        3.TCP服务器处理多个套接字时
        4.一个服务器既要处理TCP，又要处理UDP时
        5.一个服务器要处理多个服务或多个协议时

    5种I/O模型：
        1.阻塞式IO
        2.非阻塞式IO
        3.IO复用
        4.信号驱动式IO（SIGIO）
        5.异步IO（aio_系列函数）
    非阻塞式IO：
        进程把一个套接字设置为非阻塞是通知内核，当所请求的IO操作必须得把当前进程投入睡眠才能完成时，不要把当前进程投入睡眠，而是返回一个错误。
    IO复用模型：
        调用select和poll，阻塞在这两个系统调用中的某一个， 进程受阻于select调用，等待可能多个套接字中的任何一个变为可读
    信号驱动式IO模型：
        内核在描述符就绪时发送SIGIO信号通知进程
    异步IO模型：
        告知内核启动某个操作，并让内核在整个操作完成后通知进程，和信号驱动IO不同的是，信号驱动告知何时启动IO操作，异步IO告知IO操作何时完成
    select函数：  sixChapter/tcpserv12.cpp sixChapter/tcpcli12.cpp
        select允许进程指示内核等待多个事件中的任何一个发生，并只在有一个或多个事件发生或经历一段指定时间后才唤醒它，
        任何描述符都可以使用select
        原型：int select(int maxfdp1,fd_set *readset,fd_set* writeset,fd_set* exceptset,const struct timeval* timeout);
    描述符就绪条件：
        套接字准备好读：
            1.套接字接收缓冲区的数据字节数大于等于套接字接收缓冲区 RCV_LOWAT 的大小
            2.套接字连接的读半关闭(接收了FIN的TCP连接)
            3.监听套接字的已连接数不为0
            4.描述符上有一个套接字错误待处理
        套接字准备好写：
            1.套接字发送缓冲区的数据字节数大于等于套接字发送缓冲区 SND_LOWAT 的大小
            2.套接字连接的写半关闭，产生SIGPIPE
            3.使用非阻塞式connect的套接字已建立连接，或connect以失败告终
            4.描述符上有一个套接字错误待处理
        如果一个套接字存在带外数据或仍处于带外标记，那就有异常条件待处理
    shutdown函数：sixChapter/tcpserv13.cpp sixChapter/tcpcli13.cpp
        int shutdown(int sockfd,int howto);
        howto参数:SHUT_RD关闭连接读,SHUT_WR关闭连接写,SHUW_RDWR关闭连接读写

     tcp回射服务器程序（修订版）: sixChapter/tcpservselect01.cpp sixChapter/tcpcliseclet01.cpp

    拒绝服务型攻击：

    pselect函数：

    poll函数：
        poll可以工作在任何描述符上，
        原型：int poll(struct pollfd* fdarray,unsigned long nfds,int timeout);
        struct pollfd{
            int fd;//检查的描述符
            short events;//对描述符感兴趣的事件（输入）
            short revents;//对描述符上已发生的事件（返回）
        }

     tcp回射服务器程序（再修订版）: sixChapter/tcpservpoll01.cpp sixChapter/tcpclipoll01.cpp

