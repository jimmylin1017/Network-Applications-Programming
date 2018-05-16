#include "header.h"

#define BUF_SIZE 1024

class Server
{
    char hostIP4[INET_ADDRSTRLEN]; // server ip address
    int serverPort; // server port number

    int serverSocket; // server socket file descriptor
	struct sockaddr_in serverAddr; // server information : sin_family, sin_addr, sin_port
	
    string clientName;
    int clientSocket; // accept client socket file descriptor
	struct sockaddr_in clientAddr; // client information : sin_addr, sin_port

    fd_set readFdSet;
    int maxReadFd;

    map<string, int> clientSocketMap; // contain all client socket file descriptor <name, client>
    map<string, sockaddr_in>  clientSocketAddrMap;
    map<string, bool> clientSocketOnline;

public:

    Server(int port):
    serverPort(0),
    serverSocket(0),
    clientName(""),
    clientSocket(0),
    maxReadFd(0)
    {
        memset(hostIP4, 0, sizeof(hostIP4));
        memset(&serverAddr, 0, sizeof(serverAddr));
        memset(&clientAddr, 0, sizeof(clientAddr));
        
        serverPort = port; // set port number

        FD_ZERO(&readFdSet); // clear read fd set
        clientSocketMap.clear();
        clientSocketAddrMap.clear();
        clientSocketOnline.clear();
    }

    void ServerCreate();
    bool ServerListen();
    void SendString(string clientName, string message);
    void BroadCast(string message);

    string ReadString(int targetClientSocket);

    void ClientHandler(int targetClientSocket);
};
