CC=clang++
CFLAGS=-std=c++14 -g -c -Wall -Wextra -pedantic -O0
LDFLAGS=
SOURCES=main.cpp token.cpp abstractlexer.cpp lexer.cpp abstractparser.cpp parser.cpp ast.cpp declarationnode.cpp structdeclarationnode.cpp \
	   	scope.cpp structsymbol.cpp globalscope.cpp variabledeclarationnode.cpp variablesymbol.cpp codeobject.cpp builtintypesymbol.cpp \
	   	functiondeclarationnode.cpp functiontypeinfo.cpp functionsymbol.cpp statementnode.cpp variablenode.cpp numbernode.cpp callnode.cpp \
	   	semanticerror.cpp typeinfo.cpp symbol.cpp returnnode.cpp ifnode.cpp definetypesvisitor.cpp lambdanode.cpp paraminfo.cpp \
	   	basescope.cpp localscope.cpp whilenode.cpp dotnode.cpp binaryoperatornode.cpp builtins.cpp functiontraits.cpp newexpressionnode.cpp \
	   	templatestructdeclarationnode.cpp bracketnode.cpp fornode.cpp conversioninfo.cpp callinfo.cpp templateinfo.cpp stringnode.cpp \
	   	unarynode.cpp filehelper.cpp importnode.cpp filenotfoundexception.cpp logger.cpp varinfertypedeclarationnode.cpp structscope.cpp \
	   	unsafeblocknode.cpp unsafescope.cpp referencetype.cpp pointertype.cpp typefactory.cpp type.cpp functionscope.cpp addrnode.cpp \
	   	noviableoverloaderror.cpp nullnode.cpp genssavisitor.cpp checkvisitor.cpp threeaddresscode.cpp optimizer.cpp block.cpp \
		definevisitor.cpp compiler.cpp globalconfig.cpp expandtemplatesvisitor.cpp arraytype.cpp functiontype.cpp externnode.cpp \
		modulenode.cpp functionnode.cpp modulesymbol.cpp modulememberaccessnode.cpp comp.cpp compilableunit.cpp varinfo.cpp \
	   	recognitionerror.cpp breaknode.cpp variabletype.cpp iffalsecommand.cpp newcommand.cpp gotocommand.cpp labelcommand.cpp \
	   	elemcommand.cpp paramcommand.cpp callcommand.cpp numberarg.cpp assigncommand.cpp binaryopcommand.cpp variablearg.cpp unaryopcommand.cpp \
	   	stringarg.cpp returncommand.cpp labelarg.cpp temporaryarg.cpp spaceallocator.cpp substituteargvisitor.cpp checkforusevisitor.cpp \
	   	functiondeclarationinfo.cpp disposememoryvisitor.cpp inlinecallvisitor.cpp markreturnasinlinevisitor.cpp assignrefcommand.cpp dotarg.cpp \
	   	inlineinfo.cpp templatefunctiondeclarationnode.cpp templatefunctionnode.cpp templatefunctionsymbol.cpp templatestructsymbol.cpp \
		functionaltype.cpp valueinfo.cpp objecttype.cpp typemodifier.cpp callablenode.cpp exprnode.cpp deducereturntypevisitor.cpp \
	   	nodewithcall.cpp rebuildtreevisitor.cpp templatesymbol.cpp templateargument.cpp typesymbol.cpp symbolentry.cpp symbolfactory.cpp \
	   	functionalsymbol.cpp aliassymbol.cpp symboltable.cpp templatespecializationerror.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=comp

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
