#include "Calculator/operands.h"

Operand scalarMultiplication(Vector& lhs, Vector& rhs)
{
    if(lhs.size() != rhs.size())
        throw std::logic_error("Can\'t do scalar multiplication vectors with different size");
    Operand sum(OperandsEnum::NUM, new Double);
    for(std::size_t i = 0; i < lhs.size(); i++)
        sum += lhs[i] * rhs[i];
    return sum;
}


Operand vectorMultiplication(Vector& lhs, Vector& rhs)
{
    if ((lhs.size() > 3 || rhs.size() > 3) || lhs.size() != rhs.size())
    {
        throw std::logic_error("Vector Multiplication determine for vectors that have dimension 3 and below");
    }
    std::size_t n = lhs.size();
    Vector result(n);
    for (std::size_t i = 0; i < n; i++) result[i] = lhs[(i + 1) % n] * rhs[(i + 2) % n] - lhs[(i + 2) % n] * rhs[(i + 1) % n];
    return result;
}

template<typename Pred>
IOperand* doVectorOperation(Vector& lhs, const IVector<Operand>& rhs, Pred pred)
{
    for(std::size_t i = 0; i < lhs.size(); i++)
    {
        pred(lhs[i], rhs[i]);
    }
    return &lhs;
}

template<typename Pred>
IOperand* doVectorNumOperation(Vector& lhs, const Double& rhs, Pred pred)
{
    for(std::size_t i = 0; i < lhs.size(); i++)
    {
        pred(lhs[i], rhs);
    }
    return &lhs;
}


Vector::Vector(){}

Vector::Vector(const Vector& otherVector): vector(otherVector.vector){}

Vector::Vector(Vector&& otherVector) noexcept: vector(std::move(otherVector.vector)){}

Vector::Vector(const std::vector<Operand>& otherVector): vector(otherVector){}

Vector::Vector(std::vector<Operand>&& otherVector) noexcept : vector(std::move(otherVector)){}

Vector::Vector(const std::size_t i): vector(i){}

IOperand* Vector::operator=(const Vector& otherVector)
{
    if(this != std::addressof(otherVector))
    {
        return this;
    }
    vector = otherVector.vector;
    return this;
}
IOperand* Vector::operator=(Vector&& otherVector) noexcept
{
    if(this != std::addressof(otherVector))
    {
        return this;
    }
    vector = std::move(otherVector.vector);
    return this;
}

bool Vector::operator==(const Vector& otherVector) const
{
    for(std::size_t i = 0; i < size(); i++)
            if(vector[i] != otherVector[i])
            {
                return false;
            }
    return true;
}
bool Vector::operator!=(const Vector& otherVector) const
{
    return !(*this == otherVector);
}

IOperand* Vector::operator+=(const IVector<Operand> & otherVector)
{
    if(size() != otherVector.size())
    {
        throw std::logic_error("Can\'t add to vectors with different size");
    }
    return doVectorOperation(*this, otherVector, [](auto&& lhs, auto&& rhs){return lhs += rhs;});
}

IOperand* Vector::operator-=(const IVector<Operand> & otherVector)
{
    if(size() != otherVector.size())
    {
        throw std::logic_error("Can\'t substruct to vectors with different size");
    }
    return doVectorOperation(*this, otherVector, [](auto&& lhs, auto&& rhs){return lhs -= rhs;});
}

void Vector::operator*=(const IVector<Operand> &)
{
    throw std::logic_error("Use s=\"scalarMul\" or \"vectorMul\"");
}

void Vector::operator/=(const IVector<Operand> &)
{
    throw std::logic_error("This operation dosent exist");
}

IOperand *Vector::operator-()
{
    for(std::size_t i = 0; i < size(); i++)
    {
        vector[i] = -vector[i];
    }
    return this;
}

IOperand* Vector::operator+=(const INum& num)
{
    const Double realNum = dynamic_cast<const Double&>(num);
    return doVectorNumOperation(*this, realNum, [](auto&& lhs, auto&& rhs){return lhs += rhs;});
}

IOperand* Vector::operator-=(const INum& num)
{
    const Double realNum = dynamic_cast<const Double&>(num);
    return doVectorNumOperation(*this, realNum, [](auto&& lhs, auto&& rhs){return lhs -= rhs;});
}

IOperand* Vector::operator*=(const INum& num)
{
    const Double realNum = dynamic_cast<const Double&>(num);
    return doVectorNumOperation(*this, realNum, [](auto&& lhs, auto&& rhs){return lhs *= rhs;});
}

IOperand* Vector::operator/=(const INum& num)
{
    const Double realNum = dynamic_cast<const Double&>(num);
    return doVectorNumOperation(*this, realNum, [](auto&& lhs, auto&& rhs){return lhs /= rhs;});
}

Vector& Vector::pow(const INum& power)
{
    Double power_ = dynamic_cast<const Double&>(power);
    for(std::size_t i = 0; i < size(); i++)
        vector[i].pow(power_);
    return *this;
}
Vector& Vector::sqrt()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].sqrt();
    return *this;
}
Vector& Vector::abs()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].abs();
    return *this;
}

Vector& Vector::log(double dx)
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].log(dx);
    return *this;
}

Vector& Vector::sin()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].sin();
    return *this;
}
Vector& Vector::cos()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].cos();
    return *this;
}
Vector& Vector::tan(double dx)
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].tan(dx);
    return *this;
}
Vector& Vector::cot(double dx)
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].cot(dx);
    return *this;
}

Vector& Vector::asin()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].asin();
    return *this;
}
Vector& Vector::acos()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].acos();
    return *this;
}
Vector& Vector::atan()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].atan();
    return *this;
}
Vector& Vector::acot()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].acot();
    return *this;
}

Vector& Vector::sinh()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].sinh();
    return *this;
}
Vector& Vector::cosh()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].cosh();
    return *this;
}
Vector& Vector::tanh()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].tanh();
    return *this;
}
Vector& Vector::coth()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].coth();
    return *this;
}

Vector& Vector::asinh()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].asinh();
    return *this;
}
Vector& Vector::acosh()
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].acosh();
    return *this;
}
Vector& Vector::atanh(double dx)
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].atanh(dx);
    return *this;
}
Vector& Vector::acoth(double dx)
{
    for(std::size_t i = 0; i < size(); i++)
        vector[i].acoth(dx);
    return *this;
}


Operand& Vector::operator[](const std::size_t& i)
{
    return vector[i];
}

const Operand& Vector::operator[](const std::size_t& i) const
{
    return vector[i];
}

std::size_t Vector::size() const
{
    return vector.size();
}

std::string Vector::toString() const
{
    std::string str = "{";
    for(std::size_t i = 0; i < size(); i++)
    {
        str += vector[i].toString() + ", ";
    }
    str.pop_back(); str.back() = '}';
    return str;
}



Vector operator+(const Vector& lhs, const Double& rhs)
{
    Vector res(lhs);
    doVectorNumOperation(res, rhs, [](auto&& lhs, auto&& rhs){return lhs += rhs;});
    return res;
}
Vector operator-(const Vector& lhs, const Double& rhs)
{
    Vector res(lhs);
    doVectorNumOperation(res, rhs, [](auto&& lhs, auto&& rhs){return lhs -= rhs;});
    return res;
}
Vector operator*(const Vector& lhs, const Double& rhs)
{
    Vector res(lhs);
    doVectorNumOperation(res, rhs, [](auto&& lhs, auto&& rhs){return lhs *= rhs;});
    return res;
}
Vector operator/(const Vector& lhs, const Double& rhs)
{
    Vector res(lhs);
    doVectorNumOperation(res, rhs, [](auto&& lhs, auto&& rhs){return lhs /= rhs;});
    return res;
}

Vector operator+(const Double& lhs, const Vector& rhs)
{
    Vector res(rhs);
    doVectorNumOperation(res, lhs, [](auto&& lhs, auto&& rhs){return lhs += rhs;});
    return res;
}
Vector operator-(const Double& lhs, const Vector& rhs)
{
    Vector res(rhs);
    doVectorNumOperation(res, lhs, [](auto&& lhs, auto&& rhs){return lhs -= rhs;});
    return res;
}
Vector operator*(const Double& lhs, const Vector& rhs)
{
    Vector res(rhs);
    doVectorNumOperation(res, lhs, [](auto&& lhs, auto&& rhs){return lhs *= rhs;});
    return res;
}
Vector operator/(const Double& lhs, const Vector& rhs)
{
    Vector res(rhs);
    doVectorNumOperation(res, lhs, [](auto&& lhs, auto&& rhs){return lhs /= rhs;});
    return res;
}

Vector operator+(const Vector& lhs, const Vector& rhs)
{
    Vector res(lhs);
    res += rhs;
    return res;
}
Vector operator-(const Vector& lhs, const Vector& rhs)
{
    Vector res(lhs);
    res -= rhs;
    return res;
}
Vector operator*(const Vector& lhs, const Vector& rhs)
{
    Vector res(lhs);
    res *= rhs;
    return res;
}
Vector operator/(const Vector& lhs, const Vector& rhs)
{
    Vector res(lhs);
    res /= rhs;
    return res;
}

//Vector pow(const Vector& vec, const Double& power)
//{
//    Vector res(vec);
//    return res.pow(power);
//}
//Vector sqrt(const Vector& vec)
//{
//    Vector res(vec);
//    return res.sqrt();
//}
//Vector abs(const Vector& vec)
//{
//    Vector res(vec);
//    return res.abs();
//}
//Vector log(const Vector& vec)
//{
//    Vector res(vec);
//    return res.log();
//}

//Vector sin(const Vector& vec)
//{
//    Vector res(vec);
//    return res.sin();
//}
//Vector cos(const Vector& vec)
//{
//    Vector res(vec);
//    return res.cos();
//}
//Vector tan(const Vector& vec)
//{
//    Vector res(vec);
//    return res.tan();
//}
//Vector cot(const Vector& vec)
//{
//    Vector res(vec);
//    return res.cot();
//}

//Vector asin(const Vector& vec)
//{
//    Vector res(vec);
//    return res.asin();
//}
//Vector acos(const Vector& vec)
//{
//    Vector res(vec);
//    return res.acos();
//}
//Vector atan(const Vector& vec)
//{
//    Vector res(vec);
//    return res.atan();
//}
//Vector acot(const Vector& vec)
//{
//    Vector res(vec);
//    return res.acot();
//}

//Vector sinh(const Vector& vec)
//{
//    Vector res(vec);
//    return res.sinh();
//}
//Vector cosh(const Vector& vec)
//{
//    Vector res(vec);
//    return res.cosh();
//}
//Vector tanh(const Vector& vec)
//{
//    Vector res(vec);
//    return res.tanh();
//}
//Vector coth(const Vector& vec)
//{
//    Vector res(vec);
//    return res.coth();
//}

//Vector asinh(const Vector& vec)
//{
//    Vector res(vec);
//    return res.asinh();
//}
//Vector acosh(const Vector& vec)
//{
//    Vector res(vec);
//    return res.acosh();
//}
//Vector atanh(const Vector& vec)
//{
//    Vector res(vec);
//    return res.atanh();
//}
//Vector acoth(const Vector& vec)
//{
//    Vector res(vec);
//    return res.acoth();
//}
