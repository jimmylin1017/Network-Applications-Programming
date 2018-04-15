#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

#include <map>
#include <queue>

#include <fstream> // ifstream(), ofstream()
#include <bitset>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> // getaddrinfo()
#include <arpa/inet.h> // inet_ntop(), inet_pton()
#include <ifaddrs.h> // getifaddrs()
#include <unistd.h>
#include <netinet/in.h>

#include <sys/stat.h> // stat()

using namespace std;

#endif /* HEADER_H */

#ifdef _DEBUG
    #define DEBUG(x) do { std::cerr<<x<<endl; } while (0)
#else
    #define DEBUG(x)
#endif