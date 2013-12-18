CC=clang++
CFLAGS=-std=c++11 -g -c -Wall -Wextra
LDFLAGS=
SOURCES=main.cpp token.cpp recognitionerror.cpp abstractlexer.cpp lexer.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=comp

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
