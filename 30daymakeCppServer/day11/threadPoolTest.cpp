#include <iostream>
#include <string>
#include "ThreadPool.h"

void print(int a,double b,const char *c,std::string d){
	for(int i=0;i<1;i++)
		std::cout<< a<<b<<c<<d<<std::endl;
}

void test(){
	for(int i=0;i<1;i++)
		std::cout<<"test"<<std::endl;
}

int main(int argc,const char **argv){
	ThreadPool *pool=new ThreadPool();
	std::function<void()> func=std::bind(print,1,3.14,"hello",std::string("world"));

	pool->add(func);
	func=test;
	pool->add(func);

	delete pool;

	return 0;
}
