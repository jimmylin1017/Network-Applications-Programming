#include "client.h"

void display(Client client)
{
    string message = "";

    while(1)
    {
        message = client.ReadString();

        cout<<"[Display] "<<message<<endl;

        if(message == "server shutdown")
        {
            exit(1);
        }
    }
}

int main()
{
    string ip;
    int port;
    string message = "", command = "";

    while(cin>>command)
    {
        if(command == "connect")
        {
            cin>>ip>>port;
            break;
        }
    }

    Client client(ip, port);
    client.ClientCreate();
    client.ClientConnect();

    // setting clinet name
    string clientName;

    while(cin>>clientName)
    {
        string str = "<" + clientName + "> can not be used!";
        client.SendString("clientname " + clientName);

        if(client.ReadString() != str) break;
    }

    getchar();
    
    thread(display, client).detach();

    while(getline(cin, message))
    {
        client.SendString(message);
    }

    return 0;
}