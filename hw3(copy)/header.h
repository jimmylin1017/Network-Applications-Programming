#ifndef HEADER_H

#define HEADER_H

#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <map>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netdb.h> // getaddrinfo()
#include <arpa/inet.h> // inet_ntop(), inet_pton()
#include <ifaddrs.h> // getifaddrs()
#include <unistd.h>
#include <netinet/in.h>

#include <sys/stat.h> // stat()
#include <signal.h> // signal()
#include <sys/mman.h> // mmap()


using namespace std;

#endif /* HEADER_H */

#ifndef ERR_EXIT

#define ERR_EXIT(m) \
    do { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while (0)

#endif /* ERR_EXIT */

#ifdef _DEBUG
    #define DEBUG(x) do { std::cerr<<x<<endl; } while (0)
#else
    #define DEBUG(x)
#endif