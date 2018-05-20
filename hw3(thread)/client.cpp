#include "client.h"

void Client::ClientCreate()
{
    // create socket fd
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	// socket fd failed
	if (serverSocket == -1)
    {
		ERR_EXIT("socket fd failed");
	}

	cout<<"Client Create Success"<<endl;
}

void Client::ClientConnect()
{
    // connect to server
    memset(&serverAddr, 0, sizeof(serverAddr)); // full of zero
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddr.sin_port = htons(serverPort);

    if(connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
    	ERR_EXIT("connect failed");
    }

    cout<<"Server Connect Success With "<<serverIP<<":"<<serverPort<<endl;
}

void Client::SendString(string message)
{

	send(serverSocket, message.c_str(), message.length(), 0);

	//cout<<"[SendString] "<<message<<endl;
}

string Client::ReadString()
{
	char message[BUF_SIZE] = {};
    int nbytes = 0;

	if((nbytes = recv(serverSocket, message, sizeof(message), 0)) < 0)
    {
        ERR_EXIT("recv");
    }

    if(nbytes == 0)
    {
        return "server shutdown";
    }

    //cout<<"[ReadString] "<<message<<endl;

	return message;
}