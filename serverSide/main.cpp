//
//  main.cpp
//  rUDP
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
#include <iostream>
#include <climits>
#include <list>
#include<fstream>
#define LENGTH 512
#define PORTNUM 12354
#define BUFMAX 1024
#define PLIKSERV    "/Users/XCodeProjects/Final\ Project/serverSide/serverSide/receive.txt"
#define shortestPathFile    "/Users/XCodeProjects/Final\ Project/serverSide/serverSide/path.txt"
void macLinuxEchoLoop(int, struct sockaddr*, socklen_t);
void macLinuxEchoServer();
void multi(int);
void driver(int);
//using namespace std;

int main(int argc, char** argv)
{
    macLinuxEchoServer();


    puts("Press any key to continue");
    getc(stdin);

    return EXIT_SUCCESS;
}

void macLinuxEchoServer()
{
    int socketFileDescriptor;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORTNUM);

    bind(socketFileDescriptor, (struct sockaddr*)& serverAddr, sizeof(serverAddr));

    macLinuxEchoLoop(socketFileDescriptor, (struct sockaddr*)& clientAddr, sizeof(clientAddr));
}

void macLinuxEchoLoop(int sockFd, struct sockaddr* cliaddr, socklen_t clilen)
{
    int bytesRead;
    socklen_t len;
    char inputBuffer[BUFMAX] = {0};
    char msg[BUFMAX] = {0};
    char buf[512],sbuf[LENGTH];
    FILE *fp = fopen(PLIKSERV, "w");
    char* mfcc;
    int multiVar = 0;
    //(char*)malloc(1500);
    printf("Waiting for datagrams on 127.0.0.1:%d\n", PORTNUM);
    for(;;)
    {
        len = clilen;
        bzero(&msg, sizeof(msg));
        
        bytesRead = recvfrom(sockFd, msg, BUFMAX, 0, cliaddr, &len);
        fwrite(msg, 1, bytesRead, fp);
        int i;
        sscanf(msg, "%d", &i);
        fflush(fp);
        //("Got message: %s\n", msg);
        multi(i);
        FILE *fl = fopen(shortestPathFile, "r");
        int Read = fread(inputBuffer, sizeof(char), LENGTH, fl);
        
        //sprintf( msg, "%d", string(multi(i)) );
        //printf("Sending: %s", msg);
        sendto(sockFd, inputBuffer, Read, 0, cliaddr, len);
        fclose(fp);
        fclose(fl);
        //free(mfcc);
    }
    
}

void multi(int a){
    
    return driver(a);
    
}


class Graph
{
    int V;    // No. of vertices

    // Pointer to an array containing adjacency
    // lists
    std::list<int> *adj;
public:
    Graph(int V);  // Constructor

    // function to add an edge to graph
    void addEdge(int v, int w);

    // prints BFS traversal from a given source s
    void BFS(int s);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new std::list<int>[V];
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
}

void Graph::BFS(int s)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    char buffer[BUFMAX] = {0};
    for(int i = 0; i < V; i++)
        visited[i] = false;

    // Create a queue for BFS
    std::list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);

    // 'i' will be used to get all adjacent
    // vertices of a vertex
    std::list<int>::iterator i;
    std::ofstream ofile;
    ofile.open("/Users/XCodeProjects/Final\ Project/serverSide/serverSide/path.txt", std::ios::trunc);
    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        std::list<int> toSend;
        std::cout << s << " ";
        //FILE *fp = fopen(shortestPathFile, "w");
        //fwrite(s, 1, buffer, fp);
        ofile << s << std::endl;
        
        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
    }
    ofile.close();
}

void driver(int start)
{
    // Create a graph given in the above diagram
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);

    std::cout << "Following is Breadth First Traversal "
         << "(starting from vertex) \n";
    g.BFS(start);
}
