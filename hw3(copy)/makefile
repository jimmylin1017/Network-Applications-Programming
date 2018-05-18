SERVER_SRCS = server.cpp server_main.cpp
CLIENT_SRCS = client.cpp client_main.cpp
EXT  = .cpp
SERVER_OBJS = $(SERVER_SRCS:$(EXT)=.o)
CLIENT_OBJS = $(CLIENT_SRCS:$(EXT)=.o)
SERVER_EXEC = server
CLIENT_EXEC = client
CXX  = g++
CXXFLAGS = -std=c++11 -pthread -Wall -g -D _DEBUG
#CXXFLAGS = -std=c++11 -Wall -g

all:$(SERVER_EXEC) $(CLIENT_EXEC) clean

$(SERVER_EXEC):$(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_EXEC) $(SERVER_OBJS)

$(CLIENT_EXEC):$(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_EXEC) $(CLIENT_OBJS)

%.o:%$(EXT)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(SERVER_OBJS) $(CLIENT_OBJS)