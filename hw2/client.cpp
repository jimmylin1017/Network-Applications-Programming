#include "client.h"

bool Client::ClientCreate()
{
    // create socket fd
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	// socket fd failed
	if (serverSocket == -1)
    {
		perror("socket fd failed");
		return false;
	}

	cout<<"Client Create Success"<<endl;

	return true;
}

bool Client::ClientConnect()
{
    // connect to server
    memset(&serverAddr, 0, sizeof(serverAddr)); // full of zero
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddr.sin_port = htons(serverPort);

    if(connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {

    	perror("connect failed");
		return false;
    }

    cout<<"The server with IP "<<serverIP<<":"<<serverPort<<" has accepted your connection."<<endl;

    return true;
}

void Client::WriteString(string message) {

	send(serverSocket, message.c_str(), message.length(), 0);

	cout<<"Send: "<<message<<endl;
}

string Client::ReadString() {

	char message[BUF_SIZE] = {};

	if(recv(serverSocket, message, sizeof(message), 0) < 0)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    cout<<"ReadString : "<<message<<endl;

	return message;
}

int Client::GetFileSizeInBytes(string fileName)
{
    struct stat st;
    if(stat(fileName.c_str(), &st) != 0)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    return st.st_size;
}

bool Client::SendFile(string fileName)
{
    ifstream fin;
    fin.open(fileName, ios::binary);

    if(!fin)
    { 
        perror("ifstream open");
        exit(EXIT_FAILURE);
    }

    // because >> will skip space, need to cancel it
    fin.unsetf(ios::skipws);

    char fileData[BUF_SIZE];

    while(fin.peek() != EOF)
    {
        fin.read(fileData, sizeof(fileData));
        send(serverSocket, fileData, sizeof(fileData), 0);
    }

    fin.close();

    if(ReadString() == "finish")
    {
        return true;
    }
    else
    {
        return false;
    }
}