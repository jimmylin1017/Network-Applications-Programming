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

    map<string, int> *clientSocketMap; // contain all client socket file descriptor <name, client>

public:

    Server(int port):
    serverPort(0),
    serverSocket(0),
    clientName(""),
    clientSocket(0)
    {
        memset(hostIP4, 0, sizeof(hostIP4));
        memset(&serverAddr, 0, sizeof(serverAddr));
        memset(&clientAddr, 0, sizeof(clientAddr));
        
        serverPort = port;

        clientSocketMap = new map<string, int>();
    }

    void ServerCreate();
    bool ServerListen();
    void SendString(string clientName, string message);
    void BroadCast(string message);

    string ReadString();

    void ClientHandler();
};
