#include "Calculator/operands.h"
#include <cmath>
#include <sstream>
#include <QDebug>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Double& tryCast(const INum& op1, Double& op2)
{
    op2 = dynamic_cast<const Double&>(op1);
    return op2;
}

Double::Double(double otherValue): value(otherValue){}

bool Double::operator==(const INum& number) const
{
    Double doubleNumber;
    doubleNumber = tryCast(number, doubleNumber);
    return value == doubleNumber.value;
}

bool Double::operator!=(const INum& number) const
{
    Double doubleNumber;
    doubleNumber = tryCast(number, doubleNumber);
    return value != doubleNumber.value;
}

bool Double::operator<(const INum& number) const
{
    Double doubleNumber;
    doubleNumber = tryCast(number, doubleNumber);
    return value < doubleNumber.value;
}

bool Double::operator>(const INum& number) const
{
    Double doubleNumber;
    doubleNumber = tryCast(number, doubleNumber);
    return value > doubleNumber.value;
}

bool Double::operator<=(const INum& number) const
{
    Double doubleNumber;
    doubleNumber = tryCast(number, doubleNumber);
    return value <= doubleNumber.value;
}

bool Double::operator>=(const INum& number) const
{
    Double doubleNumber;
    doubleNumber = tryCast(number, doubleNumber);
    return value >= doubleNumber.value;
}


IOperand* Double::operator+=(const INum& number)
{
    Double doubleNumber;
    doubleNumber = tryCast(number, doubleNumber);
    value += doubleNumber.value;
    return this;;
}

IOperand* Double::operator-=(const INum& number)
{
    Double doubleNumber;
    doubleNumber = tryCast(number, doubleNumber);
    value -= doubleNumber.value;
    return this;;
}

IOperand* Double::operator*=(const INum& number)
{
    Double doubleNumber;
    doubleNumber = tryCast(number, doubleNumber);
    value *= doubleNumber.value;
    return this;
}

IOperand* Double::operator/=(const INum& number)
{
    Double doubleNumber;
    doubleNumber = tryCast(number, doubleNumber);
    value /= doubleNumber.value;
    return this;
}

Double& Double::pow(const INum& power)
{
    Double power_ = dynamic_cast<const Double&>(power);
    value = ::pow(value, (double)power_);
    return *this;
}
Double& Double::sqrt()
{
    if(value < 0)
    {
        throw std::logic_error("Value in square root less then 0");
    }
    value = ::sqrt(value);
    return *this;
}
Double& Double::abs()
{
    value = ::fabs(value);
    return *this;
}

Double& Double::log(double dx)
{
    if(value < 0)throw std::logic_error("Out of range in domain of the logarithm");
    if(value <= 0 + dx)
    {
        throw DomainError(-99999999);
    }
    value = ::log(value);
    return *this;
}

Double& Double::sin()
{
    value = ::sin(value);
    return *this;
}
Double& Double::cos()
{
    value = ::cos(value);
    return *this;
}
Double& Double::tan(double dx)
{
    if(fabs(::cos(value)) < 0 + dx)
    {
       throw DomainError((value < 0) ? ::tan(-M_PI/2.0): ::tan(M_PI/2));
    }
    value = ::tan(value);
    return *this;
}
Double& Double::cot(double dx)
{
    if(fabs(::sin(value)) < 0 + dx)
    {
        throw DomainError((value < 0) ? 1.0/(::tan(M_PI)): 1.0/::tan(-M_PI));
    }
    value = 1.0 /  ::tan(value);
    return *this;
}

Double& Double::asin()
{
    value = ::asin(value);
    return *this;
}
Double& Double::acos()
{
    value = ::acos(value);
    return *this;
}
Double& Double::atan()
{

    value = ::atan(value);
    return *this;
}
Double& Double::acot()
{
    value = M_PI / 2.0 - ::atan(value);
    return *this;
}

Double& Double::sinh()
{
    value = ::sinh(value);
    return *this;
}
Double& Double::cosh()
{
    value = ::cosh(value);
    return *this;
}
Double& Double::tanh()
{
    value = ::tanh(value);
    return *this;
}
Double& Double::coth()
{
    value = 1.0 / ::tanh(value);
    return *this;
}

Double& Double::asinh()
{
    value = ::asinh(value);
    return *this;
}
Double& Double::acosh()
{
    value = ::acosh(value);
    return *this;
}
Double& Double::atanh(double dx)
{
    if(value > 1.0 - dx || value < -1.0 + dx)
    {
        throw DomainError(value >= 0.9 ? ::atanh(0.9999999999999) :  (value <= -0.9) ? ::atanh(-0.9999999999999): 0);
    }
    value = ::atanh(value);
    return *this;
}
Double& Double::acoth(double dx)
{
    if(value > 1.0 - dx && value < -1.0 + dx)
    {
        throw DomainError(value >= 1 ? (0.5 * ::log((1 + dx + 1.0) / (1 + dx - 1.0))) :  (value <= -1) ? 0.5 * ::log((-1.0 - dx + 1.0) / (-1.0 - dx- 1.0)): 0);
    }
    value = 0.5 * ::log((value + 1.0) / (value - 1.0));
    return *this;
}

Double& Double::operator++()
{
    value++;
    return *this;
}

Double Double::operator++(int)
{
    Double tmp = *this;
    value++;
    return tmp;
}

Double& Double::operator--()
{
    value++;
    return *this;
}

Double Double::operator--(int)
{
    Double tmp = *this;
    value--;
    return tmp;
}

Double& Double::operator-()
{
    value = -value;
    return *this;
}

Double::operator double() const
{
    return value;
}

Double::operator bool() const
{
    if( value == 0.0) return false;
    return true;
}

std::string Double::toString() const
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

Double operator+(const Double& rhs, const Double& lhs)
{
    Double res(rhs);
    res += lhs;
    return res;
}

Double operator-(const Double& rhs, const Double& lhs)
{
    Double res(rhs);
    res -= lhs;
    return res;
}

Double operator*(const Double& rhs, const Double& lhs)
{
    Double res(rhs);
    res *= lhs;
    return res;
}

Double operator/(const Double& rhs, const Double& lhs)
{
    Double res(rhs);
    res /= lhs;
    return res;
}

//Double pow(const Double& num, const Double& power)
//{
//    Double res(num);
//    return res.pow(power);
//}
//Double sqrt(const Double& num)
//{
//    Double res(num);
//    return res.sqrt();
//}
//Double abs(const Double& num)
//{
//    Double res(num);
//    return res.abs();
//}

//Double log(const Double& num)
//{
//    Double res(num);
//    return res.log();
//}

//Double sin(const Double& num)
//{
//    Double res(num);
//    return res.sin();
//}
//Double cos(const Double& num)
//{
//    Double res(num);
//    return res.cos();
//}
//Double tan(const Double& num)
//{
//    Double res(num);
//    return res.tan();
//}
//Double cot(const Double& num)
//{
//    Double res(num);
//    return res.cot();
//}

//Double asin(const Double& num)
//{
//    Double res(num);
//    return res.asin();
//}
//Double acos(const Double& num)
//{
//    Double res(num);
//    return res.acos();
//}
//Double atan(const Double& num)
//{
//    Double res(num);
//    return res.atan();
//}
//Double acot(const Double& num)
//{
//    Double res(num);
//    return res.acot();
//}

//Double sinh(const Double& num)
//{
//    Double res(num);
//    return res.sinh();
//}
//Double cosh(const Double& num)
//{
//    Double res(num);
//    return res.cosh();
//}
//Double tanh(const Double& num)
//{
//    Double res(num);
//    return res.tanh();
//}
//Double coth(const Double& num)
//{
//    Double res(num);
//    return res.coth();
//}

//Double asinh(const Double& num)
//{
//    Double res(num);
//    return res.asinh();
//}
//Double acosh(const Double& num)
//{
//    Double res(num);
//    return res.acosh();
//}
//Double atanh(const Double& num)
//{
//    Double res(num);
//    return res.atanh();
//}
//Double acoth(const Double& num)
//{
//    Double res(num);
//    return res.acoth();
//}
