#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"
#include "socktoolspec.h"

int main(){
    common_function();
    printf("Server Functions . . .  !\n");

    int sockfd;
    int status  = 0;
    sockfd = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1){
        err_quit("socket() failed with %d", sockfd);
    } else {
        LOG_INFO("socket() succeeded with %d", sockfd);
    }

    struct sockaddr addr;
    struct sockaddr_in *paddr = &addr;
    char ipstr[20];
    paddr->sin_port = htons(SOCK_TOOL_PORT);
    inet_pton(AF_INET, "172.16.242.129", &paddr->sin_addr);
    paddr->sin_family = AF_INET;
    socklen_t sz = sizeof(addr);
    status = bind(sockfd, &addr, sz);
    if(status == 0){
        LOG_INFO("bind() succeeded");
    } else {
        err_quit("bind() failed with %d", status);
    }
    
    // getsockname
    status = getsockname(sockfd, &addr, &sz);
    if(status == 0){
        LOG_INFO("getsockname() succeeded");
    } else {
        err_quit("getsockname() failed with %d", status);
    }

    LOG_INFO("Server Socket Details : fd : %d", sockfd);
    LOG_INFO("Port : %d", ntohs(paddr->sin_port));
    inet_ntop(paddr->sin_family, &paddr->sin_addr, ipstr, 20);
    print_family(paddr->sin_family);
    LOG_INFO("Address : %s", ipstr);

    // listen - NA for SOCK_DGARM
    // status = listen(sockfd, SOCK_TOOL_MAX_CONN);
    // if (status == -1){
    //     err_quit("listen() failed with %d", status);
    // } else {
    //     LOG_INFO("listen() succeeded");
    //     LOG_INFO("Server listening . . .");
    // }


    return 0;
}