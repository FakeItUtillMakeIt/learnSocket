#include <stdio.h>
#include <unistd.h>

#define CPU_VENDOR_OS "i686-pc-liunx-gnu"

int main(){
	union{
		short s;
		char c[sizeof(short)];
	}un;

	un.s=0x0102;
	printf("%s: ",CPU_VENDOR_OS);
	
	if(sizeof(short)==2){
		if(un.c[0]==1 && un.c[1]==2){
			printf("big-endian\n");
		}
		else{
			printf("little-endian\n");
		}
	}

	return 0;
}
