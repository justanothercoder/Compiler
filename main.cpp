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
#include "checkvisitor.hpp"
#include "genssavisitor.hpp"
#include "expandtemplatesvisitor.hpp"
#include "definetypesvisitor.hpp"
#include "comp.hpp"
#include "optimizer.hpp"
#include "inlinecallvisitor.hpp"

#include "globalconfig.hpp"
#include "codeobject.hpp"

#include "logger.hpp"

int main(int argc, char** argv)
{
    std::string filename = "input.txt";

    for ( int i = 1; i < argc; ++i )
    {
        if ( argv[i][0] == '-' )
        {
            auto flag = std::string(argv[i] + 1);
            auto opt_flag = Comp::config().flagValue(flag);

            if ( opt_flag == boost::none )
               throw std::logic_error("Unknown option '" + flag + "'"); 

            Comp::config().setFlag(flag);
        }
        else
            filename = argv[i];
    }

    try
    {
        BuiltIns::defineBuiltIns();

        auto root = FileHelper::parse(filename);

        root -> scope = BuiltIns::global_scope;
        root -> build_scope();
       
        DefineTypesVisitor define_types_visitor;
        root -> accept(define_types_visitor);

        ExpandTemplatesVisitor expand_visitor;
        root -> accept(expand_visitor);

        DefineVisitor define_visitor;
        root -> accept(define_visitor);

        CheckVisitor check_visitor;
        root -> accept(check_visitor);

        if ( !*Comp::config().flagValue("fnoinline") )            
        {
            InlineCallVisitor inline_call_visitor;
            root -> accept(inline_call_visitor);
        }

        GenSSAVisitor visitor(Comp::code);
        root -> accept(visitor);

        if ( *Comp::config().flagValue("fdumpTAC") )
            Logger::log("TAC: \n" + Comp::code.toString());

        if ( *Comp::config().flagValue("optimize") )
        {
            Optimizer optimizer(Comp::code);
            optimizer.optimize();
        }
        
        if ( *Comp::config().flagValue("fdumpTAC") )
            Logger::log("Optimized TAC: \n" + Comp::code.toString());

        CodeObject code_obj;
        (Comp::code).genAsm(code_obj);

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

    return 0;
}
