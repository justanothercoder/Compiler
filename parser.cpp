#include "parser.hpp"

Parser::Parser(AbstractLexer *lexer) : AbstractParser(lexer)
{
    
}

AST* Parser::parse()
{
    AST* root = statement();
    
    return root;
}

AST* Parser::statement()
{
    return declaration();
}

DeclarationNode* Parser::declaration()
{
    return structDecl();
}

DeclarationNode* Parser::structDecl()
{
    match(TokenType::STRUCT);

    string struct_name = id();

    match(TokenType::LBRACE);

    vector<DeclarationNode*> struct_in;
    
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
    
