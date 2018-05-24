#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>

/*
 * Evan Matoske 02/12/2018
 * Sources:
 *  This program uses Beej guide to networking EXTENSIVELY. Seriously I can't stress enough how much I used it
 * I also used various other sources for random information on the C language as well as the book KR
*/


char * getHandle();
struct  addrinfo * get_addr_info(char*, char*);
int getSocket(struct addrinfo*);
void makeConnection(int, struct addrinfo*);
void sendMsg(char[500], int);
void recvMsg(int);
void chat(int, char*);

void error(const char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    
    char handle[10], ipAddr, port;
    struct addrinfo *res;
    int sockfd;
     if (argc != 3) {
        fprintf(stderr,"usage: insufficient arguments\n");
        exit(1);
    }
    
    do{
        printf("Please enter a handle to be recognized by: ");
        scanf("%s", handle);
        if(strlen(handle) > 10){
            printf("handle too long \n");
        }
    }while(strlen(handle) > 10);
    
    
    res = get_addr_info(argv[1], argv[2]);
    sockfd = getSocket(res);
    makeConnection(sockfd, res);
    chat(sockfd, handle);
   
	return 0;
}
/*
 * This is the meat and potatoes it gathers the user input and keeps the convo going. Checks for \quit.
*/
void chat(int sockfd, char* handle){
    char buffer[490];
    char sendOut[500];
    char recvIn[500];
    
    memset(buffer,'\0', sizeof(buffer));
    memset(sendOut,'\0', sizeof(sendOut));
    memset(recvIn,'\0', sizeof(recvIn));
    
    fgets(buffer, 490, stdin);
    //strcpy(sendOut, handle);
    //strcat(sendOut, buffer);
    while(1){
        
        printf("%sO> ", handle);
        fgets(buffer, 500, stdin);
        
        if(strcmp(buffer, "\\quit\n") == 0){
            sendMsg(buffer, sockfd);
            break;
        }
        
        strcpy(sendOut, handle);
        strcat(sendOut, "O> ");
        strcat(sendOut, buffer);
        sendMsg(sendOut, sockfd);
        memset(sendOut,'\0', sizeof(sendOut));
        
        recvMsg(sockfd);
    }
    
}
/*
 * Connects the sockets
*/

void makeConnection(int sockfd, struct addrinfo* res){
    
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        close(sockfd);
        fprintf(stderr, "client: connect");
        exit(1);
    }
}
/*
 * returns to you a socket descriptor that you can use in later system calls, or -1 on error.
*/
int getSocket(struct addrinfo *res){
    int sockfd;
    
    if ((sockfd = socket(res->ai_family, res->ai_socktype,
                         res->ai_protocol)) == -1){
        fprintf(stderr, "client failed to connect");
        exit(1);
    }
    return sockfd;
}

/*
 * Prepare to launch. This sets up structs we need later on
*/
struct addrinfo* get_addr_info(char* ip, char* port){
    
    int status;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    
    if ((status = getaddrinfo(ip, port, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }
    
    return res;
    
}
/*
 * Sends information gathered from the user.
 */
void sendMsg(char msg[500],int sockfd){
    
    int len, bytes_sent;
    bytes_sent = send(sockfd, msg, strlen(msg), 0);
    if(bytes_sent == -1){
        fprintf(stderr, "Error sending message");
        exit(1);
    }

    
    

}
/*
 * Gathers the information sent from the server
 */
void recvMsg(int sockfd){
 
    char buf[100];
    int numbytes;
    
    //Clears the buffer you'll get SIGFAULTS if you don't
    memset(buf,'\0', sizeof(buf));

    
    if ((numbytes = recv(sockfd, buf, 100-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
 
    printf("%s\n", buf);
 
}


