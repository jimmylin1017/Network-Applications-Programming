#include "server.h"

#define SERVER_PORT 6666

int main(int argc, char *argv[])
{
    Server server(SERVER_PORT);
    server.ServerCreate();
    server.ServerListen();

    string message = "", command = "", fileName = "";
    int fileSize = 0;
    stringstream ss;

    while(true)
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
    }

    return 0;
}