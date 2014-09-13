CC=clang++
CFLAGS=-std=c++11 -g -c -Wall -Wextra -pedantic
LDFLAGS=
SOURCES=main.cpp token.cpp abstractlexer.cpp lexer.cpp abstractparser.cpp parser.cpp ast.cpp declarationnode.cpp structdeclarationnode.cpp scope.cpp structsymbol.cpp globalscope.cpp variabledeclarationnode.cpp variablesymbol.cpp codeobject.cpp builtintypesymbol.cpp functiondeclarationnode.cpp functiontypeinfo.cpp functionsymbol.cpp statementnode.cpp exprnode.cpp variablenode.cpp numbernode.cpp callnode.cpp functionhelper.cpp overloadedfunctiontypeinfo.cpp semanticerror.cpp typeinfo.cpp symbol.cpp overloadedfunctionsymbol.cpp returnnode.cpp ifnode.cpp basescope.cpp localscope.cpp whilenode.cpp dotnode.cpp binaryoperatornode.cpp globalhelper.cpp builtins.cpp functiontraits.cpp newexpressionnode.cpp templatestructdeclarationnode.cpp templatestructsymbol.cpp variablesymboldefine.cpp functionsymboldefine.cpp symboldefine.cpp bracketnode.cpp callhelper.cpp asmarraynode.cpp fornode.cpp varallocator.cpp paramvarallocator.cpp fieldvarallocator.cpp conversioninfo.cpp callinfo.cpp templateinfo.cpp stringnode.cpp unarynode.cpp filehelper.cpp importnode.cpp filenotfoundexception.cpp logger.cpp tempallocator.cpp varinfertypedeclarationnode.cpp structscope.cpp unsafeblocknode.cpp unsafescope.cpp referencetype.cpp pointertype.cpp typefactory.cpp consttype.cpp type.cpp functionscope.cpp addrnode.cpp noviableoverloaderror.cpp nullnode.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=comp

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
