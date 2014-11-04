#include "checkvisitor.hpp"
#include "callhelper.hpp"
#include "ifnode.hpp"
#include "whilenode.hpp"
#include "fornode.hpp"
#include "bracketnode.hpp"
#include "unarynode.hpp"
#include "structsymbol.hpp"
#include "newexpressionnode.hpp"
#include "binaryoperatornode.hpp"
#include "structdeclarationnode.hpp"
#include "functiondeclarationnode.hpp"
#include "functionsymbol.hpp"
#include "variabledeclarationnode.hpp"
#include "addrnode.hpp"
#include "dotnode.hpp"
#include "statementnode.hpp"
#include "variablenode.hpp"
#include "templatestructsymbol.hpp"
#include "numbernode.hpp"
#include "callnode.hpp"
#include "returnnode.hpp"
#include "unsafeblocknode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "nullnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "builtins.hpp"

void CheckVisitor::visit(ImportNode *)
{

}

void CheckVisitor::visit(IfNode *node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void CheckVisitor::visit(ForNode *node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void CheckVisitor::visit(WhileNode *node)
{
    for ( auto child : node -> getChildren() )
        child -> accept(*this);
}

void CheckVisitor::visit(BracketNode *node)
{
    node -> base -> accept(*this);
    node -> expr -> accept(*this);

    auto base_type = dynamic_cast<const StructSymbol*>(node -> base -> getType() -> getSymbol());    

    if ( node -> base -> getType() -> getTypeKind() == TypeKind::ARRAY )
        node -> call_info = CallHelper::callCheck("operator[]", BuiltIns::global_scope, {node -> base, node -> expr});
    else
        node -> call_info = CallHelper::callCheck("operator[]", base_type, {node -> expr});

    node -> scope -> getTempAlloc().add(node -> getType() -> getSize());
}

void CheckVisitor::visit(UnaryNode *node)
{
    node -> exp -> accept(*this);

    node -> call_info = CallHelper::callCheck(node -> getOperatorName(), static_cast<const StructSymbol*>(node -> exp -> getType() -> getSymbol()), { });

    node -> scope -> getTempAlloc().add(node -> getType() -> getSize());
}

void CheckVisitor::visit(NewExpressionNode *node)
{
    for ( auto param : node -> type_info.template_params )
    {
        if ( param.which() == 0 )
            boost::get<ExprNode*>(param) -> accept(*this);
    }

    auto type = static_cast<const StructSymbol*>(fromTypeInfo(node -> type_info, node -> scope));

    for ( auto param : node -> params )
        param -> accept(*this);        

    node -> call_info = CallHelper::callCheck(type -> getName(), type, node -> params);

    node -> scope -> getTempAlloc().add(type -> getSize());      //place for object itself
    node -> scope -> getTempAlloc().add(GlobalConfig::int_size); //place for reference to it
}

void CheckVisitor::visit(BinaryOperatorNode *node)
{
    node -> lhs -> accept(*this);
    node -> rhs -> accept(*this);
    try
    {
        if ( node -> lhs -> getType() -> getUnqualifiedType() -> getTypeKind() == TypeKind::STRUCT )
            node -> call_info = CallHelper::callCheck(node -> getOperatorName(), static_cast<const StructSymbol*>(node -> lhs -> getType() -> getSymbol()), {node -> rhs});
        else
            throw SemanticError("");
    }
    catch ( SemanticError& e )
    {
        node -> call_info = CallHelper::callCheck(node -> getOperatorName(), node -> scope, {node -> lhs, node -> rhs});
    }

    node -> scope -> getTempAlloc().add(node -> getType() -> getSize());
}

void CheckVisitor::visit(StructDeclarationNode *node)
{
    node -> getDefinedSymbol() -> is_defined = true;
    /*
    	if ( node -> definedSymbol -> getDefaultConstructor() == nullptr )
    	{
    		auto default_constr = FunctionHelper::makeDefaultConstructor(node -> definedSymbol);
    		node -> definedSymbol -> define(default_constr);
    	}

    	if ( node -> definedSymbol -> getCopyConstructor() == nullptr )
    	{
    		auto copy_constr = FunctionHelper::makeDefaultCopyConstructor(node -> definedSymbol);
    		node -> definedSymbol -> define(copy_constr);
    	}
    */
    for ( auto decl : node -> inner )
    {
        if ( dynamic_cast<DeclarationNode*>(decl) )
            static_cast<DeclarationNode*>(decl) -> getDefinedSymbol() -> is_defined = true;

        decl -> accept(*this);
    }
}

void CheckVisitor::visit(FunctionDeclarationNode *node)
{
    node -> scope -> resolve(node -> name) -> is_defined = true;

    for ( auto param : node -> params_symbols )
        param -> is_defined = true;

    node -> statements -> accept(*this);
}

void CheckVisitor::visit(VariableDeclarationNode *node)
{
    for ( auto param : node -> type_info.template_params )
    {
        if ( param.which() == 0 )
            boost::get<ExprNode*>(param) -> accept(*this);
    }

    if ( !node -> is_field )
    {
        if ( !node -> type_info.is_ref )
        {
//			std::string type_name = node -> type_info.type_name;

            auto var_type = fromTypeInfo(node -> type_info, node -> scope);

//			if ( var_type -> getSymbol() == nullptr || var_type -> getSymbol() -> getSymbolType() != SymbolType::STRUCT )
//				throw SemanticError("No such struct '" + type_name + "'");

            if ( var_type -> getTypeKind() != TypeKind::POINTER )
            {
                auto struct_symbol = static_cast<const StructSymbol*>(var_type -> getSymbol());

                for ( auto param : node -> constructor_call_params )
                    param -> accept(*this);

                node -> call_info = CallHelper::callCheck(struct_symbol -> getName(), struct_symbol, node -> constructor_call_params);
            }
            else
            {
                for ( auto param : node -> constructor_call_params )
                    param -> accept(*this);
            }
        }
        else
        {
            for ( auto param : node -> constructor_call_params )
                param -> accept(*this);
        }
    }

    node -> getDefinedSymbol() -> is_defined = true;
}

void CheckVisitor::visit(AddrNode *node)
{
    node -> expr -> accept(*this);

    if ( node -> op == AddrOp::REF )
    {
        if ( !node -> expr -> isLeftValue() )
            throw SemanticError("expression is not an lvalue");

        node -> scope -> getTempAlloc().add(GlobalConfig::int_size);
    }
    else
    {
        auto type = node -> expr -> getType() -> getUnqualifiedType();

        if ( type -> getTypeKind() != TypeKind::POINTER )
            throw SemanticError("expression is not a pointer");
    }
}

void CheckVisitor::visit(NullNode *node)
{
    node -> scope -> getTempAlloc().add(node -> getType() -> getSize());
}

void CheckVisitor::visit(DotNode *node)
{
    node -> base -> accept(*this);

    auto _base_type = node -> base -> getType();

    node -> base_type = dynamic_cast<const StructSymbol*>(_base_type -> getUnqualifiedType());

    if ( node -> base_type == nullptr )
        throw SemanticError("'" + node -> base -> toString() + "' is not an instance of struct.");

    node -> member = dynamic_cast<VariableSymbol*>(node -> base_type -> resolveMember(node -> member_name));

    if ( node -> member == nullptr )
        throw SemanticError(node -> member_name + " is not member of " + node -> base_type -> getName());
}

void CheckVisitor::visit(StatementNode *node)
{
    for ( auto i : node -> statements )
        i -> accept(*this);
}

void CheckVisitor::visit(VariableNode *node)
{
    const auto& template_info = node -> scope -> getTemplateInfo();

    if ( template_info.sym && template_info.sym -> isIn(node -> name) )
    {
        auto replace = template_info.getReplacement(node -> name);

        if ( replace -> which() == 0 )
            throw SemanticError(node -> name + " is typename");

        node -> template_num = new NumberNode(std::to_string(boost::get<int>(*replace)));
        node -> template_num -> scope = node -> scope;
        node -> template_num -> build_scope();

        node -> template_num -> accept(*this);
        return;
    }

    auto sym = node -> scope -> resolve(node -> name);

    if ( sym == nullptr || !sym -> is_defined )
        throw SemanticError("No such symbol '" + node -> name + "'.");

    if ( sym -> getSymbolType() != SymbolType::VARIABLE )
        throw SemanticError("'" + node -> name + "' is not a variable.");

    node -> variable = static_cast<VariableSymbol*>(sym);
}

void CheckVisitor::visit(StringNode *)
{

}

void CheckVisitor::visit(NumberNode *node)
{
    node -> scope -> getTempAlloc().add(node -> getType() -> getSize());
}

void CheckVisitor::visit(CallNode *node)
{
    node -> caller -> accept(*this);

    for ( auto param : node -> params )
        param -> accept(*this);

    auto caller_type = node -> caller -> getType();

    if ( caller_type -> getTypeKind() != TypeKind::OVERLOADEDFUNCTION )
    {
        if ( caller_type -> getSymbol() -> getSymbolType() != SymbolType::STRUCT )
            throw SemanticError("caller '" + node -> caller -> toString() + "' is not a function.");

        node -> call_info = CallHelper::callCheck("operator()", static_cast<const StructSymbol*>(caller_type -> getSymbol()), node -> params);
    }
    else
    {
        auto ov_func = static_cast<const OverloadedFunctionSymbol*>(caller_type);
        auto _scope = ov_func -> isMethod() ? static_cast<const StructSymbol*>(ov_func -> getBaseType() -> getSymbol()) : node -> scope;
        node -> call_info = CallHelper::callCheck(ov_func -> getName(), _scope, node -> params);
    }

    node -> caller -> type_hint = node -> call_info.callee;

    node -> scope -> getTempAlloc().add(node -> getType() -> getSize());
}

void CheckVisitor::visit(ReturnNode *node)
{
    auto _scope = node -> scope;
    while ( _scope != nullptr && dynamic_cast<FunctionSymbol*>(_scope) == nullptr )
        _scope = _scope -> getEnclosingScope();

    if ( _scope == nullptr )
        throw SemanticError("return is not a in a function");

    node -> enclosing_func = dynamic_cast<FunctionSymbol*>(_scope);

    node -> expr -> accept(*this);

    if ( node -> expr -> getType() -> getTypeKind() != TypeKind::POINTER )
        CallHelper::callCheck(node -> expr -> getType() -> getUnqualifiedType() -> getName(), static_cast<const StructSymbol*>(node -> expr -> getType() -> getSymbol()), {node -> expr});

}

void CheckVisitor::visit(UnsafeBlockNode *node)
{
    node -> block -> accept(*this);
}

void CheckVisitor::visit(AsmArrayNode *)
{

}

void CheckVisitor::visit(VarInferTypeDeclarationNode *node)
{
    node -> scope -> getTempAlloc().add(node -> expr -> getType() -> getSize());
    node -> getDefinedSymbol() -> is_defined = true;

    auto type = node -> expr -> getType();

    node -> call_info = CallHelper::callCheck(type -> getName(), static_cast<const StructSymbol*>(type), {node -> expr});
}

void CheckVisitor::visit(TemplateStructDeclarationNode *node)
{
    for ( auto instance : node -> instances )
        instance -> accept(*this);
}
