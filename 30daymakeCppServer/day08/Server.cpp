#include "EventLoop.h"
#include "Server.h"
#include "errno.h"

int main(int argc,const char **argv){
	EventLoop *loop=new EventLoop();
	Server *server=new Server(loop);
	loop->loop();

	return 0;
}
