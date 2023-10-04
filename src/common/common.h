#include "socktoolspec.h"

#ifdef WINDOWS
    #include <stdint.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <synchapi.h>
#else
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif




#define LOG_INFO(...) (log_info(__FILE__, __func__, __LINE__, __VA_ARGS__))
#define LOG_PKT(...) (log_pkt(__VA_ARGS__))

void common_function(void);

void err_quit (const char *fmt, ...);

void log_info (const char* file_name, const char* func_name, int line_num, const char *fmt, ...);

void log_pkt (const char *fmt, ...);

void print_family(int family_af);

void sleep_a(uint32_t seconds);

typedef enum request_type {
    PING,
    PING_REPLY
}request_type;

char* type_to_str(request_type);

#define HOSTNAME_LEN 100
#define REPLY_MSG_LEN 100
typedef struct request_pkt{
    request_type type;
    uint32_t msgid;
    char hostname[HOSTNAME_LEN];
} request_pkt;

typedef struct reply_pkt{
    request_type type;
    uint32_t msgig;
    char message[REPLY_MSG_LEN];
} reply_pkt;

int prepare_pkt(char **buf, request_type type, uint32_t msgid, char *hostname);

int prepare_reply(char *request, int req_len, char **reply);

void parse_request(struct sockaddr *peeraddr, socklen_t peeraddrlen, char *buf, int nbytes);

void parse_reply(struct sockaddr *peeraddr, socklen_t peeraddrlen, char *buf, int nbytes);

void log_sockaddr(struct sockaddr *addr, socklen_t addrlen);