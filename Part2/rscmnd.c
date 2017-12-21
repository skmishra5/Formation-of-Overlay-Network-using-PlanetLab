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

int main(int argc, char **argv)
{
    int sd; /*to create socket*/
    int newsd; /*to accept connection*/

    struct sockaddr_in serverAddress;//server receive on this address
    struct sockaddr_in clientAddress;//server sends to client on this address
    struct hostent *hostp; /* client host info */

    int nrecv, exec_times, delay, i;
    char buf[MAXLEN], status[MAXLEN], buf_sent[MAXLEN], temp_buf[MAXLEN];
    char command[MAXLEN], cmd[MAXLEN], cmd1[MAXLEN], token[MAXLEN];
    int clientAddressLength;
    int pid, port;
    char *hostaddrp; /* dotted decimal host addr string */
    FILE *file;
    pthread_mutex_t var = PTHREAD_MUTEX_INITIALIZER;
    char filename[5];

    if((argc < 3) || (strcmp(argv[1], "-h") == 0))
    {
        printf("<rscmnd_tcp> -p <port number> \n");
        printf("(<rscmnd_tcp> -h) to get the help\n");
        return 0;
    }


    if(strcmp(argv[1], "-p") == 0)
    {
        port = atoi(argv[2]);
    }

    /* create socket */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0)
    {
        printf("socket creation failed \n");
	return 0;
    }

    /* initialize the socket addresses */
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
    serverAddress.sin_port=htons(port);

    /* bind the socket with the server address and port */
    if (bind(sd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf(" bind failed \n");
	return 0;
    }

    /* listen for connection from client */
    listen(sd, 5);

    while(1)
    {
        /* parent process waiting to accept a new connection */
        printf("\n server waiting for new client connection: \n");
  	clientAddressLength=sizeof(clientAddress);
  	newsd = accept(sd, (struct sockaddr*)&clientAddress, &clientAddressLength);
        printf("connected to client: %s\n", inet_ntoa(clientAddress.sin_addr));
        hostp = gethostbyaddr((const char *)&clientAddress.sin_addr.s_addr, sizeof(clientAddress.sin_addr.s_addr), AF_INET);
    	if (hostp == NULL)
        {
            printf("ERROR on gethostbyaddr");
        }
    	hostaddrp = inet_ntoa(clientAddress.sin_addr);
    	if (hostaddrp == NULL)
        {
      	    printf("ERROR on inet_ntoa\n");
        }
    	printf("server established connection with %s (%s)\n", hostp->h_name, hostaddrp);
            //pthread_mutex_lock(&var);
	   // visits = visits + 1;
           // pthread_mutex_unlock(&var);

        //file = fopen("temp", "w+");
        /* child process is created for serving each new clients */
        pid=fork();
        if(pid == 0) /* child process rec and send */
        {
            /* rceive from client */
    	    nrecv = recv(newsd, buf, MAXLEN, 0);
            printf("Buffer received: %s\n", buf);
            strcpy(status, "Connected !!");
	    printf("the status is : %s \n", status);
            if(nrecv == 0)
    	    {
     		close(newsd);
     		break;
            }
    	    buf[nrecv] = 0;
	    strcpy(command, strtok(buf, ":"));
            strcpy(token, strtok(NULL, ":"));
            exec_times = atoi(token);
            strcpy(token, strtok(NULL, ":"));
            delay = atoi(token);
            strcpy(cmd, command);
	    //printf("visits:%d", visits);
            //sprintf(filename, "%d.txt", visits);
            //file = fopen(filename, "w+");
            //strcat(cmd, ">");
            //strcat(cmd, filename);
	    system("date");
            for(i = 0; i < exec_times; i++)
            {
		file = popen(cmd, "r");
		if (file == NULL)
		{
		    printf("Command running failed \n");
		}
                //system(cmd);
	        //system("echo " " >> temp");
		//strcpy(cmd1, "date >>");
		//strcat(cmd1, filename);
	        //system(cmd1);
                //sleep(delay);
            //}
                memset(temp_buf, '\0', sizeof(temp_buf));
                memset(buf_sent, '\0', sizeof(buf_sent));
            	if (file)
            	{
                    while (fscanf(file, "%s", temp_buf) != EOF)
                        strcat(buf_sent, temp_buf);
                 //fclose(file);
            	}
            	send(newsd, buf_sent, strlen(buf_sent) + 1, 0);
            	sleep(delay);
		pclose(file);
	    } 	
            //pclose(file);
	    //strcpy(cmd1, "rm ");
	    //strcat(cmd1, filename);
            //system(cmd1);
            strcpy(status, " Closed!!");
            printf("IP address is: %s\n", inet_ntoa(clientAddress.sin_addr));
            printf("The command is: \"%s\" and the status is:%s\n", command, status);
        }
  	else
  	{
   	    close(newsd);//sock is closed BY PARENT
  	}
     }//close exterior while

     return 0;
}




