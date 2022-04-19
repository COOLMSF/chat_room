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
#include <signal.h>

#define PORT 7777
#define BUFSZ 128
#define MAX_CLINET 10

struct sock_info_t {
    int used;
    char name[20];
    int sockfd;
    struct sockaddr_in sock_addr;
};

typedef struct sock_info_t SockInfo;

int trim(char *s);
int save_file(char *s, int sockfd);

void show_banner(void);
void show_IP(SockInfo user, int option);
void *thread_client(void *ptr);
int choose_user_num(SockInfo *client);
