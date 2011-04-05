
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <execinfo.h>

__attribute__((__noreturn__)) void panic(char* file,int line,char *format,...)
{

	va_list args;
	va_start(args,format);

	fprintf(stderr, "%s:%i: ", file,line);
	vfprintf(stderr,format,args);

	if(errno!=0)
		fprintf(stderr,": %s",strerror(errno));

	fprintf(stderr,"\n");

	va_end(args);

    abort();
    
}



