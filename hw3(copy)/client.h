#include "header.h"

#define BUF_SIZE 512

class Client
{
    string serverIP;
    int serverPort;
    int serverSocket;
	struct sockaddr_in serverAddr;

    fd_set readFdSet;
    int maxReadFd;

public:

    Client(string ip, int port):
    serverSocket(0),
    maxReadFd(0)
    {
        memset(&serverAddr, 0, sizeof(serverAddr));

        serverIP = ip;
        serverPort = port;

        FD_ZERO(&readFdSet);
    }

    void ClientCreate();
	void ClientConnect();
    void ClientStart();
    void SendString(string message);
	string ReadString();
};