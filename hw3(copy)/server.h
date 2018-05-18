#include "header.h"

#define BUF_SIZE 1024
#define CLIENT_LIMIT 10

class ClientHandler
{
	string clientName;
    int clientSocket; // accept client socket file descriptor
	struct sockaddr_in clientAddr; // client information : sin_addr, sin_port

public:

    static map<string, int> clientSocketMap; // contain all client socket file descriptor <name, client>
    static map<string, sockaddr_in>  clientSocketAddrMap;
    static map<string, bool> clientSocketOnline;
    static map<string, vector<string>> clientSocketOffLineMessage;

	string ReadMessage();
    void SendMessage(int targetClientSocket, string message);
    void BroadCastMessage(string message);
    bool ExecuteCommand(string command);

    ClientHandler(int clientSocket, struct sockaddr_in clientAddr):
    clientName("")
    {
        memset(&(this->clientAddr), 0, sizeof(this->clientAddr));

        this->clientSocket = clientSocket;
        this->clientAddr = clientAddr;
    }
};

class Server
{
    char hostIP4[INET_ADDRSTRLEN]; // server ip address
    int serverPort; // server port number

    int serverSocket; // server socket file descriptor
	struct sockaddr_in serverAddr; // server information : sin_family, sin_addr, sin_port
	
    int clientSocket; // accept client socket file descriptor
	struct sockaddr_in clientAddr; // client information : sin_addr, sin_port

public:

    Server(int port):
    serverPort(0),
    serverSocket(0),
    clientSocket(0)
    {
        memset(hostIP4, 0, sizeof(hostIP4));
        memset(&serverAddr, 0, sizeof(serverAddr));
        memset(&clientAddr, 0, sizeof(clientAddr));
        
        serverPort = port; // set port number
    }

    void ServerCreate();
    bool ServerListen();

    static void ClientHandlerThread(ClientHandler clientHandler);
};
