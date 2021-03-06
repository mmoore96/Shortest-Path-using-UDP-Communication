//
//  clientSide.cpp
//  shortest path using UDP communication
//
//  Created by Michael Moore on 3/26/20.
//  Copyright © 2020 George Moore. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#define PORTNUM 12354
#define BUFMAX 1024
#define PLIK    "/Users/XCodeProjects/Final\ Project/serverSide/clientSide/send.txt"
#define START    "/Users/XCodeProjects/Final\ Project/serverSide/clientSide/adk.txt"
#define LENGTH 512
void error(const char*);
void macLinuxEchoLoop(int, struct sockaddr*, socklen_t);
void macLinuxEchoClient();
char connected = '1';
bool didStart = false;

int main(int argc, char** argv)
{
    macLinuxEchoClient();


    puts("Press any key to continue");
    getc(stdin);

    return EXIT_SUCCESS;
}

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void macLinuxEchoClient()
{
    int socketFileDescriptor;
    struct sockaddr_in serverAddr;

    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORTNUM);
    serverAddr.sin_addr.s_addr = inet_addr("192.168.1.104");

    inet_pton(AF_INET, "192.168.1.104", &serverAddr.sin_addr);

    socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(socketFileDescriptor < 0)
        error("socket() failed");

    macLinuxEchoLoop(socketFileDescriptor, (struct sockaddr*)& serverAddr, sizeof(serverAddr));
}

void macLinuxEchoLoop(int sock, struct sockaddr* serverAddr, socklen_t serverlen)
{
    
    int bytesRead;
    int sentResult;
    char inputBuffer[BUFMAX] = {0};
    char recvBuffer[BUFMAX] = {0};
    
    if (didStart == false){
        FILE *fa = fopen(START, "r");
        size_t Read1 = fread(inputBuffer, sizeof(char), LENGTH, fa);
        sentResult = sendto(sock, inputBuffer, Read1, 0, serverAddr, serverlen);
        fclose(fa);
        bytesRead = recvfrom(sock, recvBuffer, BUFMAX, 0, NULL, NULL);
        printf("%s", recvBuffer);
        didStart = true;
    }
    
    for(;;)
    {
        
        char start[32];
        char end[32];
        FILE *file;
        file = fopen(PLIK, "wt");
        printf("\nFrom: ");
        scanf("%s", start);
        if (strcmp (start, "exit") == 0){
            fclose(file);
            exit(0);
        }
        printf("To:  "); scanf("%s", end);
        printf("\n");
        fprintf(file, "%s\n%s\n", start, end);
        fclose(file);
        FILE *fp = fopen(PLIK, "r");
        size_t Read = fread(inputBuffer, sizeof(char), LENGTH, fp);
        
        sentResult = sendto(sock, inputBuffer, Read, 0, serverAddr, serverlen);
        if(sentResult < 0)
            error("sendTo() failed");

        bytesRead = recvfrom(sock, recvBuffer, BUFMAX, 0, NULL, NULL);

        if(bytesRead < 0)
            error("recvfrom() failed");

        recvBuffer[bytesRead] = 0;
        printf(recvBuffer, "\n");
        fclose(fp);
        
        //exit(0);

    }
    
}

