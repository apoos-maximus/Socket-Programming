#include <stdio.h>
#include <sys/socket.h>
// #include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"
#include "socktoolspec.h"



static void print_usage() {
    printf("./client X.X.X.X[AF_INET]  Y.Y.Y.Y[AF_INET] \n");
    printf("X - client if addr\n");
    printf("Y - server addr");
}

int main(int argc, char* argv[]){
    // common_function();
    // LOG_INFO("client functions . . .!");

    if (argc < 3) {
        print_usage();
        err_quit("supply all arguments");
    }

    int status = 0;
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1){
        err_quit("socket() call failed with %d", sockfd);
    } else {
        LOG_INFO("socket() succeded with fd %d", sockfd);
    }

    struct sockaddr addr;
    struct sockaddr_in *paddr = &addr;
    char ipstr[20];
    // paddr->sin_port = htons(6001);
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
    LOG_INFO("Port : %d", ntohs(paddr->sin_port));
    inet_ntop(paddr->sin_family, &paddr->sin_addr, ipstr, 20);
    print_family(paddr->sin_family);
    LOG_INFO("Address : %s", ipstr);
    
    //connect
    struct sockaddr serveraddr;
    struct sockaddr_in *pservaddr = &serveraddr;
    pservaddr->sin_family = AF_INET;
    pservaddr->sin_port = htons(SOCK_TOOL_PORT);
    inet_pton(AF_INET, argv[2], &pservaddr->sin_addr);
    status = connect(sockfd, &serveraddr, sizeof(serveraddr));
    if (status == 0){
        LOG_INFO("connect() succeeded");
    } else {
        err_quit("connect() failed with %d", status);
    }
    
    // getpeername
    status = getpeername(sockfd, &addr, &sz);
    if(status == 0){
        LOG_INFO("getpeername() succeeded");
    } else {
        err_quit("getpeername() failed with %d", status); 
    }
    LOG_INFO("Peer Name :");
    LOG_INFO("Port : %d", ntohs(paddr->sin_port));
    inet_ntop(paddr->sin_family, &paddr->sin_addr, ipstr, 20);
    print_family(paddr->sin_family);
    LOG_INFO("Address : %s", ipstr);

    //data transfer
    char *buf = NULL;
    int pkt_len = 0;
    int i = 0;
    int ids = 0;
    request_type tp = PING;
    char hostname[HOSTNAME_LEN];
    status = gethostname(hostname, HOSTNAME_LEN);

    int recvbytes = 0;
    char recvbuf[MAX_UDP_PAYLOAD_LEN];
    struct sockaddr peeraddr;
    socklen_t peeraddrlen;

    if (status < 0) {
        err_quit("gethostname() failed");
    }
    for (;;){
        pkt_len = prepare_pkt(&buf, tp, ids, hostname);
        status = send(sockfd, buf, pkt_len, 0);
        if(status < 0){
            err_quit("send() failed with %d", status);
        } else {
            free(buf);
            peeraddrlen = sizeof(struct sockaddr);
            recvbytes = recvfrom(sockfd, recvbuf, MAX_UDP_PAYLOAD_LEN, 0, &peeraddr, &peeraddrlen);
            parse_reply(&peeraddr, peeraddrlen, recvbuf, recvbytes);
        }
        ids++;
        sleep(1);
    }    
    
    
    return 0;
}