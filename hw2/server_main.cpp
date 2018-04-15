#include "server.h"
#include "huffman_coding.h"

#define SERVER_PORT 6666

int main(int argc, char *argv[])
{
    Server server(SERVER_PORT);
    server.ServerCreate();
    server.ServerListen();

    string message = "", command = "", fileName = "";
    int fileSize = 0;
    stringstream ss;

    int uploadfCounter = 0;
    int uploadCounter = 0;

    HuffmanCoding huffmanCoding;

    while(true)
    {
        message = server.ReadString();
        ss.str(message);
        ss>>command>>fileName>>fileSize;

        if(command == "uploadf")
        {
            server.WriteString("uploadf_ack");
            server.ReceiveFile(fileName, fileSize);
            uploadfCounter++;

            if(uploadfCounter == 2)
            {
                uploadfCounter = 0;

                string s = "";
                for(int i=0; i<(int)fileName.length(); i++)
                {
                    if(fileName[i] == '-') break;
                    s += fileName[i];
                }

                cout<<"Reveive File : "<<s<<endl;
                huffmanCoding.DecompressByFixCode(s);
                cout<<"Decompress File "<<s<<" Finish"<<endl;
            }
        }

        if(command == "upload")
        {
            server.WriteString("upload_ack");
            server.ReceiveFile(fileName, fileSize);
            uploadCounter++;

            if(uploadCounter == 2)
            {
                uploadCounter = 0;

                string s = "";
                for(int i=0; i<(int)fileName.length(); i++)
                {
                    if(fileName[i] == '-') break;
                    s += fileName[i];
                }

                cout<<"Reveive File : "<<s<<endl;
                huffmanCoding.DecompressByCode(s);
                cout<<"Decompress File "<<s<<" Finish"<<endl;
            }
        }

        if(command == "close")
        {
            server.ClinetClose();
            break;
        }

        // clear stringstream
        ss.str("");
        ss.clear();
    }

    return 0;
}