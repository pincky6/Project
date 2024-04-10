#ifndef RECURSIVEDESCENT_H
#define RECURSIVEDESCENT_H

#include "operands.h"
#include <map>
#include <QString>

enum class MathEnum
{
    NUM,
    VECTORBEGIN = '{', VECTOREND = '}',
    MATRIXBEGIN = '[', MATRIXEND = ']',
    PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
    LEFTBRACKET = '(', RIGHTBRACKET = ')',
    FACTORIAL = '!', DOUBLEFACTORIAL, SUBFACTORIAL,
    POW = '^', NEXTOPERAND = ',', NAME = 'z', END = '\0'
};

struct Token
{
public:
    Token();
    Token(MathEnum);
    Token(MathEnum, const Operand&);
    Token(MathEnum, Operand&&) noexcept;
    Token(MathEnum, const QString&);
    Token(const Token&);
    Token(Token&&) noexcept;
    Token& operator=(const Token&);
    Token& operator=(Token&&) noexcept;
public:
    Operand value;
    MathEnum kind;
    QString stringName;
};

class TokenStream
{
public:
    TokenStream();

    Token getToken();
    void putTokenBack(const Token&);
    void putTokenBack(Token&&) noexcept;
    void clearBuffer();
    void setExpression(const QString&);
    Operand getNum();
private:
    bool full;
    Token buffer;
    QString expression;
    QString::ConstIterator it;
};


class RecursiveDecsent
{
    using FunctionTable = std::map<QString, Operand&(Operand::*)()>;
    using EpsFunctionTable = std::map<QString, Operand&(Operand::*)(double)>;
    using ArgumentTable = std::map<QString, Operand>;
public:
    RecursiveDecsent();

    Operand calculate(const QString&);

    Operand expression();
    Operand term();
    Operand upterm();
    Operand primary();

    void clearBuffer();
    void addArgumentToTable(const QString&, const Operand);
    void setEpsilon(double);
    Operand& getElementFromArgumentTable(const QString&);
private:
    TokenStream tokenStream;
    FunctionTable functionTable;
    EpsFunctionTable epsFunctionTable;
    ArgumentTable argsTable;
    double eps;
};

#endif
