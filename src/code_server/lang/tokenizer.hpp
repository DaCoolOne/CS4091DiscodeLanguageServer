#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <istream>

namespace discode
{

enum TokenType
{
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_STRING,
};

enum Keyword
{
    KEYWORD_NULL,
    KEYWORD_TRUE,
    KEYWORD_FALSE,
    KEYWORD_IF,
    KEYWORD_RETURN,
    KEYWORD_DECLARE,
    KEYWORD_STRUCTURE,
    KEYWORD_AND,
    KEYWORD_OR,
    KEYWORD_NOT,

    OPERATOR_PLUS,
    OPERATIR_MINUS,
    OPERATOR_STAR,
    OPERATOR_DIV,

    OPERATOR_EQ,
    OPERATOR_LSS,
    OPERATOR_GTR,
    OPERATOR_LSEQ,
    OPERATOR_GTEQ,
    OPERATOR_NEQ,

    OPERATOR_EXCLAM,
    OPERATOR_POUND,

    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,
};

class Token
{
    TokenType _type;
public:
    Token(TokenType type): _type(type) {}

    virtual double getNum() { throw std::logic_error("No number to get"); }
    virtual std::string getString() { throw std::logic_error("No string to get"); }
    virtual Keyword getKeyword() { throw std::logic_error("No keyword to get"); }
};

class NumberToken : Token
{
    double num;
public:
    NumberToken(double value): Token(TokenType::TOKEN_NUMBER), num(value) {}
    double getNum() { return num; }
};

class StringToken : Token
{
    std::string text;
public:
    StringToken(std::string value): Token(TokenType::TOKEN_STRING), text(value) {}
    std::string getString() { return text; }
};

class KeywordToken : Token
{
    Keyword keyword;
public:
    KeywordToken(Keyword value): Token(TokenType::TOKEN_KEYWORD), keyword(value) {}
    Keyword getKeyword() { return keyword; }
};


class Tokenizer
{
    std::istream * str;
public:
    Tokenizer(std::istream * stream): str(stream) {}
};

};
#endif