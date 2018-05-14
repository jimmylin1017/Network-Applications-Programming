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
    close(clientSocket);
	cout<<"Listening......"<<endl;

	// listen 5 client
	if(listen(serverSocket, 5) < 0)
    {
        ERR_EXIT("listen");
    }

	// receive client information
    socklen_t clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

    if(clientSocket < 0)
    {
        ERR_EXIT("accept");
    }

    cout<<"Client Connect Success"<<endl;

    return true;
}

void Server::SendString(string clientName, string message)
{
    if((*clientSocketMap).find(clientName) != (*clientSocketMap).end())
    {
        // get target client file descriptor
        int targetClientSocket = (*clientSocketMap)[clientName];

        // send message to target client
        send(targetClientSocket, message.c_str(), message.length(), 0);
        cout<<"Send : "<<message<<endl;
    }
}

void Server::BroadCast(string message)
{
    for(auto it = (*clientSocketMap).begin(); it != (*clientSocketMap).end(); it++)
    {
        int targetClientSocket = it->second;

        // send message to target client
        send(targetClientSocket, message.c_str(), message.length(), 0);

        cout<<it->first<<endl;
    }

    cout<<"BroadCast : "<<message<<endl;
}

string Server::ReadString()
{
	char message[BUF_SIZE] = {};
    int nbytes;

    if((nbytes = recv(clientSocket, message, sizeof(message), 0)) < 0)
    {
        ERR_EXIT("recv");
    }

    if(nbytes == 0) // client close
    {
        exit(EXIT_SUCCESS);
    }

    cout<<"ReadString "<<" : "<<message<<" ("<<nbytes<<")"<<endl;

	return message;
}

void Server::ClientHandler()
{
    close(serverSocket);

    clientName = ReadString();
    (*clientSocketMap)[clientName] = clientSocket; // add name and client to map

    BroadCast("<User " + clientName + "is on‐line, IP address: " + inet_ntoa(clientAddr.sin_addr) + ".>");

    string input = "";
    string command = "";
    string message = "";
    stringstream ss;

    while(1)
    {
        command = ReadString();
        SendString(clientName, "<SendString> " + command);
        BroadCast("<" + clientName + "> " + command);
    }

    /*while(true)
    {
        message = server.ReadString();
        ss.str(message);
        ss>>command>>fileName>>fileSize;

        if(command == "upload")
        {
            cout<<command<<endl;
            cout<<fileName<<endl;
            cout<<fileSize<<endl;

            server.WriteString("upload_ack");
            server.ReceiveFile(fileName, fileSize);
        }

        // clear stringstream
        ss.str("");
        ss.clear();
    }*/
}