#include <stdio.h>
#include "client.h"

extern SockInfo server, user;
/* ================================================ */

/* Show startting Message */

void show_banner(char serverIP[]){

    system("clear");

    printf("\t\t*****************************************\n"
           "\t\t*                                       *\n"
           "\t\t*            Chat Room Client           *\n"
           "\t\t*                                       *\n"
           "\t\t*****************************************\n");

    printf("\n\n============= Connectting to server successfully (%s) =============\n\n", serverIP);
    printf("\n=============      Welcome to Chat Room          =============\n\n");
    return;

    return ;
}

/* ================================================ */
/* Send() and Recv() */

void send_recv(void)
{
    pthread_t tid_init, tid_send, tid_recv;

    /* Create thread */
    pthread_create(&tid_init, NULL, thread_init, NULL);
    pthread_join(tid_init, NULL);

    pthread_create(&tid_recv, NULL, thread_recv, NULL);
    pthread_create(&tid_send, NULL, thread_send, NULL);

    pthread_join(tid_recv, NULL);
    pthread_join(tid_send, NULL);

    return ;
}

/* ================================================ */

/* Login - Enter ID  */

void thread_init(void *ptr)
{
    char str_newline[] = "\n";
    char buffer[SIZE];

    /* Enter the Name */

    bzero(buffer, sizeof(buffer));
    recv(user.sockfd, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    scanf("%s", user.Name);
    send(user.sockfd, user.Name, strlen(user.Name), 0);

    system("clear");
    printf("\n=============    Hello, %s. Welcome to Chatroom     =============\n\n\n\n > ", user.Name);

    return ;
}

/* ================================================ */

/* Send() used by pthread */

void thread_send(void *ptr)
{
    char send_buffer[SIZE + 24];
    char str_newline[] = "\n";

    bzero(send_buffer, sizeof(send_buffer));

    while(1) {

		/* input prompt */
		printf(">");

        fgets(send_buffer, sizeof send_buffer, stdin);
		fflush(stdin);

        strcat(send_buffer, str_newline);

		char *filename;

		// cmd should be like this, "cmd send filename someone"
		// We should split the send_buffer by space
		// the first element is cmd signature
		// the second is the command, 
		// the third element is the filename
		// the last element is the target

		if (strstr(send_buffer, "send ") != NULL) {

			/*
			 * multiple file support?
			 */
			filename = strtok(send_buffer, " ");
			filename = strtok(NULL, " ");
			/*
			 * filename contains '\n', remove it
			 */
			trim(filename);

			printf("Sending file:%s\n", filename);

			/*
			 * send signature
			 */
			char send_sig[BUFSZ] = "signature send";
			if (send(user.sockfd, send_sig, sizeof send_sig, 0) == -1) {
				perror("send signature");
				continue;
			}

			if (send_file(filename, user.sockfd)) {
				printf("Sending file OK\n");
				continue;
			}

			printf("Sending file ERROR\n");
			continue;
		}

        send(user.sockfd, send_buffer, strlen(send_buffer), 0);

        bzero(send_buffer, sizeof(send_buffer));
        fflush(stdin);
    }

    return ;
}

/* ================================================ */

/* Send() used by pthread */

void thread_recv(void *ptr)
{
    char recv_buf[SIZE];

    /* Initialization */

    bzero(recv_buf, sizeof(recv_buf));

    while(1){

        if(recv(user.sockfd, recv_buf, sizeof(recv_buf), 0) > 0){
            printf("%s", recv_buf);
        }

        bzero(recv_buf, sizeof(recv_buf));
        fflush(stdout);
    }

    return ;
}

int send_file(char *filename, int sockfd)
{
	FILE *fp;
	char buf[BUFSZ];

	fp = fopen(filename, "r");
	if (fp != NULL) {
		perror("fopen");
		return -1;
	}

	//Server 代码
	int nCount;
	while( (nCount = fread(buf, 1, BUFSZ, fp)) > 0 ){
	    send(sockfd, buf, nCount, 0);
	}
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

