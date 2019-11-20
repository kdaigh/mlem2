CPP_SRCS := $(wildcard *.cpp) 
OBJ_SRCS := $(CPP_SRCS:.cpp=.o)

mlem2: $(OBJ_SRCS)
	g++ -std=c++17 -g -Wall -Werror -o $@ $(OBJ_SRCS)

%.o: %.cpp 
	g++ -std=c++17 -g -Wall -c -o $@ $<

clean:
	rm -rf *.o *.d mlem2
