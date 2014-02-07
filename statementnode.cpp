#include "statementnode.hpp"

StatementNode::StatementNode(const vector<AST*>& statements) : statements(statements)
{

}

StatementNode::~StatementNode()
{
    for ( auto i : statements )
	delete i;
}

void StatementNode::build_scope()
{
    for ( auto i : this->statements )
    {
	i->setScope(getScope());
	i->build_scope();
    }
}

void StatementNode::define()
{
    for ( auto i : statements )
	i->define();
}

void StatementNode::check()
{
    for ( auto i : statements )
	i->check();
}

void StatementNode::gen()
{
    for ( auto i : statements )
	i->gen();
}

void StatementNode::template_define(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    for ( auto i : statements )
	i->template_define(template_sym, expr);
}

void StatementNode::template_check(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    for ( auto i : statements )
	i->template_check(template_sym, expr);
}

bool StatementNode::isTemplated() const
{
    bool is_templated = false;
    
    for ( auto i : statements )
	is_templated |= i->isTemplated();

    return is_templated;
}

AST* StatementNode::copyTree() const
{
    vector<AST*> stats;

    std::transform(std::begin(statements), std::end(statements), std::back_inserter(stats), [&](AST *t) { return t->copyTree(); });
    
    return new StatementNode(stats);
}

void StatementNode::template_gen(const TemplateStructSymbol *template_sym, const std::vector<ExprNode*>& expr)
{
    for ( auto i : statements )
	i->template_gen(template_sym, expr);
}
