
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>
#include <error.h>
#include "error.h"

#define MAXFD 64


int daemon_init(const char *pname,int facility);
