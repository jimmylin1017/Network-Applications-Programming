#include "client.h"

int main()
{
    string ip;
    int port;
    string message = "", command = "", fileName = "";
    int fileSize = 0;

    cin>>ip>>port;

    Client client(ip, port);
    client.ClientCreate();
    client.ClientConnect();

    while(cin>>command>>fileName)
    {
        if(command == "upload")
        {
            fileSize = client.GetFileSizeInBytes(fileName);
            client.WriteString(command + " " + fileName + " " + to_string(fileSize));
            message = client.ReadString();
            
            if(message == "upload_ack")
            {
                client.SendFile(fileName);
            }
        }
    }

    return 0;
}