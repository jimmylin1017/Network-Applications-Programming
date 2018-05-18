#include "server.h"

/*
Server
*/

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

	// listen client
	if(listen(serverSocket, CLIENT_LIMIT) < 0)
    {
        ERR_EXIT("listen");
    }

    while(1)
    {

        socklen_t clientAddrSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

        if(clientSocket < 0)
        {
            ERR_EXIT("accept");
        }

        ClientHandler clientHandler(clientSocket, clientAddr);
        thread(ClientHandlerThread, clientHandler).detach();
    }

    return true;
}

void Server::ClientHandlerThread(ClientHandler clientHandler)
{
    while(1)
    {
        string command = clientHandler.ReadMessage();

        if(clientHandler.ExecuteCommand(command)) continue;

        break;
    }
}

/*
ClientHandler
*/

map<string, int>  ClientHandler::clientSocketMap;
map<string, sockaddr_in> ClientHandler::clientSocketAddrMap;
map<string, bool> ClientHandler::clientSocketOnline;
map<string, vector<string>> ClientHandler::clientSocketOffLineMessage;

bool ClientHandler::ExecuteCommand(string command)
{
    if(command == "bye")
    {
        string message = "<User " + clientName + " is off-line.>";
        BroadCastMessage(message);
        return false;
    }

    stringstream ss(command);

    string operation = "";

    ss>>operation;

    if(operation == "clientname")
    {
        cout<<"in clientname"<<endl;

        string name = "";
        ss>>name;

        // find client name in map
        if(clientSocketOnline.find(name) != clientSocketOnline.end())
        {
            // check client online or not
            if(!clientSocketOnline[name])
            {
                clientName = name;
                clientSocketMap[name] = clientSocket;
                clientSocketAddrMap[name] = clientAddr;
                clientSocketOnline[name] = true;

                string message = "<User " + clientName + " is on-line, IP address: " + inet_ntoa(clientAddr.sin_addr) + ".>";
                BroadCastMessage(message);

                if(!clientSocketOffLineMessage[name].empty())
                {
                    for(int i = 0; i < (int)clientSocketOffLineMessage[name].size(); i++)
                    {
                        sleep(0.5);
                        SendMessage(clientSocket, clientSocketOffLineMessage[name][i]);
                    }

                    clientSocketOffLineMessage[name].clear();
                }
            }
            else
            {
                string str = "<" + name + "> can not be used!";
                SendMessage(clientSocket, str);
            }
        }
        else
        {
            clientName = name;
            clientSocketMap[name] = clientSocket;
            clientSocketAddrMap[name] = clientAddr;
            clientSocketOnline[name] = true;
            clientSocketOffLineMessage[name].clear();

            string message = "<User " + clientName + " is on-line, IP address: " + inet_ntoa(clientAddr.sin_addr) + ".>";
            BroadCastMessage(message);
        }
    }
    else if(operation == "chat")
    {
        string target = "", message = "";
        ss>>target>>message;

        // check client exist
        if(clientSocketOnline.find(target) != clientSocketOnline.end())
        {
            // check client online
            if(clientSocketOnline[target])
            {
                message = "@" + clientName + " : " + message;
                SendMessage(clientSocketMap[target], message);
            }
            else
            {
                string str = "<" + target + "> is not online!";
                SendMessage(clientSocket, str);

                message = "@" + clientName + " : " + message;
                clientSocketOffLineMessage[target].push_back(message);
            }
        }
        else
        {
            string str = "<" + target + "> is not exist!";
            SendMessage(clientSocket, str);
        }
    }
    else if(operation == "chatall")
    {
        string message = "";
        ss>>message;

        message = "@" + clientName + " : " + message;
        BroadCastMessage(message);
    }

    return true;
}

void ClientHandler::BroadCastMessage(string message)
{
    for(auto it = clientSocketMap.begin(); it != clientSocketMap.end(); it++)
    {
        string name = it->first;
        int targetClientSocket = 0;
        // client is online
        if(clientSocketOnline[name])
        {
            cout<<name<<endl;

            targetClientSocket = it->second;
            // send message to target client
            send(targetClientSocket, message.c_str(), message.length(), 0);
        }
    }

    cout<<"[BroadCast] "<<message<<endl;
}

string ClientHandler::ReadMessage()
{
	char message[BUF_SIZE] = {};
    int nbytes;

    if((nbytes = recv(clientSocket, message, sizeof(message), 0)) < 0)
    {
        ERR_EXIT("recv");
    }

    if(nbytes == 0) // client close
    {
        clientSocketOnline[clientName] = false;
        cout<<"Client " + clientName + " Disconnect"<<endl;
        return "bye";
    }

    cout<<"[ReadMessage] "<<message<<" ("<<nbytes<<")"<<endl;

	return message;
}

void ClientHandler::SendMessage(int targetClientSocket, string message)
{
    // send message to target client
    send(targetClientSocket, message.c_str(), message.length(), 0);
    cout<<"[SendMessage] "<<message<<endl;
}