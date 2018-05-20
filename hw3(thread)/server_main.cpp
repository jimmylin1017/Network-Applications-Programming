#include "server.h"

#define SERVER_PORT 6666

int main(int argc, char *argv[])
{
    Server server(SERVER_PORT);
    server.ServerCreate();

    server.ServerListen();

    return 0;
}