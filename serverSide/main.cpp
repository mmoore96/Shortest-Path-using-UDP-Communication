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
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <limits>
#include <fstream>
#include <sstream>
#define LENGTH 512
#define PORTNUM 12354
#define BUFMAX 1024
#define PLIKSERV    "/Users/XCodeProjects/Final\ Project/serverSide/serverSide/toServer.txt"
#define shortestPathFile    "/Users/XCodeProjects/Final\ Project/serverSide/serverSide/toClient.txt"

void macLinuxEchoLoop(int, struct sockaddr*, socklen_t);
void macLinuxEchoServer();
void sendLinuxEchoClient();
void multi(int);
void driver(int);
bool sent = false;
void error(const char*);
void sendLinuxEchoLoop(int, struct sockaddr*, socklen_t);
struct sockaddr_in serverAddr;
struct sockaddr_in clientAddr;
int start();




void macLinuxEchoServer()
{
    int socketFileDescriptor;
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
    char check[32] = "1";
    socklen_t len;
    char inputBuffer[BUFMAX] = {0};
    char msg[BUFMAX] = {0};
    char recvBuffer[BUFMAX] = {0};
    FILE *fp = fopen(PLIKSERV, "w");
    

    printf("Waiting for datagrams on 127.0.0.1:%d\n", PORTNUM);
    for(;;)
    {
        int i;
        len = clilen;
        bzero(&msg, sizeof(msg));
        
        bytesRead = recvfrom(sockFd, msg, BUFMAX, 0, cliaddr, &len);
        //scanf("%d", msg);
        if (*msg == '1'){
            FILE *fl = fopen("/Users/XCodeProjects/Final\ Project/serverSide/serverSide/graph.txt", "r");
            int Read = fread(inputBuffer, sizeof(char), LENGTH, fl);
            
            printf("Sending: ");
            sendto(sockFd, inputBuffer, Read, 0, cliaddr, len);
            fclose(fl);
            sent = true;
        }else{
            fwrite(msg, 1, bytesRead, fp);
            sscanf(msg, "%d", &i);
            fflush(fp);
            printf("Got message: %s\n", msg);
            start();
            fclose(fp);
            FILE *fl = fopen(shortestPathFile, "r");
            int Read = fread(inputBuffer, sizeof(char), LENGTH, fl);
            
            
            fclose(fl);
            
            sendto(sockFd, inputBuffer, Read, 0, cliaddr, len);
            macLinuxEchoLoop(sockFd, (struct sockaddr*)& clientAddr, sizeof(clientAddr));
            
        }
    }
    
}


void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}


int main(int argc, char** argv)
{
    macLinuxEchoServer();
    
}

using namespace std;
class Node;
class Edge {
public:
    Edge(Node* endNode, int weight) : endNode(endNode), weight(weight) {};
    Node* getEndNode() const { return endNode; }
    int getWeight() const { return weight; }

private:
    Node* endNode;
    int weight;
};




class Node {
public:
    Node(string locationName) : locationName(locationName) {};
    void setLocationName(string locationName) { this->locationName = locationName; }
    string getLocationName() const { return locationName; }
        void setDistance(int distance) {this->distance = distance;}
        int getDistance() const {return distance;}
        void setPreviousNode(Node* previousNode) {this->previousNode = previousNode;}
        Node* getPreviousNode() const {return previousNode;}
        void setVisited(bool visited) {this->visited = visited;}
        bool isVisited() const {return visited;}
    void addEdge(Node* endNode, int weight);
    unordered_map<string, Edge*> getEdges() const;
        string toString() const;

private:
    string locationName;
    unordered_map<string, Edge*> edges;
        int distance;
        Node* previousNode;
        bool visited;
};

void Node::addEdge(Node* endNode, int weight) {
    edges.emplace(endNode->getLocationName(), new Edge(endNode, weight));
}

unordered_map<string, Edge*> Node::getEdges() const {
    return edges;
}

string Node::toString() const {
    string edgeString = locationName + " ->";
    for (const auto& currEdge : edges) {
        edgeString += " (" + currEdge.first + ":" + to_string(currEdge.second->getWeight()) + "),";
    }
    edgeString.erase(edgeString.length() - 1);
    return edgeString;
}



class Graph {
public:
    Graph() {};
    void addEdge(string vertex1, string vertex2, int weight);
    string toString() const;
        string dijkstraShortestPath(string startV, string endV);

private:
    unordered_map<string, Node*> nodes;
    Node* getNode(string node);
        string getPath(string startV, string endV) const;
        string getAllDistances(string startV) const;
};

void Graph::addEdge(string vertex1, string vertex2, int weight) {
    Node* v1 = getNode(vertex1);
    Node* v2 = getNode(vertex2);

    v1->addEdge(v2, weight);
    v2->addEdge(v1, weight);
}

Node* Graph::getNode(string node) {
    if (nodes.count(node) == 0) {
        nodes.emplace(node, new Node(node));
    }
    return nodes.at(node);
}

string Graph::toString() const {
    string graph = "";
    for (const auto& currNode : nodes) {
        graph += currNode.second->toString() + "\n";
    }
    graph.erase(graph.length() - 1);
    return "** Node -> (Edge:Weight) **\n" + graph;
}

string Graph::dijkstraShortestPath(string startV, string endV) {
    if ((nodes.count(startV) == 0) || (nodes.count(endV) == 0)) {
        return "Starting and/or ending location not in graph.";
    }
    
    unordered_map<string, Node*> unvisitedNodes;
    int edgeWeight, newPathDistance;
    Node* currNode;
    Node* adjacentNode;
    
    for (const auto& current : nodes) {
        current.second->setDistance(numeric_limits<int>::max());
        current.second->setPreviousNode(nullptr);
        current.second->setVisited(false);
        unvisitedNodes.emplace(current.first, current.second);
    }
    
    currNode = unvisitedNodes.at(startV);
    currNode->setDistance(0);
    
    while (!unvisitedNodes.empty()) {
        for (const auto& current : unvisitedNodes) {
            if (currNode == nullptr) {
                currNode = current.second;
            }
            else if (current.second->getDistance() < currNode->getDistance()) {
                currNode = current.second;
            }
        }
        currNode->setVisited(true);
        for (const auto& currEdge : currNode->getEdges()) {
            adjacentNode = currEdge.second->getEndNode();
            if (!adjacentNode->isVisited()) {
                edgeWeight = currEdge.second->getWeight();
                newPathDistance = currNode->getDistance() + edgeWeight;

                if (newPathDistance < adjacentNode->getDistance()) {
                    adjacentNode->setDistance(newPathDistance);
                    adjacentNode->setPreviousNode(currNode);
                }
            }
        }
        unvisitedNodes.erase(currNode->getLocationName());
        currNode = nullptr;
    }
    
    return getPath(startV, endV) + "\n\n" + getAllDistances(startV);
}

string Graph::getPath(string startV, string endV) const {
    string path = "";
    Node* currNode = nodes.at(endV);
    int endDistance = currNode->getDistance();
    
    if (currNode->getPreviousNode() == nullptr) {
        return "No path from " + startV + " to " + endV + ".";
    }
    
    while (currNode->getPreviousNode() != nullptr) {
        path = "->" + currNode->getLocationName() + path;
        currNode = currNode->getPreviousNode();
    }
    
    return "** Shortest Path **\nTime: " + to_string(endDistance / 60) +
            ((endDistance / 60 == 1) ? " hour, " : " hours, ") +
            to_string(endDistance % 60) + " minutes\nPath: " +
            currNode->getLocationName() + path;
}

string Graph::getAllDistances(string startV) const {
    string distances = "";
    for (const auto& currNode : nodes) {
        distances += ", (" + currNode.first + ":" + to_string(currNode.second->getDistance()) + ")";
    }
    distances.erase(0,2);
    
    return "** All Distances From " + startV + " **\n" + distances;
}

Graph* createGraph(ifstream& inFileStream, ofstream& outFileStream) {
    istringstream inStringStream;
    Graph* g = new Graph;
    string currLine, currNode, currEdge;
    
    inFileStream.open("/Users/XCodeProjects/Final\ Project/serverSide/serverSide/graph.txt");
    
    if (!inFileStream.is_open()) {
        cout << "Could not open file graph.txt." << endl;
        exit(1);
    }
    
    getline(inFileStream, currLine);
    while (!inFileStream.fail()) {
        inStringStream.str(currLine);
        getline(inStringStream, currNode, ',');
        
        while (getline(inStringStream, currEdge, ',')) {
            g->addEdge(currNode, currEdge, (rand() % 60) + 1);
        }
        inStringStream.clear();
        getline(inFileStream, currLine);
    }
    
    if (!inFileStream.eof()) {
        cout << "Error occurred while reading file graph.txt." << endl;
        inFileStream.close();
        exit(1);
    }
    
    inFileStream.close();
    
    outFileStream.open("toClient.txt");
    
    if (!outFileStream.is_open()) {
        cout << "Could not open file toClient.txt." << endl;
        exit(1);
    }
    
    outFileStream << g->toString();
    outFileStream.close();
    
    return g;
}




void driver(ofstream& outFileStream, Graph* g, string startingLocation, string endingLocation) {
    outFileStream.open("/Users/XCodeProjects/Final\ Project/serverSide/serverSide/toClient.txt");
    
    if (!outFileStream.is_open()) {
        cout << "Could not open file toClient.txt." << endl;
        exit(1);
    }
    
    outFileStream << g->dijkstraShortestPath(startingLocation, endingLocation);
    outFileStream.close();
}

int start()
{
    ifstream inFileStream;
    ofstream outFileStream;
    string startingLocation, endingLocation;
    srand(time(0));
    
        Graph* g = createGraph(inFileStream, outFileStream);
        
        inFileStream.open("/Users/XCodeProjects/Final\ Project/serverSide/serverSide/toServer.txt");
        
        if (!inFileStream.is_open()) {
            cout << "Could not open file toServer.txt." << endl;
            exit(1);
        }
        
        getline(inFileStream, startingLocation);
        getline(inFileStream, endingLocation);
        
        inFileStream.close();
        
        driver(outFileStream, g, startingLocation, endingLocation);
      
        return 0;
    }
   

