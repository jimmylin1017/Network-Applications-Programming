SERVER_SRCS = huffman_coding.cpp server.cpp server_main.cpp
CLIENT_SRCS = huffman_coding.cpp client.cpp client_main.cpp
EXT  = .cpp
SERVER_OBJS = $(SERVER_SRCS:$(EXT)=.o)
CLIENT_OBJS = $(CLIENT_SRCS:$(EXT)=.o)
SERVER_EXEC = server
CLIENT_EXEC = client
CXX  = g++
#CXXFLAGS = -std=c++11 -Wall -g -D _DEBUG
CXXFLAGS = -std=c++11 -Wall -g

all:$(SERVER_EXEC) $(CLIENT_EXEC) clean

$(SERVER_EXEC):$(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_EXEC) $(SERVER_OBJS)

$(CLIENT_EXEC):$(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_EXEC) $(CLIENT_OBJS)

%.o:%$(EXT)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(SERVER_OBJS) $(CLIENT_OBJS)

cleanexe:
	rm -rf $(SERVER_EXEC) $(CLIENT_EXEC)

cleantest:
	rm -rf receive_test.mp3-fix_code receive_test.mp3-fix_compressed receive_testfile-fix_code receive_testfile-fix_compressed test.mp3-fix_code test.mp3-fix_compressed receive_test.mp3-origin testfile-fix_code testfile-fix_compressed receive_testfile-origin
	rm -rf receive_test.mp3-code receive_test.mp3-compressed receive_testfile-code receive_testfile-compressed test.mp3-code test.mp3-compressed receive_test.mp3-origin testfile-code testfile-compressed receive_testfile-origin

difftestf:
	diff test.mp3 receive_test.mp3-origin
	diff test.mp3-fix_code receive_test.mp3-fix_code
	diff test.mp3-fix_compressed receive_test.mp3-fix_compressed

difftest:
	diff test.mp3 receive_test.mp3-origin
	diff test.mp3-code receive_test.mp3-code
	diff test.mp3-compressed receive_test.mp3-compressed

