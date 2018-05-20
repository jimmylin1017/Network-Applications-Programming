#include "server.h"

#define SERVER_PORT 6666

int main(int argc, char *argv[])
{
    signal(SIGCHLD, SIG_IGN);

    Server server(SERVER_PORT);
    server.ServerCreate();

    pid_t pid;

    while(1)
    {
        if(server.ServerListen())
        {
            pid = fork();

            if(pid == -1)
            {
                ERR_EXIT("fork");
            }

            if(pid == 0) // child process
            {
                cout<<"In Child Process"<<endl;
                server.ClientHandler();
            }
            else // parent process
            {
                cout<<"In Parent Process"<<endl;
            }
        }
    }

    return 0;
}