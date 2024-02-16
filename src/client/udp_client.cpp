#include "common.h"
#include "socktoolspec.h"
#ifdef WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unordered_set>


#ifdef WINDOWS
    #pragma comment(lib, "Ws2_32.lib")
#endif

static void print_usage() {
    printf("./client X.X.X.X[AF_INET]  Y.Y.Y.Y[AF_INET] \n");
    printf("X - client if addr\n");
    printf("Y - server addr\n");
}

struct sync_index {
    std::unordered_set<int> dtbl;
};


sync_index sindex;

pthread_t send_thread;
pthread_t receive_thread;


void handle_signal(int sig){
    pthread_cancel(send_thread);
    pthread_cancel(receive_thread);

    exit(0);
}

void* send_func(void *sockfdptr){
    
    int sockfd = *(int*)sockfdptr;
    //data transfer
    char *buf = NULL;
    int pkt_len = 0;
    int ids = 0;
    request_type tp = PING;
    int status = 0;
    char hostname[HOSTNAME_LEN];
    LOG_INFO("HOLA");
    
    status = gethostname(hostname, HOSTNAME_LEN);
    if (status < 0) {
        err_quit("gethostname() failed");
    }
    LOG_INFO("HOLA");
    LOG_INFO("sockfd %d", sockfd);
    while(true){
        pkt_len = prepare_pkt(&buf, tp, ids, hostname);
        status = send(sockfd, buf, pkt_len, 0);
        if(status < 0){
            err_quit("send() failed with %d", status);
        } else {
            free(buf);
            sindex.dtbl.insert(ids);

        }
        ids++;
        sleep_a(1);
    }
}

void* receive_func(void *sockfdptr){

    int sockfd = *(int*)sockfdptr;
    int recvbytes = 0;
    char recvbuf[MAX_UDP_PAYLOAD_LEN];
    struct sockaddr peeraddr;
    socklen_t peeraddrlen;
    peeraddrlen = sizeof(struct sockaddr);

    while(true){
        recvbytes = recvfrom(sockfd, recvbuf, MAX_UDP_PAYLOAD_LEN, 0, &peeraddr, &peeraddrlen);
        reply_pkt *pkt = (reply_pkt*)recvbuf;
        std::unordered_set<int>::iterator itr = sindex.dtbl.find(pkt->msgig);
        if (itr  != sindex.dtbl.end()){
            sindex.dtbl.erase(itr);
            parse_reply(&peeraddr, peeraddrlen, recvbuf, recvbytes);
        }
    }


}

int main(int argc, char* argv[]){
    // common_function();
    // LOG_INFO("client functions . . .!");

        

#ifdef WINDOWS
    WSADATA wsaData;
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
#endif

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
    struct sockaddr_in *paddr = (sockaddr_in*)&addr;
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
    struct sockaddr_in *pservaddr = (sockaddr_in*)&serveraddr;
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



    
    
    pthread_create(&send_thread, NULL, send_func, (void*) &sockfd);
    // pthread_create(&receive_thread, NULL, receive_func, (void*) &sockfd);

    signal(SIGINT, handle_signal);
    
    
#ifdef WINDOWS
    WSACleanup();
#endif
    
    return 0;
}