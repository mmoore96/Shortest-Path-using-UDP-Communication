# Shortest-Path-using-UDP-Communication

Program written in C++ that mimics how GPS routing systems work on a basic level. A client connects to the server. The user will enter a starting and ending location which is sent to the server. The server will send the shortest path back to the user. Dijkstra's Shortest Path First algorithm is used along with UDP for communication.

## Installation

You can download the files directly from github or using git clone in terminal.

```bash
git clone https://github.com/mmoore96/Shortest-Path-using-UDP-Communication.git
```

## Setup
1. Navigate the the clientSide and serverSide directory.
2. Open the main.cpp file of both in an IDE or text editor of your liking.
3. Change the following lines of code:

clientSide:
```C++
#define PORTNUM 12354  //enable port forwarding if using machines outside your LAN.
#define BUFMAX 1024    

//change to the path of the files on your computer
#define PLIK    "/User/Folder/clientSide/send.txt"
#define START    "/User/Folder/clientSide/adk.txt"
.
.
.
.
.
//change to IP address of the server
inet_pton(AF_INET, "xxx.xxx.x.xxx", &serverAddr.sin_addr);
```


serverSide:
```C++
#define PORTNUM 12354  //enable port forwarding if using machines outside your LAN.
.
.
.
.
.
.
//change to the path of the files on your computer
#define PLIKSERV    "/User/Folder/serverSide/toServer.txt"
#define shortestPathFile    "/User/Folder/serverSide/toClient.txt"
```
4. Change all instances of file paths in the serverSide code.

*Future commits will fix the need for this.*

## Usage

Run the serverSide first followed by the clientSide.

*I recommend using Xcode to run this project. See section titled "Important" for further details.*


## Example Output

clientSide:
```bash
Home,School,GroceryStore
GroceryStore,Home,HardwareStore
School,Home,HardwareStore,Mall,StripMall
HardwareStore,GroceryStore,School,Mall,GasStation
GasStation,HardwareStore
Mall,HardwareStore,School,StripMall,Bank
StripMall,School,Mall,Bank,Park
Park,StripMall
Bank,StripMall,Mall,BookStore
BookStore,Bank

From: HardwareStore
To:  Park

** Shortest Path **
Time: 1 hour, 20 minutes
Path: HardwareStore->School->StripMall->Park

** All Distances From HardwareStore **
(Bank:53), (Park:80), (StripMall:34), (Home:42), (Mall:37), (GasStation:2), (HardwareStore:0), (School:4), (BookStore:63), (GroceryStore:16)
```

## Important
This program was written as a final project for one of my undergraduate Computer Science classes.
This is intended to run on MacOS using Xcode. It was compiled using LLVM and Clang. However, any C++ compiler will work.

I plan on updating this code in the future so users do not have to change file paths manually in the code.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
GNU General Public License v3.0
