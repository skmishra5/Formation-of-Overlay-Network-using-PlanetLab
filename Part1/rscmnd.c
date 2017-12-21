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
#include <pthread.h>

#define MAXLEN 4096
int visits = 0;

/* Structure defination to serve one client at a time */
struct clientRequest
{
    int delay;
    int exec_time;
    char command1[MAXLEN];
    FILE *file;
};

int main(int argc, char **argv)
{
    struct sockaddr_in servAddr;  /* Server address */
    struct sockaddr_in cliAddr;   /* Client address */
    
    struct clientRequest clre[10];

    socklen_t addrlen = sizeof(cliAddr); /* Length of client address*/  
    int recvlen, exec_times, delay, i, count = 0;
    int sd; /*Socket descriptor*/
    unsigned char buf[MAXLEN], temp_buf[MAXLEN], buf_sent[MAXLEN];  /*Buffer used to receive data*/
    char token[MAXLEN], status[MAXLEN];
    char command[MAXLEN], cmd[MAXLEN], cmd1[MAXLEN];
    struct hostent *hostp;
    char *hostaddrp;
    int optval, port;
    pthread_mutex_t var = PTHREAD_MUTEX_INITIALIZER;
    char filename[10];

    if(argc < 3)
    {
        printf("<rscmnd_udp> -p <port number> \n");
        printf("(<rscmnd_udp> -h) to get the help\n");
	return 0;
    }

    if(strcmp(argv[1], "-h") == 0)
    {
        printf("<rscmnd_udp> -p <port number>");
        return 0;
    }

    if(strcmp(argv[1], "-p") == 0)
    {
	port = atoi(argv[2]);
    }

    /* create a UDP socket */
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        printf("socket creation failed with socket descriptor: %d\n", sd);
        return 0;
    }

    //optval = 1;
    //setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, 
    //	     (const void *)&optval , sizeof(int));

    //memset((char *)&servAddr, 0, sizeof(servAddr));
    bzero((char *) &servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    /* Binding the scoket to IP address and port*/
    if (bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) 
    {
        printf("System call bind failed with socket descriptor: %d \n", sd);
        return 0;
    }
    
    while(1)
    {
        printf("waiting on port %d\n", port);
	visits = visits + 1;
	//printf("visits: %d", visits);
        bzero(buf, MAXLEN);
        recvlen = recvfrom(sd, buf, MAXLEN, 0, (struct sockaddr *)&cliAddr, &addrlen);
        if (recvlen < 0)
        {
	    printf("Received nothing \n");
	}
        strcpy(status, "Connected !!");
        printf("received %d bytes \n", recvlen);
        hostp = gethostbyaddr((const char *)&cliAddr.sin_addr.s_addr, 
			  sizeof(cliAddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL)
	{
            printf("ERROR on gethostbyaddr \n");
        }
        hostaddrp = inet_ntoa(cliAddr.sin_addr);
        if (hostaddrp == NULL)
        {
            printf("ERROR on inet_ntoa\n");
        }
	printf("IP address is: %s\n", inet_ntoa(cliAddr.sin_addr));

        if((recvlen > 0) & (count != 5))
        {
	    printf("The status is:%s \n", status);
	    pthread_mutex_lock(&var);  // lock the critical section
  	    sprintf(filename, "%d.txt", visits);
            clre->file = fopen(filename, "w+");
            buf[recvlen] = 0;
            strcpy(clre[count].command1, strtok(buf, ":"));
            strcpy(token, strtok(NULL, ":")); 
            clre[count].exec_time = atoi(token);
            strcpy(token, strtok(NULL, ":"));
            clre[count].delay = atoi(token);
	    strcpy(cmd, clre[count].command1);
            strcat(cmd, ">");	
	    strcat(cmd, filename);
	    system("date");
            memset(temp_buf, '\0', sizeof(temp_buf));
            memset(buf_sent, '\0', sizeof(buf_sent));
            for(i = 0; i < clre[count].exec_time; i++)
            {
	        system(cmd);
	        //system("date >> temp");
	//	sleep(clre[count].delay);	
           // }
           // memset(temp_buf, '\0', sizeof(temp_buf));
           // memset(buf_sent, '\0', sizeof(buf_sent));
	    if(clre->file) 
            {
	        while(fscanf(clre->file, "%s\n", temp_buf) != EOF)
			strcat(buf_sent, temp_buf);
               // fclose(clre->file);
            }
            //system("rm temp");
	    if(sendto(sd, buf_sent, strlen(buf_sent), 0, (struct sockaddr *)&cliAddr, addrlen)==-1)
	    {
                printf("sendto failed \n");
            }
	    sleep(clre[count].delay);	
	    }
            fclose(clre->file);
	    strcpy(cmd1, "rm ");
            strcat(cmd1, filename);
            system(cmd1);
        }
        strcpy(status, "Closed !!");
        printf("The command is: \"%s\" and the status is:%s\n", clre[count].command1, status);
        pthread_mutex_unlock(&var); // unlock
        count++;
	if(count == 10)
        {
	    printf("Maximum 10 clients are allowed \n");
	    break;
        }
   } 

    return 0;
}

