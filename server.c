#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<pthread.h>
#include<errno.h>

#include"server.h"

int CLIENT_NUMBER = 3;
struct data{
    uint64_t thread_id;
    uint64_t fd;
};

void *handleclientconnection(void *cfd){
    char buf[1024];
    int *fd = cfd;
    pthread_t id = pthread_self();

    printf("The client descriptor is: %d\n", *fd);

    while(1){
	int retval = recv(*fd, buf, sizeof(buf), 0);
	if (retval==-1 || retval==0) break;
	printf("client: %d app says: %s\n", *fd, buf);
	if (buf[0] == '+' || buf[0]=='\0') {
	    puts("The client has terminated the ");
	    break;
	}
    }

    CLIENT_NUMBER--;
    printf("Closing the connection to this fd: %d\n", *fd);
    close(*fd);
    free(cfd);
    pthread_cancel(id);
    return NULL;
}

int main(int argc, char** argv){
    const int socketfd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(socketfd, (const struct sockaddr *) &addr, sizeof(addr));
    pthread_t tid[10];
    int i = 0;

    socklen_t addr_len = sizeof(addr);
    getsockname(socketfd, (struct sockaddr*) &addr, &addr_len);

    int port_number = ntohs(addr.sin_port);
    printf("The server is on port %d\n", port_number);
    FILE *file = fopen("port", "w");
    if(!file){
	printf("An error occured %d", errno);
	return -1;
    }
    fprintf(file, "%d\n", port_number);
    fclose(file);

    if (listen(socketfd, 1)){
	perror("listen error");
	return -1;
    }
    while(1){
	if(CLIENT_NUMBER==0) continue;
	struct sockaddr_storage caddr;
	socklen_t cadd_len = sizeof(caddr);	
	int *cfd = malloc(sizeof(int));
	*cfd = accept(socketfd, (struct sockaddr*)&caddr, &cadd_len);

	pthread_create(&(tid[i]), NULL, handleclientconnection, cfd);
	CLIENT_NUMBER++;
    }

    return 0;
}

