
#include "errif.h"


void errif(bool condition,const char *errmsg){
	if(condition==1){
		perror(errmsg);
		exit(EXIT_FAILURE);
	}
}

