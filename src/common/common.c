#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#define MAXLINE 4096


void common_function(void){

    printf("This is common functionality !\n");
}

/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void
err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
	char	buf[MAXLINE];

	vsnprintf(buf, MAXLINE-1, fmt, ap);
	if (errnoflag)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s",
		  strerror(error));
	strcat(buf, "\n");
	fflush(stdout);		/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(NULL);		/* flushes all stdio output streams */
}

static void 
log_print_it(const char *fmt, va_list ap, const char* func_name)
{
    char buf[MAXLINE];
    char buf1[2*MAXLINE];
    vsnprintf(buf, MAXLINE-1, fmt, ap);
    snprintf(buf1, MAXLINE*2, "LOG_INFO:%s: %s\n", func_name, buf);
    fflush(stdout);
    fputs(buf1, stdout);
    fflush(NULL);
}

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void
err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
	exit(1);
}

void log_info (const char* func_name, const char *fmt, ...)
{
    va_list ap;
    va_start(ap,fmt);
    log_print_it(fmt,ap, func_name);
    va_end(ap);    
}

