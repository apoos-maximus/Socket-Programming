#include <stdio.h>
#include <sys/socket.h>
// #include <sys/types.h>
#include <netinet/in.h>
#include "common.h"


int main(){
    common_function();
    printf("client functions . . .!\n");

    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1){
        err_quit("socket() call failed with %d", sockfd);
    } else {
        LOG_INFO("socket() succeded with fd %d", sockfd);
    }
    return 0;
}