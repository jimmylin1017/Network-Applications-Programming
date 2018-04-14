#include "header.h"

#define BUF_SIZE 512

class Server
{
    struct sockaddr_in *hostAddr;
    char hostIP4[INET_ADDRSTRLEN];
    int serverPort;
    int serverSocket;
	struct sockaddr_in serverAddr;
    int serverSocketForFile;
	struct sockaddr_in serverAddrForFile;
	int clientSocket;
	struct sockaddr_in clientAddr;
    int clientSocketForFile;
	struct sockaddr_in clientAddrForFile;

    struct ifaddrs *hostAddrInfo;

public:

    Server(int port):
    hostAddr(nullptr),
    serverPort(0),
    serverSocket(0),
    serverSocketForFile(0),
    clientSocket(0),
    clientSocketForFile(0),
    hostAddrInfo(nullptr)
    {
        memset(hostIP4, 0, sizeof(hostIP4));
        memset(&serverAddr, 0, sizeof(serverAddr));
        memset(&serverAddrForFile, 0, sizeof(serverAddrForFile));
        memset(&clientAddr, 0, sizeof(clientAddr));
        memset(&clientAddrForFile, 0, sizeof(clientAddrForFile));
        
        serverPort = port;
    }

    bool ServerCreate();
    void ServerListen();
    void GetHostInfo();
    void WriteString(string message);
    string ReadString();
    void ReceiveFile(string fileName, int fileSize);
};