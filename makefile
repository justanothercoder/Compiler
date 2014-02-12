CC=clang++
CFLAGS=-std=c++11 -g -c -Wall -Wextra
LDFLAGS=
SOURCES=main.cpp token.cpp recognitionerror.cpp abstractlexer.cpp lexer.cpp abstractparser.cpp parser.cpp ast.cpp declarationnode.cpp structdeclarationnode.cpp scope.cpp structsymbol.cpp globalscope.cpp variabledeclarationnode.cpp variablesymbol.cpp codegen.cpp builtintypesymbol.cpp functiondeclarationnode.cpp functiontypeinfo.cpp functionsymbol.cpp statementnode.cpp emptystatementnode.cpp exprnode.cpp variablenode.cpp numbernode.cpp callnode.cpp functionhelper.cpp overloadedfunctiontypeinfo.cpp semanticerror.cpp referencetype.cpp typeinfo.cpp typehelper.cpp symbol.cpp overloadedfunctionsymbol.cpp returnnode.cpp ifnode.cpp basescope.cpp localscope.cpp whilenode.cpp dotnode.cpp binaryoperatornode.cpp globalhelper.cpp builtins.cpp functiontraits.cpp type.cpp newexpressionnode.cpp templatestructdeclarationnode.cpp templatestructsymbol.cpp classvariablesymbol.cpp variablesymboldefine.cpp functionsymboldefine.cpp symboldefine.cpp bracketnode.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=comp

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
