#include "recursivedescent.h"
#include <cmath>
#include <QDebug>
Token::Token(){}
Token::Token(MathEnum mathEnum): kind(mathEnum){}
Token::Token(MathEnum mathEnum, const Operand& op): value(op), kind(mathEnum) {}
Token::Token(MathEnum mathEnum, Operand&& op) noexcept: value(std::move(op)), kind(mathEnum){}
Token::Token(MathEnum mathEnum, const QString& stringValue_): kind(mathEnum), stringName(stringValue_){}
Token::Token(const Token& token): value(token.value), kind(token.kind), stringName(token.stringName)
{}
Token::Token(Token&& token)  noexcept: value(std::move(token.value)), kind(token.kind), stringName(std::move(token.stringName))
{}

Token& Token::operator=(const Token& token)
{
    if(this == std::addressof(token))
    {
        return *this;
    }
    kind = token.kind;
    value = token.value;
    stringName = token.stringName;
    return *this;
}
Token& Token::operator=(Token&& token) noexcept
{
    if(this == std::addressof(token))
    {
        return *this;
    }
    kind = token.kind;
    value = std::move(token.value);
    stringName = std::move(token.stringName);
    return *this;
}


Token TokenStream::getToken()
{
    if(full)
    {
        full = false;
        return buffer;
    }
    QChar ch;
    do{
        ch = *it++;
        if(ch == '\0') return Token(MathEnum::END);
    }while(ch != '\n' && (ch == ' ' || ch == '\t'));

    switch (ch.toLatin1())
    {
    case '(': case ')': case '+': case '-': case '*': case '/': case '^': case ',':
        return Token(MathEnum(ch.toLatin1()));
    case '!':
    {
        if(*it == '!') return Token(MathEnum::DOUBLEFACTORIAL);
        if((*it).isDigit() || *it == '(') return Token(MathEnum::SUBFACTORIAL);
        return Token(MathEnum(ch.toLatin1()));
    }
    case '.': case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
        --it;
        return Token(MathEnum::NUM, Operand(getNum()));
    }
    case '{':
        return Token(MathEnum::VECTORBEGIN);
    case '}':
        return Token(MathEnum::VECTOREND);
    case '[':
        return Token(MathEnum::MATRIXBEGIN);
    case ']':
        return Token(MathEnum::MATRIXEND);
    default:
    {
        --it;
        QString stringValue;
        while(it != expression.end() && ((*it).isLetterOrNumber() || *it == QString("∘") || *it == QString("√")) && (stringValue != "log" || stringValue != "∘"))
        {
            stringValue.push_back(*it++);
        }
        return Token(MathEnum::NAME, stringValue);
    }
    }
}

TokenStream::TokenStream()
{
    full = false;
}

void TokenStream::putTokenBack(const Token& token)
{
    full = true;
    buffer = token;
}
void TokenStream::putTokenBack(Token&& token) noexcept
{
    full = true;
    buffer = std::move(token);

}

void TokenStream::clearBuffer()
{
    full = false;
}

void TokenStream::setExpression(const QString & expression_)
{
    full = false;
    expression = expression_;
    it = expression.cbegin();
}

Operand TokenStream::getNum()
{
    QString num;
    for(auto end = expression.cend(); it != end && ((*it).isDigit() || *it == '.'); it++ )
    {
        num.push_back(*it);
    }
    return Operand(new Double((num).toDouble()));
}

Operand factorial(const Operand& fact)
{
    if(fact.getOperandType() != OperandsEnum::NUM)
    {
        throw std::logic_error("This operation can't be completed");
    }
    Double res = dynamic_cast<Double&>(*fact.getOperand());
    res = tgamma(res + Double(1.0));
    return Operand(res);
}

Operand doubleFactorial(const Operand& fact)
{
    if(fact.getOperandType() != OperandsEnum::NUM)
    {
        throw std::logic_error("This operation can't be completed");
    }
    Double res = dynamic_cast<Double&>(*fact.getOperand());
    res = (::pow(2.0, double(res / Double(2.0)))) * tgamma(res / Double(2.0));
    return Operand(res);
}

Operand subfactorial(const Operand& num)
{
    if(num.getOperandType() != OperandsEnum::NUM)
    {
        throw std::logic_error("This operation can't be completed");
    }
    double tmp = 0, fact = 1.0;
    std::size_t count = 0, numOfIterations = (double)dynamic_cast<Double&>(*num.getOperand());
    for (std::size_t i = 1; i <= numOfIterations; i++)
    {
        fact = fact * i;
        if (count % 2 == 0)
        {
            tmp = tmp - (1.0 / fact);
        }
        else
        {
            tmp = tmp + (1.0 / fact);
        }
        count++;
    }
    Double res = fact * (1 + tmp);
    return Operand(res);
}

Operand logarithm(Operand& num, Operand& order, double eps = 0)
{
    num.log();
    if(num.getOperandType() == OperandsEnum::MATRIX && order.getOperandType() == OperandsEnum::MATRIX)
    {
        dynamic_cast<Matrix&>(*order.getOperand()).inverseMatrix();
        order.log(eps);
        num *= order;
        return num;
    }
    order.log(eps);
    num /= order;
    return num;
}

RecursiveDecsent::RecursiveDecsent(): eps(0)
{
    functionTable.emplace("sin", &Operand::sin);
    functionTable.emplace("cos", &Operand::cos);
    epsFunctionTable.emplace("tan", &Operand::tan);
    epsFunctionTable.emplace("cot", &Operand::cot);

    functionTable.emplace("asin", &Operand::asin);
    functionTable.emplace("acos", &Operand::acos);
    functionTable.emplace("atan", &Operand::atan);
    functionTable.emplace("acot", &Operand::acot);

    functionTable.emplace("sinh", &Operand::sinh);
    functionTable.emplace("cosh", &Operand::cosh);
    functionTable.emplace("tanh", &Operand::tanh);
    functionTable.emplace("coth", &Operand::coth);

    functionTable.emplace("asinh", &Operand::asinh);
    functionTable.emplace("acosh", &Operand::acosh);
    epsFunctionTable.emplace("atanh", &Operand::atanh);
    epsFunctionTable.emplace("acoth", &Operand::acoth);

    epsFunctionTable.emplace("ln", &Operand::log);
    epsFunctionTable.emplace("lg", &Operand::lg);
    functionTable.emplace("abs", &Operand::abs);
    functionTable.emplace("√", &Operand::sqrt);

    argsTable.emplace("e", Double(exp(1)));
    argsTable.emplace("π", Double(M_PI));
}


Operand RecursiveDecsent::calculate(const QString &expression_)
{
    tokenStream.setExpression(expression_);
    Operand op = expression();
    tokenStream.clearBuffer();
    return op;
}

Operand RecursiveDecsent::expression()
{

    Operand left = term();
    Token t = tokenStream.getToken();
    while (true)
    {
        switch (t.kind)
        {
        case MathEnum::PLUS:
            left += term();
            t = tokenStream.getToken();
            break;
        case MathEnum::MINUS:
            left -= term();
            t = tokenStream.getToken();
            break;
        default:
                tokenStream.putTokenBack(std::move(t));
            return left;
        }
    }
}

Operand RecursiveDecsent::term()
{
    Operand left = upterm();
    Token t = tokenStream.getToken();
    while (true)
    {
        switch (t.kind)
        {
        case MathEnum::MUL:
            left *= upterm();
            t = tokenStream.getToken();
            break;
        case MathEnum::DIV:
        {
            Operand d = upterm();
            if ((d <= Double(eps) && d > Double(-eps))) throw std::logic_error("Zero division");
            left /= d;
            t = tokenStream.getToken();
            break;
        }
        default:
            tokenStream.putTokenBack(std::move(t));
            return left;
        }
    }
}

Operand RecursiveDecsent::upterm()
{
    Operand left = primary();


    Token t = tokenStream.getToken();
    while (true)
    {
        switch (t.kind)
        {
        case MathEnum::FACTORIAL:
            left = factorial(left);
            t = tokenStream.getToken();
            break;
        case MathEnum::DOUBLEFACTORIAL:
        {
            left = doubleFactorial(left);
            t = tokenStream.getToken();
            break;
        }
        case MathEnum::POW:
        {
            Operand power = primary();
            if(power.getOperandType() != OperandsEnum::NUM)
            {
                throw std::logic_error("Power is not a number");
            }
            left.pow(dynamic_cast<Double&>(*power.getOperand()));
            t = tokenStream.getToken();
            break;
        }
        default:
            tokenStream.putTokenBack(std::move(t));
            return left;
        }
    }
}

Operand RecursiveDecsent::primary()
{
    Token t = tokenStream.getToken();
    switch (t.kind)
    {
    case MathEnum::LEFTBRACKET:
    {
        Operand d = expression();
        t = tokenStream.getToken();
        if (t.kind != MathEnum::RIGHTBRACKET) throw std::logic_error("Expected \')\'");
        return d;
    }
    case MathEnum::NUM:
    {
        Operand val = t.value;
        t = tokenStream.getToken();
        if(t.stringName == "∘")
        {
            val *= Double(M_PI);
            val /= Double(180.0);
        }
        else
        {
            tokenStream.putTokenBack(t);
        }
        return val;
    }
    case MathEnum::VECTORBEGIN:
    {
        std::vector<Operand> vec;
        do
        {
            vec.emplace_back(expression());
            t = tokenStream.getToken();
            if(t.kind == MathEnum::END)
            {
                throw std::logic_error("Uncorrect enter vector");
            }
        }while(t.kind == MathEnum::NEXTOPERAND);
        if(t.kind != MathEnum::VECTOREND) throw std::logic_error("Error: define of vector is incorrect");
        return Operand(new Vector(std::move(vec)));
    }
    case MathEnum::MATRIXBEGIN:
    {
        std::vector<std::vector<Operand> > matr;
        std::size_t row = 0, column = 0;
        t = tokenStream.getToken();
        if(t.kind != MathEnum::MATRIXBEGIN)
        {
            throw std::logic_error("Uncorrect enter matrix");
        }
        do{
            row++;
            matr.emplace_back(std::vector<Operand>());
            do{
                matr[row - 1].push_back(expression());
                t = tokenStream.getToken();
                if(t.kind == MathEnum::END)
                {
                    throw std::logic_error("Uncorrect enter matrix");
                }
            }while(t.kind != MathEnum::MATRIXEND && t.kind == MathEnum::NEXTOPERAND);
            t = tokenStream.getToken();
            if(t.kind == MathEnum::END)
            {
                throw std::logic_error("Uncorrect enter matrix");
            }
        }while(t.kind != MathEnum::MATRIXEND && t.kind == MathEnum::MATRIXBEGIN);
        column = matr[0].size();
        for(std::size_t i = 0; i < matr.size(); i++)
        {
            if(column != matr[i].size())
            {
                throw std::logic_error("Uncorrect enter matrix");
            }
        }
        t = tokenStream.getToken();
        Matrix* matrix = new Matrix(matr);
        if(t.stringName == "T")
        {
            matrix->transpose();
        }
        else
        {
            tokenStream.putTokenBack(t);
        }
        return Operand(matrix);
    }
    case MathEnum::MINUS:
        return -primary();
    case MathEnum::PLUS:
        return primary();
    case MathEnum::DOUBLEFACTORIAL: case MathEnum::FACTORIAL:
        return Operand(new Double);
    case MathEnum::SUBFACTORIAL:
        return subfactorial(primary());
    case MathEnum::NAME:
    {

        if (argsTable.find(t.stringName) != argsTable.end())
        {
            return argsTable[t.stringName];
        }
        if(t.stringName == "det")
        {
            Operand op = primary();
            return determine(op);
        }
        if(t.stringName == "scalar" || t.stringName == "vector")
        {
            t = tokenStream.getToken();
            if(t.kind != MathEnum::LEFTBRACKET)
            {
                throw std::logic_error("Function error");
            }
            Operand p1 = expression();
            Operand p2 = expression();

            if(t.stringName == "scalar")
            {
                return scalarMultiplication(p1, p2);
            }
            else if(t.stringName == "vector")
            {
                return vectorMultiplication(p1, p2);
            }
        }
        if (t.stringName != "log" && functionTable.find(t.stringName) == functionTable.end() && epsFunctionTable.find(t.stringName) == epsFunctionTable.end())
        {
            throw std::logic_error("Unknown variable or function");
        }
        if (t.stringName != "log" &&  epsFunctionTable.find(t.stringName) != epsFunctionTable.end())
        {
            Operand d = primary();
            (d.*epsFunctionTable[t.stringName])(eps);
            return d;
        }
        if (t.stringName != "log" &&  functionTable.find(t.stringName) != functionTable.end())
        {
            Operand d = primary();
            (d.*functionTable[t.stringName])();
            return d;
        }

        Operand order = primary();
        Operand num = primary();
        return logarithm(num, order, eps);
        break;
    }
    default:
        throw std::logic_error("Enter error");
        break;
    }
}

void RecursiveDecsent::clearBuffer()
{
    tokenStream.clearBuffer();
}

void RecursiveDecsent::addArgumentToTable(const QString &name, const Operand operand)
{
    argsTable.emplace(name, operand);
}

void RecursiveDecsent::setEpsilon(double newEps)
{
    eps = newEps;
}


Operand& RecursiveDecsent::getElementFromArgumentTable(const QString& name)
{
    return  argsTable[name];
}
