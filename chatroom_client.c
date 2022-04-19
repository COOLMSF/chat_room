#include "client.h"

SockInfo server, user;

/* ================================================ */

int main(void)
{
    /* Initializaton */

    char buffer[SIZE];
    char serverIP[20];

    printf("Please Enter the server IP : ");
    scanf("%s", serverIP);
    system("clear");

    bzero(&user, sizeof(user));                    // Initialize user
    bzero(&server, sizeof(server));                // Initialize Server Info.
    server.sock_addr.sin_family = PF_INET;
    server.sock_addr.sin_port = htons(PORT);            // Socket Port
    inet_aton(serverIP, &server.sock_addr.sin_addr);    // Server IP

    /* socket() - Create an endpoint for communication. */

    if((user.sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
       printf("socket() is ERROR!!");
       exit(0);
    }

    /* connect() - Initiate a connection on a socket */

    if(connect(user.sockfd, (struct sockaddr*)&server.sock_addr, sizeof(server.sock_addr)) == -1){
        printf("Can not connect to Server (%s)... \n", serverIP);
        exit(0);
    } // connect to the server.
    else{
        show_banner(serverIP);       // show welcome message
    }

    /* recv() & send() - Receive and Send messages */

    send_recv();

    /* close() - Close the connection */

    close(user.sockfd);

    return 0;
}

