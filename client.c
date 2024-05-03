#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<netinet/in.h>

int main(int argc, char** argv){
    const int socketfd = socket(PF_INET, SOCK_STREAM, 0);

    FILE *file = fopen("port", "r");

    char buff[32];
    fgets(buff, sizeof(buff), file);
    fclose(file);

    int PORT = atoi(buff);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    if(connect(socketfd, (const struct sockaddr *) &addr, sizeof(addr))!=0){
	perror("connect");
	return -1;
    }

    char buf[1024];
    while(1){
	puts("Write something to send to the server: ");
	fgets(buf, sizeof(buf), stdin);
	printf("The value of buf is %s \n", buf);
	if(buf[0]=='+'){
	    send(socketfd, buf, sizeof(buf), 0);
	    printf("this is not good at all");
	    break;
	}

	if (send(socketfd, buf, sizeof(buf), 0) == -1){
	    puts("A mistakeeeee\n");
	    break;
	}
    }
    puts("Terminating client.");
    sleep(1);
    puts(".");
    sleep(1);
    puts(".");
    return 0;
}
