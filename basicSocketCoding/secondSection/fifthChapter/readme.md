第五章、TCP客户、服务器程序示例

    Posix信号处理：
        信号是告知某个进程发生了某个事件的通知，也称为软件中断，通常是异步的
        信号可以由一个进程发给另一个进程/自身，也可以由内核发给某个进程
    网络编程时可能遇到的三种情况：
        1.当fork子进程时，必须捕获SIGCHLD信号
        2.当捕获信号时，必须处理被中断的系统调用
        3，SIGCHLD的信号处理函数必须正确编写，应使用waitpid函数以避免留下僵死进程
    accept返回连接前终止：
        再次调用accept
    服务器进程终止：
        当FIN到达套接字时，客户正阻塞在fgets调用上，程序不能阻塞在这两个特定源的输入上，而应该阻塞在其中任何一个上，这就是select和poll函数的目的之一
    SIGPIPE信号：   
        用户不理会read函数返回的错误，反而写入更多数据到服务器上，就有可能引发这种情况
    服务器主机崩溃：
        客户端主动发数据，服务器不可达（网络问题）时，客户端需要设置超时时间避免长时间等待
        客户端不主动发数据，服务器不可达时（网络问题），需要讨论SO_KEEPALIVE套接字选项
    服务器主机崩溃后重启：
    
    服务器主机关机：
        Unix系统关机时，init进程先发送SIGTERM信号，等待一段固定时间，给仍在运行的进程发送SIGKILL信号

    TCP程序例子小结：

    数据格式：
        