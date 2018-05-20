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

    message = client.ReadString();
    cout<<message<<endl;

    while(cin>>command)
    {
        /*if(command == "upload")
        {
            fileSize = client.GetFileSizeInBytes(fileName);
            client.WriteString(command + " " + fileName + " " + to_string(fileSize));
            message = client.ReadString();
            
            if(message == "upload_ack")
            {
                client.SendFile(fileName);
            }
        }*/

        client.SendString(command);

        message = client.ReadString();
        cout<<"receive : "<<message<<endl;
    }

    return 0;
}