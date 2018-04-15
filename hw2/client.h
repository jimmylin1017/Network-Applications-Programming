#include "header.h"

#define BUF_SIZE 512

class Client
{
    string serverIP;
    int serverPort;
    int serverSocket;
	struct sockaddr_in serverAddr;

public:

    Client(string ip, int port):
    serverSocket(0)
    {
        memset(&serverAddr, 0, sizeof(serverAddr));

        serverIP = ip;
        serverPort = port;
    }

    bool ClientCreate();
	bool ClientConnect();
    void WriteString(string message);
	string ReadString();
    int GetFileSizeInBytes(string fileName);
    bool SendFile(string fileName);

};