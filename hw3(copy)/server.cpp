#include "server.h"

void Server::ServerCreate()
{
    // create server socket fd
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	// socket fd failed
	if (serverSocket == -1) {

		ERR_EXIT("socket fd failed");
	}

    // bind socket fd with ip and port
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; // use IPv4
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // ip address
    serverAddr.sin_port = htons(serverPort); // port number

    // to avoid socket on use
    int reuseAddr = 1;
	socklen_t reuseAddrLength = sizeof(reuseAddr);
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, reuseAddrLength);

    // bind socket and check with failed
	if(bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        ERR_EXIT("socket bind failed");
    }

    cout<<"Server Create Successful With Port : "<<serverPort<<endl;
}


bool Server::ServerListen()
{
	cout<<"Listening......"<<endl;

	// listen 5 client
	if(listen(serverSocket, 5) < 0)
    {
        ERR_EXIT("listen");
    }

    FD_SET(serverSocket, &readFdSet);
    maxReadFd = serverSocket;

    while(1)
    {
        if(select(maxReadFd + 1, &readFdSet, NULL, NULL, NULL) < 0)
        {
            ERR_EXIT("select");
        }

        // new client want to connect
        if(FD_ISSET(serverSocket, &readFdSet))
        {
            socklen_t clientAddrSize = sizeof(clientAddr);
            clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

            if(clientSocket < 0)
            {
                ERR_EXIT("accept");
            }
            else
            {
                // update max fd number
                if(clientSocket > maxReadFd)
                {
                    maxReadFd = clientSocket;
                }

                FD_SET(clientSocket, &readFdSet);

                

                // set client username
                clientName = ReadString(clientSocket);
                clientSocketMap[clientName] = clientSocket; // add name and client to map
                clientSocketAddrMap[clientName] = clientAddr; // add client address infomation
                clientSocketOnline[clientName] = true; // client is online
                
                BroadCast("<User " + clientName + "is on‐line, IP address: " + inet_ntoa(clientAddr.sin_addr) + ".>");
                cout<<"Client ("<<clientName<<") Connect Success"<<endl;
            }
        }

        // find which readFd have something to read
        for(auto it = clientSocketOnline.begin(); it != clientSocketOnline.end(); it++)
        {
            // client not online
            if(!(it->second))
            {
                continue;   
            }

            int readFd = clientSocketMap[it->first];

            // client have something to be read
            if(FD_ISSET(readFd, &readFdSet))
            {
                ClientHandler(readFd);
            }
        }
    }

    return true;
}

void Server::SendString(string clientName, string message)
{
    if(clientSocketMap.find(clientName) != clientSocketMap.end())
    {
        // get target client file descriptor
        int targetClientSocket = clientSocketMap[clientName];

        // send message to target client
        send(targetClientSocket, message.c_str(), message.length(), 0);
        cout<<"Send : "<<message<<endl;
    }
}

void Server::BroadCast(string message)
{
    for(auto it = clientSocketMap.begin(); it != clientSocketMap.end(); it++)
    {
        int targetClientSocket = it->second;

        // send message to target client
        send(targetClientSocket, message.c_str(), message.length(), 0);

        cout<<it->first<<endl;
    }

    cout<<"BroadCast : "<<message<<endl;
}

string Server::ReadString(int targetClientSocket)
{
	char message[BUF_SIZE] = {};
    int nbytes;

    if((nbytes = recv(clientSocket, message, sizeof(message), 0)) < 0)
    {
        ERR_EXIT("recv");
    }

    if(nbytes == 0) // client close
    {
        //clientSocketOnline[]
        FD_CLR(targetClientSocket, &readFdSet);
    }

    cout<<"ReadString "<<" : "<<message<<" ("<<nbytes<<")"<<endl;

	return message;
}

void Server::ClientHandler(int targetClientSocket)
{
    string input = "";
    string command = "";
    string message = "";
    stringstream ss;

    input = ReadString(targetClientSocket);
    //SendString(clientName, "<SendString> " + command);
    BroadCast(input);
}