#include "client.h"

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
    cin>>clientName;
    client.SendString(clientName);

    client.ClientStart();

    return 0;
}