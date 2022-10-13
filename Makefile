CXX=g++
CXXFLAGS=-g -ggdb -O2 -Wall

TARGET = main

OBJS = main.o \
		lexical/SymbolTable.o \
		lexical/LexicalAnalysis.o \
		syntatic/SyntaticAnalysis.o

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJS)

install:
	cp $(TARGET) /usr/local/bin

main.o: lexical/LexicalAnalysis.h

lexical/SymbolTable.o: lexical/TokenType.h

lexical/LexicalAnalysis.o: lexical/LexicalAnalysis.h lexical/Lexeme.h lexical/SymbolTable.h

syntatic/SyntaticAnalysis.o:

$(TARGET):	$(OBJS)
			$(CXX) -o $(TARGET) $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
