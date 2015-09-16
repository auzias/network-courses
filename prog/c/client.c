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
#define TCP             SOCK_STREAM
#define PROTOCOL        0
#define IPV4            AF_INET
#define BUFFER_SIZE     256
#define HOSTNAME_SIZE   32
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
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_RESET   "\x1b[0m"

void error(const char *msg, const int err_num)
{
    fprintf(stderr, ANSI_RED);
    fprintf(stderr, msg);
    fprintf(stderr, ANSI_RESET);
    fprintf(stderr, "\n");
    exit(err_num);
}

void message(const char *msg)
{
    fprintf(stdout, ANSI_GREEN);
    fprintf(stdout, msg);
    fprintf(stdout, ANSI_RESET);
    fprintf(stdout, "\n");
}

void received(const char *msg)
{
    fprintf(stdout, ANSI_BLUE);
    fprintf(stdout, "received:");
    fprintf(stdout, msg);
    fprintf(stdout, ANSI_RESET);
}

int main(int argc, char *argv[])
{
    int socket_fd = 0;
    int port = 0;
    int argv_port_index = 0;
    int argv_host_index = 0;
    int read_bytes = 0;
    int write_bytes = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char *server_name;
    char incoming[BUFFER_SIZE];
    char outgoing[BUFFER_SIZE];

    //Check args
    if (argc < 2)
        error("usage: ./client [hostname] port", ERR_USAGE);

    if (argc == 3) {
        argv_host_index = 1;
        argv_port_index = 2;
        server_name = argv[argv_host_index];
        server = gethostbyname (server_name);
        if (server == NULL)
            error ("ERROR no such host", ERR_HOST);
    }
    if (argc == 2) {
        message ("\"localhost\" is assumed as host.");
        server = gethostbyname ("localhost");
        argv_port_index = 1;
    }

    port = atoi (argv[argv_port_index]);//Ascii TO Int
    socket_fd = socket (IPV4, TCP, PROTOCOL);//creates an endpoint for communication and returns a file descriptor.
    if (socket_fd < 0)
        error ("ERROR opening socket", ERR_SOCKET);
    message ("Socket opened.");

    bzero ((char *) &serv_addr, sizeof(serv_addr));//Clear the struct sockaddr_in serv_addr
    serv_addr.sin_family = IPV4;
    bcopy ((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(socket_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error ("ERROR connecting", ERR_CONNECTION);
    fprintf (stdout, "%sConnected with %s:%d %s\n", ANSI_GREEN, server->h_name, port, ANSI_RESET);

    //Start the loop of receiving/sending:
    do {
        bzero (incoming,BUFFER_SIZE);
        message ("Wait for a message");
        read_bytes = read (socket_fd, incoming, BUFFER_SIZE-1);
        if (read_bytes < 0)
            error ("ERROR reading from socket", ERR_READING);
        received (incoming);

        bzero (outgoing, BUFFER_SIZE);
        message ("Write a message");
        fgets (outgoing, BUFFER_SIZE-1, stdin);
        write_bytes = write(socket_fd,outgoing,strlen(outgoing));
        if (write_bytes < 0)
            error ("ERROR writing to socket", ERR_WRITING);
    } while(!(outgoing[0] == 'e' &&
             outgoing[1] == 'x' &&
             outgoing[2] == 'i' &&
             outgoing[3] == 't'));
    shutdown (socket_fd, SHUT_RDWR);
    close (socket_fd);
    return 0;
}
