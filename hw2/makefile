SRCS = huffman_coding.cpp main.cpp
EXT  = .cpp
OBJS = $(SRCS:$(EXT)=.o)
EXEC = main
CXX  = g++
CXXFLAGS = -std=c++11 -Wall -g -D _DEBUG
#CXXFLAGS = -std=c++11 -Wall -g

all:$(EXEC) clean

$(EXEC):$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

%.o:%$(EXT)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(OBJS)