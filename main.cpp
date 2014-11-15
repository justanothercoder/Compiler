#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "lexer.hpp"
#include "parser.hpp"
#include "builtins.hpp"
#include "filehelper.hpp"
#include "functionsymbol.hpp"

#include "definevisitor.hpp"
#include "isdefinedvisitor.hpp"
#include "checkvisitor.hpp"
#include "genssavisitor.hpp"
#include "expandtemplatesvisitor.hpp"

#include "statementnode.hpp"

using std::shared_ptr;

int main()
{
    std::string filename = "input.txt";

    try
    {
        BuiltIns::defineBuiltIns();

        shared_ptr<AST> root(FileHelper::parse(filename));

        root -> scope = BuiltIns::global_scope;

        root -> build_scope();
        
        ExpandTemplatesVisitor expand_visitor;
        root -> accept(expand_visitor);
       
        DefineVisitor define_visitor;
        root -> accept(define_visitor);

//        IsDefinedVisitor is_defined_visitor;
//        root -> accept(is_defined_visitor);

        CheckVisitor check_visitor;
        root -> accept(check_visitor);

        GenSSAVisitor visitor;
        root -> accept(visitor);
//		std::cerr << "code:\n" << visitor.getString() << '\n';

//        visitor.optimize();
		std::cerr << "optimized code:\n" << visitor.getString() << '\n';

//        std::cerr << "\nasm code:\n";

        CodeObject code_obj;
        visitor.getCode().genAsm(code_obj);

        code_obj.gen();
    }
//	catch ( SemanticError& e )
//	{
//		std::cerr << e.what() << '\n';
//		return 1;
//	}
//	catch ( RecognitionError& e )
//	{
//		std::cerr << e.what() << '\n';
//		return 2;
//	}
    catch ( int x )
    {
        return 1;
    }

    delete BuiltIns::global_scope;

    return 0;
}
