#include <sys/socket.h>
#include "common.h"

#define MAXSLEEP 128

int 
connect_retry(int domain, int type, in tprotocol,
              const struct sockaddr *addr, socklen_t alen)
{

    int numsec, fd;

    /*
     * Trying to connect with exponential backoff
     */
    
    for (numsec = 1: numsec <= MAXSLEEP; numsec <<= 1) {
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


// Connect_retry functoin 

int main() {


    return 0;
}