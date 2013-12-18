#include "parser.hpp"

AST* Parser::parse()
{
    AST* root = statement();
    
    return root;
}

AST* Parser::statement()
{
    return declaration();
}

AST* Parser::declaration()
{
    return structDecl();
}

AST* Parser::structDecl()
{
    match(TokenType::STRUCT);

    string struct_name = id();

    match(TokenType::LBRACE);

    vector<AST*> struct_in;
    
    while ( getTokenType(1) != TokenType::RBRACE )
	struct_in.push_back(declaration());
    
    match(TokenType::RBRACE);

    return new StructDeclarationNode(struct_name, struct_in);
}

string Parser::id()
{
    string name = getToken(1).text;
    match(TokenType::ID);
    return name;
}
    
