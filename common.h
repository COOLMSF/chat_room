/*
 * This file include all common structures and macro
 */
struct sock_info_t {
    int used;
    char Name[20];
    int sockfd;
    struct sockaddr_in sock_addr;
};

typedef struct sock_info_t SockInfo;