#include <stdio.h>
#include <iostream>
#include "Buffer.h"

Buffer::Buffer(){
	
}

Buffer::~Buffer(){
	
}

void Buffer::append(const char *buf,int _size){
	for(int i=0;i<_size;i++){
		if(buf[i]=='\0')break;
		readBuffer.push_back(buf[i]);
	}
}

const char* Buffer::c_str(){
	return readBuffer.c_str();
}

int Buffer::size(){
	return readBuffer.size();
}

void Buffer::clear(){
	readBuffer.clear();
}

void Buffer::getline(){
	readBuffer.clear();
	std::getline(std::cin,readBuffer);
}

void Buffer::setBuf(const char *_buf){
	readBuffer.clear();
	readBuffer.append(_buf);
}
