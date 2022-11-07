第十一章、名字与地址转换

    概述：
        本章讲述名字与数值地址间进行转换的函数，gethostbyname和gethostbyaddr(在主机名与地址间进行转换)，getservbyname和getservbyaddr(在服务名字和端口号间进行转换),getaddrinfo和getnameinfo(用于主机名和IP地址及服务名和端口间转换)

    域名系统：
        用于主机名和IP地址进行映射
        解析器与名字服务器
    
        gethostbyname例子:secondSection/elevenChapter/hostent.cpp
        gethostbyaddr例子：secondSection/elevenChapter/hostent1.cpp
        getservbyname:secondSection/elevenChapter/hostent2.cpp
        getservbyport:
    
    getaddrinfo函数：
        该函数可以处理名字到地址及服务到端口两种转换，返回值是sockaddr结构
    gai_strerror函数：
        输出由getaddrinfo返回值的含义
    freeaddrinfo函数：

    tcp_connect例子：secondSection/elevenChapter/daytimetcpcli.cpp

    tcp_listen例子：secondSection/elevenChapter/daytimetcpserv.cpp

    可指定协议的时间获取服务器程序：secondSection/elevenChapter/daytimetcpserv2.cpp
    