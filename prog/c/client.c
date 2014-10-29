#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
//Lisibility:
#define TCP          SOCK_STREAM
#define PROTOCOL     0
#define IPV4         AF_INET
#define BUFFER_SIZE  256
//Error code:
#define ERR_USAGE        -1
#define ERR_SOCKET       -2
#define ERR_CONNECTION   -3
#define ERR_WRITING      -4
#define ERR_READING      -5
#define ERR_HOST         -6
//Color code:
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_RESET   "\x1b[0m"

void error(const char *msg, const int err_num)
{
    fprintf(stderr, ANSI_RED);
    fprintf(stderr, msg);
    fprintf(stderr, ANSI_RESET);
    fprintf(stderr, "\n");
    exit(err_num);
}

int main(int argc, char *argv[])
{
    int sockfd = 0;
    int portno = 0;
    int read_bytes = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char incoming[BUFFER_SIZE];
    char outgoing[BUFFER_SIZE];

    //Check args
    if (argc < 3)
        error("usage: ./client hostname port", ERR_USAGE);

    portno = atoi(argv[2]);//Ascii TO Int
    sockfd = socket(IPV4, TCP, PROTOCOL);//creates an endpoint for communication and returns a descriptor.
    if (sockfd < 0)
        error("ERROR opening socket", ERR_SOCKET);

    server = gethostbyname(argv[1]);
    if (server == NULL)
        error("ERROR no such host", ERR_HOST);

    bzero((char *) &serv_addr, sizeof(serv_addr));//Clear the struct sockaddr_in serv_addr
    serv_addr.sin_family = IPV4;
    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting", ERR_CONNECTION);
    fprintf(stdout, "%sConnected%s\n", ANSI_GREEN, ANSI_RESET);

            bzero(incoming,BUFFER_SIZE);
        read_bytes = read(sockfd, incoming, BUFFER_SIZE-1);
        if (read_bytes < 0)
            error("ERROR reading from socket", ERR_READING);
    do{
        bzero(outgoing, BUFFER_SIZE);
        fgets(outgoing, BUFFER_SIZE-1, stdin);
        read_bytes = write(sockfd,outgoing,strlen(outgoing));
        if (read_bytes < 0) 
            error("ERROR writing to socket", ERR_WRITING);

        bzero(incoming,BUFFER_SIZE);
        read_bytes = read(sockfd, incoming, BUFFER_SIZE-1);
        if (read_bytes < 0)
            error("ERROR reading from socket", ERR_READING);

        printf("%s",incoming);
    }while(!(outgoing[0] == 'e' &&
             outgoing[1] == 'x' &&
             outgoing[2] == 'i' &&
             outgoing[3] == 't'));
    close(sockfd);
    return 0;
}
