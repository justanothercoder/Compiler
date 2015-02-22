#include "rebuildtreevisitor.hpp"
#include "ifnode.hpp"
#include "dotnode.hpp"
#include "fornode.hpp"
#include "nullnode.hpp"
#include "addrnode.hpp"
#include "callnode.hpp"
#include "unarynode.hpp"
#include "breaknode.hpp"
#include "whilenode.hpp"
#include "stringnode.hpp"
#include "numbernode.hpp"
#include "importnode.hpp"
#include "modulenode.hpp"
#include "returnnode.hpp"
#include "externnode.hpp"
#include "bracketnode.hpp"
#include "variablenode.hpp"
#include "functionnode.hpp"
#include "statementnode.hpp"
#include "unsafeblocknode.hpp"
#include "newexpressionnode.hpp"
#include "binaryoperatornode.hpp"
#include "templatefunctionnode.hpp"
#include "structdeclarationnode.hpp"
#include "modulememberaccessnode.hpp"
#include "functiondeclarationnode.hpp"
#include "variabledeclarationnode.hpp"
#include "varinfertypedeclarationnode.hpp"
#include "templatestructdeclarationnode.hpp"
#include "templatefunctiondeclarationnode.hpp"

#include "logger.hpp"

RebuildTreeVisitor::RebuildTreeVisitor(TemplateInfo template_info) : template_info(template_info) { assert(template_info.sym); }

ASTNode RebuildTreeVisitor::result() { return std::move(_ast); }

ASTNode RebuildTreeVisitor::rebuild(AST* node)
{
    node -> accept(*this);
    return std::move(_ast);
}

ASTExprNode RebuildTreeVisitor::rebuild(ExprNode* node)
{
    return ASTExprNode(static_cast<ExprNode*>(rebuild(static_cast<AST*>(node)).release()));
}

TypeInfo RebuildTreeVisitor::processTypeInfo(TypeInfo type_info)
{
    auto name               = std::string("");
    auto is_ref             = type_info.isRef();
    auto is_const           = type_info.isConst();
    auto template_arguments = TemplateArgumentsInfo{ };
    auto modifiers          = std::vector<TypeModifier>{ };
    auto module_name        = type_info.moduleName();

    if ( type_info.name() == template_info.sym -> getName() )
    {
        name               = template_info.getInstName();
        template_arguments = type_info.templateArgumentsInfo();
        modifiers          = type_info.modifiers();
        module_name        = "";
    }
    else if ( template_info.isIn(type_info.name()) )
    {
        auto replace = template_info.getReplacement(type_info.name());
        auto new_info = boost::get<TypeInfo>(*replace);

        auto modifiers_ = new_info.modifiers();
        modifiers_.insert(std::end(modifiers_), std::begin(type_info.modifiers()), std::end(type_info.modifiers()));
        
        name               = new_info.name();
        is_ref             = new_info.isRef()   || type_info.isRef();
        is_const           = new_info.isConst() || type_info.isConst();
        template_arguments = new_info.templateArgumentsInfo();
        modifiers          = modifiers_; 
        module_name        = "";
    }
    else
    {
        name               = type_info.name();
        template_arguments = type_info.templateArgumentsInfo();
        modifiers          = type_info.modifiers();
    }

    for ( auto& param : template_arguments )
    {
        if ( param.which() == 0 )
            param = std::shared_ptr<ExprNode>{rebuild(boost::get< std::shared_ptr<ExprNode> >(param).get())};
    }

    for ( auto& modifier : modifiers )
    {
        if ( auto dim = modifier.dimension() )
            modifier = TypeModifier(std::shared_ptr<ExprNode>{rebuild(*dim)});
    }

    return TypeInfo(name, is_ref, is_const, template_arguments, modifiers, module_name);
}

void RebuildTreeVisitor::visit(IfNode* node) 
{
    _ast = std::make_unique<IfNode>(rebuild(node -> condition()), rebuild(node -> trueBranch()), rebuild(node -> falseBranch()));
}

void RebuildTreeVisitor::visit(ForNode* node) 
{
    _ast = std::make_unique<ForNode>(rebuild(node -> initializer())
                                   , rebuild(node -> condition())
                                   , rebuild(node -> iteration())
                                   , rebuild(node -> body()));
}

void RebuildTreeVisitor::visit(CallNode* node) 
{
    auto new_params = std::vector<ASTExprNode>{ };

    for ( const auto& param : node -> params() )
        new_params.emplace_back(rebuild(param.get()));

    _ast = std::make_unique<CallNode>(rebuild(node -> caller()), std::move(new_params));
}

void RebuildTreeVisitor::visit(VariableNode* node) 
{
    if ( template_info.isIn(node -> name()) )
    {
        auto replace = template_info.getReplacement(node -> name());

        if ( replace -> which() == 0 )
            throw SemanticError(node -> name() + " is typename");

        _ast = std::make_unique<NumberNode>(std::to_string(boost::get<int>(*replace)));
    }
    else 
        _ast = std::make_unique<VariableNode>(node -> name());
}

void RebuildTreeVisitor::visit(StatementNode* node) 
{
    auto new_statements = std::vector<ASTNode>{ };

    for ( const auto& stat : node -> statements() )
        new_statements.emplace_back(rebuild(stat.get()));

    _ast = std::make_unique<StatementNode>(std::move(new_statements));
}

void RebuildTreeVisitor::visit(NewExpressionNode* node) 
{
    auto new_type_info = processTypeInfo(node -> typeInfo());
    auto new_params = std::vector<ASTExprNode>{ };

    for ( const auto& param : node -> params() )
        new_params.emplace_back(rebuild(param.get()));

    _ast = std::make_unique<NewExpressionNode>(new_type_info, std::move(new_params));
}

void RebuildTreeVisitor::visit(BinaryOperatorNode* node) 
{
    _ast = std::make_unique<BinaryOperatorNode>(rebuild(node -> lhs()), rebuild(node -> rhs()), node -> op());
}

void RebuildTreeVisitor::visit(StructDeclarationNode* node) 
{
    auto new_inner = std::vector<ASTNode>{ };

    for ( const auto& decl : node -> inner() )
        new_inner.emplace_back(rebuild(decl.get()));

    _ast = std::make_unique<StructDeclarationNode>(node -> name(), std::move(new_inner));
}

void RebuildTreeVisitor::visit(ModuleMemberAccessNode* node) 
{
    _ast = std::make_unique<ModuleMemberAccessNode>(node -> name(), node -> member());
}

void RebuildTreeVisitor::visit(FunctionDeclarationNode* node) 
{
    auto new_return_type_info = processTypeInfo(node -> info().returnTypeInfo());
    auto new_formal_params = std::vector<ParamInfo>{ };

    for ( auto param : node -> info().formalParams() )
        new_formal_params.emplace_back(param.first, processTypeInfo(param.second));

    auto new_info = FunctionDeclarationInfo(new_return_type_info, new_formal_params);

    _ast = std::make_unique<FunctionDeclarationNode>(node -> name()
                                                   , new_info 
                                                   , rebuild(node -> body())
                                                   , node -> traits()
                                                   , node -> isUnsafe());
}

void RebuildTreeVisitor::visit(VariableDeclarationNode* node) 
{
    auto new_type_info = processTypeInfo(node -> typeInfo());
    auto new_params = std::vector<ASTExprNode>{ };

    for ( const auto& param : node -> constructorParams() )
        new_params.emplace_back(rebuild(param.get()));

    _ast = std::make_unique<VariableDeclarationNode>(node -> name()
                                                   , new_type_info
                                                   , node -> isField()
                                                   , std::move(new_params));
}

void RebuildTreeVisitor::visit(VarInferTypeDeclarationNode* node) 
{
    _ast = std::make_unique<VarInferTypeDeclarationNode>(node -> name(), rebuild(node -> expr()));
}

void RebuildTreeVisitor::visit(TemplateStructDeclarationNode* node) 
{
    auto new_inner = std::vector<ASTNode>{ };

    for ( const auto& decl : node -> getChildren() )
        new_inner.emplace_back(rebuild(decl));

    _ast = std::make_unique<TemplateStructDeclarationNode>(node -> name()
                                                         , std::move(new_inner)
                                                         , node -> templateParamsInfo());
}

void RebuildTreeVisitor::visit(TemplateFunctionDeclarationNode* node) 
{
    _ast = std::make_unique<TemplateFunctionDeclarationNode>(node -> name()
                                                           , node -> info()
                                                           , rebuild(node -> body())
                                                           , node -> traits()
                                                           , node -> isUnsafe()
                                                           , node -> templateParamsInfo());
}

void RebuildTreeVisitor::visit(TemplateFunctionNode* node) 
{
    _ast = std::make_unique<TemplateFunctionNode>(node -> name(), node -> templateArgumentsInfo());
}

void RebuildTreeVisitor::visit(NullNode* )         { _ast = std::make_unique<NullNode>(); }
void RebuildTreeVisitor::visit(BreakNode* )        { _ast = std::make_unique<BreakNode>(); }
void RebuildTreeVisitor::visit(DotNode* node)      { _ast = std::make_unique<DotNode>(rebuild(node -> base()), node -> memberName()); }
void RebuildTreeVisitor::visit(AddrNode* node)     { _ast = std::make_unique<AddrNode>(rebuild(node -> expr()), node -> op()); }
void RebuildTreeVisitor::visit(WhileNode* node)    { _ast = std::make_unique<WhileNode>(rebuild(node -> condition()), rebuild(node -> body())); }
void RebuildTreeVisitor::visit(UnaryNode* node)    { _ast = std::make_unique<UnaryNode>(rebuild(node -> expr()), node -> op()); } 
void RebuildTreeVisitor::visit(ImportNode* node)   { _ast = std::make_unique<ImportNode>(node -> lib, node -> root, node -> imports); }
void RebuildTreeVisitor::visit(StringNode* node)   { _ast = std::make_unique<StringNode>(node -> str()); }
void RebuildTreeVisitor::visit(NumberNode* node)   { _ast = std::make_unique<NumberNode>(node -> num()); }
void RebuildTreeVisitor::visit(ReturnNode* node)   { _ast = std::make_unique<ReturnNode>(rebuild(node -> expr())); }
void RebuildTreeVisitor::visit(ExternNode* node)   { _ast = std::make_unique<ExternNode>(node -> name(), node -> info(), node -> isUnsafe()); }
void RebuildTreeVisitor::visit(ModuleNode* node)   { _ast = std::make_unique<ModuleNode>(node -> name()); }
void RebuildTreeVisitor::visit(BracketNode* node)  { _ast = std::make_unique<BracketNode>(rebuild(node -> base()), rebuild(node -> expr())); } 
void RebuildTreeVisitor::visit(FunctionNode* node) { _ast = std::make_unique<FunctionNode>(node -> name()); }
void RebuildTreeVisitor::visit(UnsafeBlockNode* node) { _ast = std::make_unique<UnsafeBlockNode>(rebuild(node -> block())); } 
