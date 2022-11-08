CC=g++
SRCPATH=./src/
LIB=-Wl,-rpath,/usr/local/lib /usr/local/lib/libgf0.so.1.0.0 /usr/local/lib/libgfnet0.so.1.0.0 /usr/local/lib/libgfcore0.so.1.0.0 /usr/lib/libz.so
SRC=$(wildcard $(SRCPATH)*.cpp)
OBJ=$(SRC:.cpp=.o)
EXEC=main

all: $(EXEC)

main: $(OBJ)
	$(CC) $(EXEC).cpp -o $(EXEC) $(notdir $^) $(LIB)
	rm *.o

%.o: %.cpp
	$(CC) -c $<

clean:
	rm main; rm *.o
