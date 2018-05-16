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

	cout<<"Send : "<<message<<endl;
}

string Client::ReadString()
{
	char message[BUF_SIZE] = {};

	if(recv(serverSocket, message, sizeof(message), 0) < 0)
    {
        ERR_EXIT("recv");
    }

	return message;
}

void Client::ClientStart()
{
    FD_SET(STDIN_FILENO, &readFdSet);
    FD_SET(serverSocket, &readFdSet);
    maxReadFd = serverSocket > STDIN_FILENO ? serverSocket : STDIN_FILENO;

    string message = "", command = "";

    while(1)
    {
        if(select(maxReadFd + 1, &readFdSet, NULL, NULL, NULL) < 0)
        {
            ERR_EXIT("select");
        }

        if(FD_ISSET(STDIN_FILENO, &readFdSet))
        {
            cin>>command;
            SendString(command);
            cout<<"STDIN_FILENO"<<endl;
        }

        if(FD_ISSET(serverSocket, &readFdSet))
        {
            cout<<"serverSocket"<<endl;
            message = ReadString();
            cout<<"receive : "<<message<<endl;
        }
    }
}