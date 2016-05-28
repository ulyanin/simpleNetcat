/*
 * nc.c - A simple TCP client
 * usage: nc <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>


#define MAX_BUFSIZE 65536

/*
 * error - wrapper for perror
 */
void print_error(char *msg) {
    perror(msg);
    exit(0);
}

void* listen_(void* arg){
    int sock = *(int*)arg;
    int n = 0;
    char buf[MAX_BUFSIZE];
    bzero(buf, MAX_BUFSIZE);
    while(1){
        //printf("_\n");
        n = read(sock, buf, MAX_BUFSIZE);
        //printf("%d\n", n);
        if(n == 0){
            printf("server sended empty msg, terminating\n");
            exit(0);
        }
        if (n < 0)
            print_error("ERROR reading from socket");
        buf[n] = 0;
        printf("server response:\n%s", buf);
    }
    pthread_exit(0);
}

int main(int argc, char **argv) {
    int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[MAX_BUFSIZE];

    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        print_error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }


    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
            (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    /* connect: create a connection with the server */
    if (connect(sockfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        print_error("ERROR connecting");
    /* get message line from the user */
    int i, c;
    

    pthread_t thread;
    pthread_create(&thread, NULL, listen_, &sockfd);
    
    while(1){
        bzero(buf, MAX_BUFSIZE);
        for (i = 0; i < MAX_BUFSIZE - 1; ++i) {
            c = fgetc(stdin); 
            buf[i] = c;
            if (c == '\n')
                break;
        }
        /* send the message line to the server */
        n = write(sockfd, buf, strlen(buf));
        if (n < 0)
            print_error("ERROR writing to socket");
    }

    close(sockfd);

    return 0;
}
