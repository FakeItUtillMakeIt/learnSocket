
#pragma once

#include <string>

class Buffer{
	private:
		std::string readBuffer;

	public:
		Buffer();
		~Buffer();

		void append(const char *buf,int _size);
		const char* c_str();
		int size();
		void clear();
		void getline();
};
