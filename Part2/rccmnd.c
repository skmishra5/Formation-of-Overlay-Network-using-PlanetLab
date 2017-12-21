#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <time.h>

#define MAXLEN 4096

int main(int argc, char **argv)
{
    int sd; /* to create socket */

    struct sockaddr_in serverAddress;/* client will connect on this */

    int nbytes, port;
    char buf1[MAXLEN];
    char buf2[MAXLEN];
    struct hostent *server;
    char *hostname;
    unsigned int t;
    struct timeval start, end;
    int count, i;

    if ((argc != 11) || (strcmp(argv[1], "-h") == 0))
    {
        printf("Usage: <rccmnd_tcp> -s <server IP> -p <port> -c <command within double quotes> -n <execution time> -d <delay>\n");
	printf("To get the help: <rccmnd_udp> -h \n");
        exit(1);
    }
    
    if(strcmp(argv[1], "-s") == 0)
    {
        hostname = argv[2];
    }    

    if(strcmp(argv[3], "-p") == 0)
    {
        port = atoi(argv[4]);
    }

    /* create socket */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0)
    {
        printf("socket creation failed \n");
    }

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* initialize the socket addresses */
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family=AF_INET;
    //serverAddress.sin_addr.s_addr=inet_addr(argv[1]);
    bcopy((char *)server->h_addr, 
	  (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port=htons(port);

    /* client  connect to server on port */
    connect(sd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));


    if(strcmp(argv[5], "-c") == 0)
    {
        strcpy(buf1, argv[6]);
    	strcat(buf1, ":");
    }
    if(strcmp(argv[7], "-n") == 0)
    {
        strcat(buf1, argv[8]);
        strcat(buf1, ":");
    }
    if(strcmp(argv[9], "-d") == 0)
    {
        strcat(buf1, argv[10]);
    }
    
    count = atoi(argv[8]);
    /* send to sever and receive from server */

    nbytes = strlen(buf1) + 1;
    gettimeofday(&start, NULL);    
    send(sd, buf1, nbytes, 0);
    for(i = 0; i < count; i++)
    {
        nbytes = recv(sd, buf2, MAXLEN, 0);
        printf("\nReceive message from  server::\n\n %s \n\n", buf2);
	system("date");
        gettimeofday(&end, NULL);
        t = end.tv_usec - start.tv_usec;  
        printf("The RTT is: %d\n", t);
    }

    //printf("Receive message from  server::\n\n %s \n\n", buf2);
   // }

    return 0;
}






