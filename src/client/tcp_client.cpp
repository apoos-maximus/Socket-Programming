#include "common.h"
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>


#define MAXSLEEP 128
#define BUFLEN  128

int 
connect_retry(int domain, int type, int protocol,
              const struct sockaddr *addr, socklen_t alen)
{

    int numsec, fd;

    /*
     * Trying to connect with exponential backoff
     */
    
    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) {
        if ((fd = socket(domain, type, protocol)) < 0)
            return (-1);
        
        if (connect(fd, addr, alen) == 0) {
            /*
             * Connection accepted.
             */
            return (fd);
        }

        close(fd);

        /*
         * Delay before trying again
         */
        if (numsec <= MAXSLEEP/2)
            sleep_a(numsec);

    }

    return (-1);
}

void 
print_uptime(int sockfd)
{
    int     n;
    char    buf[BUFLEN];
    while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
        write(STDOUT_FILENO, buf, n);
    if (n < 0)
        err_quit("recv error");
}
// Connect_retry functoin 

int main() {

    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, err;

    if (argc != 2)
        err_quit("usage arguments - <hostname>");
    memset (&hint, 0, )
    return 0;
}