CPP_SRCS := $(wildcard *.cpp) 
OBJ_SRCS := $(CPP_SRCS:.cpp=.o)

mlem2: $(OBJ_SRCS)
	g++ -std=c++14 -g -Wall -Werror -o $@ $(OBJ_SRCS)

%.o: %.cpp 
	g++ -std=c++14 -g -Wall -Werror -c -o $@ $<

clean:
	rm -rf *.o *.d mlem2
