#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
log_print_it(const char *fmt, va_list ap, const char* file_name, const char* func_name, int line_num)
{
    char buf[MAXLINE];
    char buf1[2*MAXLINE];
    vsnprintf(buf, MAXLINE-1, fmt, ap);
    snprintf(buf1, MAXLINE*2, "LOG_INFO:  %s:%d:%s:\t%s\n", file_name, line_num, func_name, buf);
    fflush(stdout);
    fputs(buf1, stdout);
    fflush(NULL);
}

static void 
log_print_pkt(const char *fmt, va_list ap)
{
    char buf[MAXLINE];
    char buf1[2*MAXLINE];
    vsnprintf(buf, MAXLINE-1, fmt, ap);
    snprintf(buf1, MAXLINE*2, "LOG_PKT:  %s\n", buf);
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

void log_info (const char* file_name, const char* func_name, int line_num, const char *fmt, ...)
{
    va_list ap;
    va_start(ap,fmt);
    log_print_it(fmt,ap, file_name, func_name, line_num);
    va_end(ap);    
}

void log_pkt (const char *fmt, ...)
{
    va_list ap;
    va_start(ap,fmt);
    log_print_pkt(fmt, ap);
    va_end(ap); 
}

void print_family(int family_af){
    
    switch (family_af)
    {
    case AF_INET:
        LOG_INFO("Family: inet");
        break;
    case AF_INET6:
        LOG_INFO("Family: inet6");
        break;
    case AF_UNIX:
        LOG_INFO("Family: unix");
        break;
    case AF_UNSPEC:
        LOG_INFO("Family: unspecified");
        break;
    default:
        LOG_INFO("Family: unknown");
    }
}

char* type_to_str(request_type type) {
    switch(type){
        case HELLO : 
            return "HELLO";
            break;
        default :
            return "UNKNOWN";
            break;
    }
}


int prepare_pkt(char **buf, request_type type, uint32_t msgid) {
    request_pkt *pkt = (request_pkt*)calloc(1, sizeof(request_pkt));
    pkt->type = type;
    pkt->msgid = msgid;
    *buf = (char*)pkt;
    return sizeof(request_pkt);
}

request_type parse_request(struct sockaddr *peeraddr, socklen_t peeraddrlen, char *buf, int nbytes) {


    request_pkt *req = (request_type*)buf;
    char ipstr[20];
    struct sockaddr_in *paddr = peeraddr;
    inet_ntop(paddr->sin_family, &paddr->sin_addr, ipstr, peeraddrlen);

    LOG_PKT("%d %s:%d   %s  msgid:%d", nbytes,
                                        ipstr, ntohs(paddr->sin_port),
                                        type_to_str(req->type), req->msgid);
    
    return HELLO;
}