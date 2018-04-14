#include "server.h"

void Server::GetHostInfo()
{
    cout<<"GetHostInfo"<<endl;

    if(getifaddrs(&hostAddrInfo) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    
    // local ip, for follow for loop to get real ip
    struct sockaddr_in local;
    inet_pton(AF_INET, "127.0.0.1", &(local.sin_addr));

    for(auto ifa = hostAddrInfo; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_addr == nullptr)
            continue;

        if(ifa->ifa_addr->sa_family == AF_INET)
        {
            hostAddr = (struct sockaddr_in *)ifa->ifa_addr;

            // check local ip, if true then continue to get new one
            if((hostAddr->sin_addr).s_addr == local.sin_addr.s_addr)
            {
                continue;
            }
            
            if (!inet_ntop(ifa->ifa_addr->sa_family, &(hostAddr->sin_addr), hostIP4, sizeof(hostIP4)))
            {
                perror("inet_ntop");
            }
            else
            {
                cout<<hostIP4<<endl;
                break;
            }
        }
    }
}

bool Server::ServerCreate()
{
    // create server socket fd
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	// socket fd failed
	if (serverSocket == -1) {

		perror("socket fd failed");
		return false;
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
        perror("socket bind failed");
        return false;
    }

    cout<<"Server Create Successful With Port "<<serverPort<<endl;

    return true;
}


void Server::ServerListen() {

	cout<<"Listening..."<<endl;

	// listen 1 client
	if(listen(serverSocket, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

	// receive client information
    socklen_t clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

    if(clientSocket < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    cout<<inet_ntoa(clientAddr.sin_addr)<<endl;

    cout<<ntohs(clientAddr.sin_port)<<endl;


    cout<<"Client Connect Success"<<endl;
}

void Server::WriteString(string message)
{
	send(clientSocket, message.c_str(), message.length(), 0);

	cout<<"Send: "<<message<<endl;
}

string Server::ReadString()
{
	char message[BUF_SIZE] = {};
    int nbytes;

    if((nbytes = recv(clientSocket, message, sizeof(message), 0)) < 0)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    if(nbytes == 0)
    {
        exit(EXIT_SUCCESS);
    }

    cout<<"ReadString "<<nbytes<<" : "<<message<<endl;
	return message;
}

void Server::ReceiveFile(string fileName, int fileSize)
{
	char fileData[BUF_SIZE] = {};
    int remainData = fileSize;
    int nbytes = 0;

    ofstream fout;
    fout.open("receive_" + fileName, ios::binary);

    while((remainData > 0) && ((nbytes = recv(clientSocket, fileData, sizeof(fileData), 0)) > 0))
    {
        if(nbytes > remainData)
            fout.write(fileData, remainData);
        else
            fout.write(fileData, nbytes);
        remainData -= nbytes;
        cout<<"remainData : "<<remainData<<endl;
    }

    cout<<"File Store Success"<<endl;

    WriteString("finish");

    fout.close();
}