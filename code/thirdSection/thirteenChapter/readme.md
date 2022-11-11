第十三章、守护进程和inetd超级服务器

    概述：
        守护进程是在后台运行且不与任何控制终端关联的进程。常由系统初始化脚本启动，也可从shell由命令启动

        启动方法：
            1.系统启动时初始化脚本启动
            2.inetd超级服务器启动
            3.cron守护进程启动
            4.at指令指定将来某个时刻执行程序
            5.终端命令启动
        
    syslogd守护进程：
        1.读取配置文件
        2.创建一个Unix域数据报套接字，捆绑/var/run.log
        3.创建一个UDP套接字，捆绑端口514
        4.打开路径/dev/klog
        然后syslogd进入循环，调用select等待3个描述符之一变为可读，处理日志
    
    syslog函数：
        守护进程没有终端，不能使用打印，从守护进程中登记消息使用syslog函数
        日志相关头函数 syslog.h
    
    daemon_init函数：thirdSection/thirdteenChapter/daemon_init文件夹
        调用这个函数可以把普通进程变为守护进程

    inetd守护进程：
    
    daemon_inetd函数：
        
