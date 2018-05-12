#ifndef HEADER_H

#define HEADER_H

#include <iostream>
#include <cstring>
#include <string>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> // getaddrinfo()
#include <arpa/inet.h> // inet_ntop(), inet_pton()
#include <ifaddrs.h> // getifaddrs()
#include <unistd.h>
#include <netinet/in.h>

#include <sys/stat.h> // stat()
#include <fstream>

using namespace std;

#endif /* HEADER_H */

#ifdef _DEBUG
    #define DEBUG(x) do { std::cerr<<x<<endl; } while (0)
#else
    #define DEBUG(x)
#endif