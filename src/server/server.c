#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "common.h"
#include "socktoolspec.h"


static void print_usage() {
    printf("./server X.X.X.X [AF_INET]\n");
}

int main(int argc, char* argv[]){
    // common_function();
    // printf("Server Functions . . .  !\n");

    if (argc < 2) {
        print_usage();
        err_quit("supply server address");
    }

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
    inet_pton(AF_INET, argv[1], &paddr->sin_addr);
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

    ssize_t nbytes = 0;
    char buf[MAX_UDP_PAYLOAD_LEN];
    struct sockaddr peeraddr;
    socklen_t peeraddrlen = 0;

    ssize_t send_bytes = 0;
    char *send_buf = NULL;
    int send_pkt_len = 0;

    for (;;){
        
        peeraddrlen = sizeof(struct sockaddr);
        nbytes = recvfrom(sockfd, buf, MAX_UDP_PAYLOAD_LEN, 0, &peeraddr, &peeraddrlen);
        if (nbytes < 0) {
            LOG_INFO("Error during recieve");
        } else if (nbytes == 0) {
            LOG_INFO("No messages available");
        } else {
            // LOG_INFO("Recieved %d bytes", nbytes);

            parse_request(&peeraddr, peeraddrlen, buf, nbytes);
            send_pkt_len = prepare_reply(buf, nbytes, &send_buf);
            send_bytes = sendto(sockfd, send_buf, send_pkt_len, 0,
                                &peeraddr, peeraddrlen);
            if(send_bytes < 0){
                err_quit("send() failed with %d", send_bytes);
            }
            free(send_buf);
        }
    }
    
    return 0;
}