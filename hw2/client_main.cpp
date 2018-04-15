#include "client.h"
#include "huffman_coding.h"

int main()
{
    string ip;
    int port;
    string message = "", command = "", fileName = "";
    int fileSize = 0;

    while(cin>>command)
    {
        if(command == "login")
        {
            cin>>ip>>port;
            break;
        }
    }

    Client client(ip, port);
    client.ClientCreate();
    client.ClientConnect();

    HuffmanCoding huffmanCoding;

    while(cin>>command)
    {
        if(command == "uploadf")
        {
            cin>>fileName;

            huffmanCoding.CompressByFixCode(fileName);
            fileSize = client.GetFileSizeInBytes(fileName + "-fix_compressed");
            client.WriteString(command + " " + fileName + "-fix_compressed" + " " + to_string(fileSize));
            message = client.ReadString();
            
            if(message == "uploadf_ack")
            {
                client.SendFile(fileName + "-fix_compressed");
            }

            fileSize = client.GetFileSizeInBytes(fileName + "-fix_code");
            client.WriteString(command + " " + fileName + "-fix_code" + " " + to_string(fileSize));
            message = client.ReadString();
            
            if(message == "uploadf_ack")
            {
                client.SendFile(fileName + "-fix_code");
            }
        }

        if(command == "upload")
        {
            cin>>fileName;
            
            huffmanCoding.CompressByCode(fileName);
            fileSize = client.GetFileSizeInBytes(fileName + "-compressed");
            client.WriteString(command + " " + fileName + "-compressed" + " " + to_string(fileSize));
            message = client.ReadString();
            
            if(message == "upload_ack")
            {
                client.SendFile(fileName + "-compressed");
            }

            fileSize = client.GetFileSizeInBytes(fileName + "-code");
            client.WriteString(command + " " + fileName + "-code" + " " + to_string(fileSize));
            message = client.ReadString();
            
            if(message == "upload_ack")
            {
                client.SendFile(fileName + "-code");
            }
        }

        if(command == "logout")
        {
            client.WriteString("close");
            break;
        }
    }

    return 0;
}