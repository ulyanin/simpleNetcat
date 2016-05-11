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


#define MAX_BUFSIZE 65536

/*
 * error - wrapper for perror
 */
void print_error(char *msg) {
    perror(msg);
    exit(0);
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
    bzero(buf, MAX_BUFSIZE);
    for (i = 0; i < MAX_BUFSIZE; ++i) {
        c = fgetc(stdin);
        if (c == EOF)
            break;
        buf[i] = c;
    }
    /* send the message line to the server */
    n = write(sockfd, buf, strlen(buf));
    if (n < 0)
        print_error("ERROR writing to socket");

    /* print the server's reply */
    bzero(buf, MAX_BUFSIZE);
    n = read(sockfd, buf, MAX_BUFSIZE);
    if (n < 0)
        print_error("ERROR reading from socket");
    printf("server response:\n%s", buf);
    close(sockfd);
    return 0;
}
