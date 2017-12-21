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

#define MAXLEN 4096

int main(int argc, char **argv)
{
    struct sockaddr_in servAddr, cliAddr;
    int sd, slen=sizeof(cliAddr); 
    char buf[MAXLEN], buf1[MAXLEN];
    int recvlen, port;
    struct hostent *server;
    struct timeval start, end;
    char *hostname;
    unsigned int t;
    int count, i;

    if (argc != 11) 
    {
        printf("Usage: <rccmnd_udp> -s <server IP> -p <port> -c <command within double quotes> -n <num_exec> -d <delay>\n");
	printf("To get the help: <rccmnd_udp> -h \n");
        exit(1);
    }

    if(strcmp(argv[1], "-s") == 0)
    {
        hostname = argv[2];
    }
    else if(strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: <rccmnd_udp> -s <server IP> -p <port> -c <command within double quotes> -n <num_exec> -d <delay>\n");
	return 0;
    }

    if(strcmp(argv[3], "-p") == 0)
    {
        port = atoi(argv[4]);
    }

    if ((sd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    {
        printf("socket creation failed\n");
        return 0;
    }

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    memset((char *) &cliAddr, 0, sizeof(cliAddr));
    cliAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&cliAddr.sin_addr.s_addr, server->h_length);
    cliAddr.sin_port = htons(port);
    //if (inet_aton(argv[1], &cliAddr.sin_addr)==0) 
    //{
    //   fprintf(stderr, "inet_aton() failed\n");
    //   exit(1);
    //}

     /* sending the messages */
    
    memset(buf, '\0', sizeof(buf));
    memset(buf1, '\0', sizeof(buf1));
    
    if(strcmp(argv[5], "-c") == 0)
    {
    	strcpy(buf, argv[6]);
    	strcat(buf, ":");
    }
    if(strcmp(argv[7], "-n") == 0)
    {
        strcat(buf, argv[8]); 
        strcat(buf, ":");
    }
    if(strcmp(argv[9], "-d") == 0)
    {
        strcat(buf, argv[10]);
    } 

    gettimeofday(&start, NULL);
    if(sendto(sd, buf, strlen(buf), 0, (struct sockaddr *)&cliAddr, slen)==-1)
    {
        printf("sendto failed \n");
    }
    
    count = atoi(argv[8]);
    
    for(i =0; i < count; i++)
    {
        recvlen = recvfrom(sd, buf1, MAXLEN, 0, (struct sockaddr *)&cliAddr, &slen);
        gettimeofday(&end, NULL);
        if (recvlen < 0)
        {
	    printf("Error in receiving from server \n");
        }
        else
        {
            printf("The output of the command is and bytes received:\n\n %s %d\n\n", buf1, recvlen);    
            t = end.tv_usec - start.tv_usec;
            printf("The RTT is: %d\n", t);
        }
	memset(buf1, '\0', sizeof(buf1));
	system("date");
    }

    close(sd);
    return 0;
}




