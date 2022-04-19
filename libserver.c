#include "server.h"

extern SockInfo client[MAX_CLINET];
/* ================================================ */

/* Show Start Information */

void show_banner(void) {

    system("clear");

    printf("*****************************************\n"
           "*                                       *\n"
           "*            Chat Room Server           *\n"
           "*                                       *\n"
           "*****************************************\n");

    printf("\n\nServer Socket Port : %d\n\n", PORT);
    printf("\nServer is running............\n\n\n");
    return;
}

/* ================================================ */

/* Show Client's IP */

void show_IP(SockInfo user, int option){

    char *user_IP;

    user_IP = inet_ntoa(user.sock_addr.sin_addr);

    switch(option){

        case 1 :
            printf("Receive the connection from : %s / name : %s\n", user_IP, user.name);
            break;
        case 2 :
            printf("Close the connection from : %s / name : %s\n", user_IP, user.name);
            break;
        default :
            break;
    }


    return ;
}

/* ================================================ */

/* Choose an empty ID */

int choose_user_num(SockInfo *client){

    int i;

    for(i = 0; (client[i].used != 0) && i < MAX_CLINET; i++){
        /* Do Nothing */
        //printf("client[i].used = %d\n\n", i, client[i].used);
    }

    client[i].used = 1;

    return i;
}

/* ================================================ */

/* Service thread for client (Receive) */

void *thread_client(void *ptr)
{
    char recv_buf[BUFSZ];
    char send_buf[BUFSZ + 24];
    SockInfo *user = (SockInfo *)ptr;
    int i;
    char str1[] = "Please Enter Your name : ";

    /* Initialization */

    bzero(recv_buf, sizeof(recv_buf));
    bzero(send_buf, sizeof(send_buf));

    /* request for user name */

    send(user->sockfd, str1, strlen(str1), 0);
    recv(user->sockfd, user->name, sizeof(user->name), 0);

    show_IP(*user, 1);

    while(1) {

		char *filename;

        if (recv(user->sockfd, recv_buf, sizeof(recv_buf), 0) > 0){

            sprintf(send_buf, "# %s > ", user->name);
            strcat(send_buf, recv_buf);

            for(i = 0; i < MAX_CLINET; i++){
                if((client[i].used != 0) && (client[i].sockfd != user->sockfd)){
                    send(client[i].sockfd, send_buf, strlen(send_buf), 0);
                }
            }

            bzero(send_buf, sizeof(send_buf));
            bzero(recv_buf, sizeof(recv_buf));
        }
    }

    user->used = 0;           // set to not used
    close(user->sockfd);
    return 1;
}

int trim(char *str)
{
	char *p = str;

	while (*p != '\0') {
		if (*p == '\n') {
			*p = '\0';
			return 1;
		}
		p++;
	}
}

int save_file(char *filename, int sockfd)
{
	// if (access(filename, F_OK) == -1) {
	// 	perror("access");
	// 	exit(-1);
	// }
	//
	
	FILE *fp;
	char buf[BUFSZ];

	fp = fopen(filename, "w");
	if (!fp) {
		perror("fopen");
		return -1;
	}

	/*
	 * recv file
	 */
	while (recv(sockfd, buf, sizeof buf, 0) != -1) {
		fwrite(buf, sizeof buf, 1, fp);
	}

	fclose(fp);
}

