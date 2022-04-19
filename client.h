#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


#define BUFSZ 1024
#define PORT 7777
#define SIZE 128

struct sock_info_t {
    int used;
    char Name[20];
    int sockfd;
    struct sockaddr_in sock_addr;
};
typedef struct sock_info_t SockInfo;

void show_banner(char serverIP[]);
void send_recv(void);
void thread_init(void *ptr);
void thread_send(void *ptr);
void thread_recv(void *ptr);
int send_file(char *filename, int sockfd);
int trim(char *str);
