#include "Calculator/operands.h"
#include <cmath>
Operand::Operand(): operand(new Double), name(OperandsEnum::NUM){}

Operand::Operand(OperandsEnum name_, IOperand* operand_):  operand(operand_), name(name_)
{}
Operand::Operand(const Operand& otherOperand): name(otherOperand.name)
{
    IOperand* newOp;
    switch (name)
    {
    case OperandsEnum::NUM:
        newOp = new Double(dynamic_cast<const Double&>(*otherOperand.getOperand()));
        break;
    case OperandsEnum::VECTOR:
        newOp = new Vector(dynamic_cast<const Vector&>(*otherOperand.getOperand()));
        break;
    case OperandsEnum::MATRIX:
        newOp = new Matrix(dynamic_cast<const Matrix&>(*otherOperand.getOperand()));
        break;
    }
    operand.reset(newOp);
}
Operand::Operand(Operand&& otherOperand) noexcept: operand(std::move(otherOperand.operand)), name(otherOperand.name)
{}


Operand::Operand(const double& num):operand(new Double(num)), name(OperandsEnum::NUM)
{}

Operand::Operand(const Double& num)
{

    name = OperandsEnum::NUM;
    operand.reset(new Double(num));
}
Operand::Operand(const Vector& vec)
{
    name = OperandsEnum::VECTOR;
    operand.reset(new Vector(vec));
}
Operand::Operand(const Matrix& matr)
{
    name = OperandsEnum::MATRIX;
    operand.reset(new Matrix(matr));
}

Operand::Operand(Double* num)
{
    name = OperandsEnum::NUM;
    operand.reset(num);
}
Operand::Operand(Vector* vec)
{
    name = OperandsEnum::VECTOR;
    operand.reset(vec);
}
Operand::Operand(Matrix* matrix)
{
    name = OperandsEnum::MATRIX;
    operand.reset(matrix);
}

void Operand::setOperand(OperandsEnum name_, IOperand* operand_)
{
    name = name_;
    operand.reset(operand_);
}
IOperand* Operand::getOperand() const
{
    return operand.get();
}

OperandsEnum Operand::getOperandType() const
{
    return name;
}

Operand& Operand::operator=(const double& otherOperand)
{
    if(name == OperandsEnum::NUM)
    {
        dynamic_cast<Double&>(*getOperand()) = otherOperand;
        return *this;
    }
    name = OperandsEnum::NUM;
    operand.reset(new Double(otherOperand));
    return *this;
}

Operand& Operand::operator=(const Operand& otherOperand)
{
    if(this == std::addressof(otherOperand))
    {
        return *this;
    }
    IOperand* newOp;
    name = otherOperand.name;

    switch (name)
    {
    case OperandsEnum::NUM:
        newOp = new Double(dynamic_cast<const Double&>(*otherOperand.getOperand()));
        break;
    case OperandsEnum::VECTOR:
        newOp = new Vector(dynamic_cast<const Vector&>(*otherOperand.getOperand()));
        break;
    case OperandsEnum::MATRIX:
        newOp = new Matrix(dynamic_cast<const Matrix&>(*otherOperand.getOperand()));
        break;
    }
    operand.reset(newOp);
    return *this;
}
Operand& Operand::operator=(Operand&& otherOperand) noexcept
{
    if(this == std::addressof(otherOperand))
    {
        return *this;
    }
    name = otherOperand.name;
    operand = std::move(otherOperand.operand);
    return *this;
}


Operand& Operand::operator=(Double* num)
{
    if(!num)
    {
        throw std::logic_error("Free pointer");
    }
    if(operand.get() == num)
    {
        return *this;
    }
    name = OperandsEnum::NUM;
    operand.reset(num);
    return *this;
}
Operand& Operand::operator=(Vector* vec)
{
    if(!vec)
    {
        throw std::logic_error("Free pointer");
    }
    if(operand.get() == vec)
    {
        return *this;
    }
    name = OperandsEnum::VECTOR;
    operand.reset(vec);
    return *this;
}
Operand& Operand::operator=(Matrix* matrix)
{
    if(!matrix)
    {
        throw std::logic_error("Free pointer");
    }
    if(operand.get() == matrix)
    {
        return *this;
    }
    name = OperandsEnum::MATRIX;
    operand.reset(matrix);
    return *this;
}

Operand& Operand::operator=(const Double& num)
{
    if(name == OperandsEnum::NUM)
    {
        dynamic_cast<Double&>(*getOperand()) = num;
        return *this;
    }
    name = OperandsEnum::NUM;
    operand.reset(new Double(num));
    return *this;
}
Operand& Operand::operator=(const Vector& vec)
{
    if(name == OperandsEnum::VECTOR)
    {
        dynamic_cast<Vector&>(*getOperand()) = vec;
        return *this;
    }
    name = OperandsEnum::VECTOR;
    operand.reset(new Vector(vec));
    return *this;
}
Operand& Operand::operator=(const Matrix& matrix)
{
    if(name == OperandsEnum::MATRIX)
    {
        dynamic_cast<Matrix&>(*getOperand()) = matrix;
        return *this;
    }
    name = OperandsEnum::MATRIX;
    operand.reset(new Matrix(matrix));
    return *this;
}


bool Operand::operator==(const Operand& num) const
{
    if(name != num.name)
    {
        throw std::logic_error("Operation failed");
    }
    switch (name) {
    case OperandsEnum::NUM:
        return dynamic_cast<const Double&>(*getOperand()) == dynamic_cast<const Double&>(*num.getOperand());
        break;
    case OperandsEnum::VECTOR:
        return dynamic_cast<const Vector&>(*getOperand()) == dynamic_cast<const Vector&>(*num.getOperand());
        break;
    case OperandsEnum::MATRIX:
        return dynamic_cast<const Matrix&>(*getOperand()) == dynamic_cast<const Matrix&>(*num.getOperand());
        break;
    }
}
bool Operand::operator!=(const Operand& num) const
{
    return !(*this == num);
}

bool Operand::operator<(const Double& num) const
{
    if(name != OperandsEnum::NUM)
    {
        throw std::logic_error("Operation failed");
    }

    std::cout << "HERE" << std::endl;
    return dynamic_cast<const Double&>(*getOperand()) < num;
}
bool Operand::operator>(const Double& num) const
{
    if(name != OperandsEnum::NUM)
    {
        throw std::logic_error("Operation failed");
    }
    return dynamic_cast<const Double&>(*getOperand()) > num;
}
bool Operand::operator<=(const Double& num) const
{
    return !(*this > num);
}
bool Operand::operator>=(const Double& num) const
{
    return !(*this < num);
}

Operand& Operand::operator+=(const Operand& otherOperand)
{
    switch (otherOperand.name)
    {
    case OperandsEnum::NUM:
        *this += dynamic_cast<const Double&>(*otherOperand.getOperand());
        break;
    case OperandsEnum::VECTOR:
        *this += dynamic_cast<const Vector&>(*otherOperand.getOperand());
        break;
    case OperandsEnum::MATRIX:
        *this += dynamic_cast<const Matrix&>(*otherOperand.getOperand());
        break;
    }
    return *this;
}
Operand& Operand::operator-=(const Operand& otherOperand)
{
    switch (otherOperand.name)
    {
    case OperandsEnum::NUM:
        *this -= dynamic_cast<const Double&>(*otherOperand.getOperand());
        break;
    case OperandsEnum::VECTOR:
        *this -= dynamic_cast<const Vector&>(*otherOperand.getOperand());
        break;
    case OperandsEnum::MATRIX:
        *this -= dynamic_cast<const Matrix&>(*otherOperand.getOperand());
        break;
    }
    return *this;
}
Operand& Operand::operator*=(const Operand& otherOperand)
{
    switch (otherOperand.name)
    {
    case OperandsEnum::NUM:
        *this *= dynamic_cast<const Double&>(*otherOperand.getOperand());
        break;
    case OperandsEnum::VECTOR:
        *this *= dynamic_cast<const Vector&>(*otherOperand.getOperand());
        break;
    case OperandsEnum::MATRIX:
        *this *= dynamic_cast<const Matrix&>(*otherOperand.getOperand());
        break;
    }
    return *this;
}

Operand& Operand::operator/=(const Operand& otherOperand)
{
    switch (otherOperand.name)
    {
    case OperandsEnum::NUM:
        *this /= dynamic_cast<const Double&>(*otherOperand.getOperand());
        break;
    case OperandsEnum::VECTOR:
        *this /= dynamic_cast<const Vector&>(*otherOperand.getOperand());
        break;
    case OperandsEnum::MATRIX:
        *this /= dynamic_cast<const Matrix&>(*otherOperand.getOperand());
        break;
    }
    return *this;
}

Operand Operand::determine()
{
    if(getOperandType() != OperandsEnum::MATRIX)
    {
        throw std::logic_error("Use matrix to find determinant");
    }
    return dynamic_cast<Matrix&>(*getOperand()).determine();
}

Operand& Operand::operator+=(const Double& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
        dynamic_cast<Double&>(*getOperand()) += otherOperand;
        break;
    case OperandsEnum::VECTOR:
        dynamic_cast<Vector&>(*getOperand()) += otherOperand;
        break;
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) += otherOperand;
        break;
    }
    return *this;
}
Operand& Operand::operator-=(const Double& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
        dynamic_cast<Double&>(*getOperand()) -= otherOperand;
        break;
    case OperandsEnum::VECTOR:
        dynamic_cast<Vector&>(*getOperand()) -= otherOperand;
        break;
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) -= otherOperand;
        break;
    }
    return *this;
}
Operand& Operand::operator*=(const Double& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
        dynamic_cast<Double&>(*getOperand()) *= otherOperand;
        break;
    case OperandsEnum::VECTOR:
        dynamic_cast<Vector&>(*getOperand()) *= otherOperand;
        break;
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) *= otherOperand;
        break;
    }
    return *this;
}
Operand& Operand::operator/=(const Double& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
        dynamic_cast<Double&>(*getOperand()) /= otherOperand;
        break;
    case OperandsEnum::VECTOR:
        dynamic_cast<Vector&>(*getOperand()) /= otherOperand;
        break;
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) /= otherOperand;
        break;
    }
    return *this;
}

Operand& Operand::operator+=(const Vector& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
    {
        Vector* newOp = new Vector(otherOperand);
        *newOp += dynamic_cast<Double&>(*getOperand());
        setOperand(OperandsEnum::VECTOR, newOp);
        break;
    }
    case OperandsEnum::VECTOR:
        dynamic_cast<Vector&>(*getOperand()) += otherOperand;
        break;
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) += otherOperand;
        break;
    }
    return *this;
}
Operand& Operand::operator-=(const Vector& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
    {
        Vector* newOp = new Vector(otherOperand);
        -(*newOp);
        *newOp += dynamic_cast<Double&>(*getOperand());
        setOperand(OperandsEnum::VECTOR, newOp);
        break;
    }
    case OperandsEnum::VECTOR:
        dynamic_cast<Vector&>(*getOperand()) -= otherOperand;
        break;
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) -= otherOperand;
        break;
    }
    return *this;
}
Operand& Operand::operator*=(const Vector& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
    {
        Vector* newOp = new Vector(otherOperand);
        *newOp *= dynamic_cast<Double&>(*getOperand());
        setOperand(OperandsEnum::VECTOR, newOp);
        break;
    }
    case OperandsEnum::VECTOR:
        dynamic_cast<Vector&>(*getOperand()) *= otherOperand;
        break;
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) *= otherOperand;
        break;
    }
    return *this;
}
Operand& Operand::operator/=(const Vector& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
    {
        Vector* newOp = new Vector(otherOperand);
        *newOp /= dynamic_cast<Double&>(*getOperand());
        setOperand(OperandsEnum::VECTOR, newOp);
        break;
    }
    case OperandsEnum::VECTOR:
        dynamic_cast<Vector&>(*getOperand()) /= otherOperand;
        break;
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) /= otherOperand;
        break;
    }
    return *this;
}

Operand& Operand::operator+=(const Matrix& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
    {
        Matrix* newOp = new Matrix(otherOperand);
        *newOp += dynamic_cast<Double&>(*getOperand());
        setOperand(OperandsEnum::MATRIX, newOp);
        break;
    }
    case OperandsEnum::VECTOR:
    {
        Matrix* newOp = new Matrix(otherOperand);
        *newOp += dynamic_cast<Vector&>(*getOperand());
        setOperand(OperandsEnum::MATRIX, newOp);
        break;
    }
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) += otherOperand;
        break;
    }
    return *this;
}
Operand& Operand::operator-=(const Matrix& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
    {
        Matrix* newOp = new Matrix(otherOperand);
        -(*newOp);
        *newOp += dynamic_cast<Double&>(*getOperand());
        setOperand(OperandsEnum::MATRIX, newOp);
        break;
    }
    case OperandsEnum::VECTOR:
    {
        Matrix* newOp = new Matrix(otherOperand);
        -(*newOp);
        *newOp += dynamic_cast<Vector&>(*getOperand());
        setOperand(OperandsEnum::MATRIX, newOp);
        break;
    }
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) -= otherOperand;
        break;
    }
    return *this;
}
Operand& Operand::operator*=(const Matrix& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
    {
        Matrix* newOp = new Matrix(otherOperand);
        *newOp *= dynamic_cast<Double&>(*getOperand());
        setOperand(OperandsEnum::MATRIX, newOp);
        break;
    }
    case OperandsEnum::VECTOR:
    {
        Matrix* newOp = new Matrix(otherOperand);
        *newOp *= dynamic_cast<Vector&>(*getOperand());
        setOperand(OperandsEnum::MATRIX, newOp);
        break;
    }
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) *= otherOperand;
        break;
    }
    return *this;
}
Operand& Operand::operator/=(const Matrix& otherOperand)
{
    switch (name)
    {
    case OperandsEnum::NUM:
    {
        Matrix* newOp = new Matrix(otherOperand);
        *newOp /= dynamic_cast<Double&>(*getOperand());
        setOperand(OperandsEnum::MATRIX, newOp);
        break;
    }
    case OperandsEnum::VECTOR:
    {
        Matrix* newOp = new Matrix(otherOperand);
        *newOp /= dynamic_cast<Vector&>(*getOperand());
        setOperand(OperandsEnum::MATRIX, newOp);
        break;
    }
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()) /= otherOperand;
        break;
    }
    return *this;
}

Operand& Operand::pow(const INum& power)
{
    switch (name) {
    case OperandsEnum::NUM:
        dynamic_cast<Double&>(*getOperand()).pow(power);
        break;
    case OperandsEnum::VECTOR:
        dynamic_cast<Vector&>(*getOperand()).pow(power);
        break;
    case OperandsEnum::MATRIX:
        dynamic_cast<Matrix&>(*getOperand()).pow(power);
        break;
    }
    return *this;;
}
Operand& Operand::sqrt()
{
    getOperand()->sqrt();
    return *this;
}
Operand& Operand::abs()
{
    getOperand()->abs();
    return *this;
}

Operand& Operand::log(double dx)
{
    getOperand()->log(dx);
    return *this;
}

Operand &Operand::lg(double dx)
{
    Operand op = Double(10.0);
    getOperand()->log(dx);
    op.log(dx);
    *this /= op;
    return *this;
}

Operand& Operand::sin()
{
    getOperand()->sin();
    return *this;
}
Operand& Operand::cos()
{
    getOperand()->cos();
    return *this;
}
Operand& Operand::tan(double dx)
{
    getOperand()->tan(dx);
    return *this;
}
Operand& Operand::cot(double dx)
{
    getOperand()->cot(dx);
    return *this;
}

Operand& Operand::asin()
{
    getOperand()->asin();
    return *this;
}
Operand& Operand::acos()
{
    getOperand()->acos();
    return *this;
}
Operand& Operand::atan()
{
    getOperand()->atan();
    return *this;
}
Operand& Operand::acot()
{
    getOperand()->acot();
    return *this;
}

Operand& Operand::sinh()
{
    getOperand()->sinh();
    return *this;
}
Operand& Operand::cosh()
{
    getOperand()->cosh();
    return *this;
}
Operand& Operand::tanh()
{
    getOperand()->tanh();
    return *this;
}
Operand& Operand::coth()
{
    getOperand()->coth();
    return *this;
}

Operand& Operand::asinh()
{
    getOperand()->asinh();
    return *this;
}
Operand& Operand::acosh()
{
    getOperand()->acosh();
    return *this;
}
Operand& Operand::atanh(double dx)
{
    getOperand()->atanh(dx);
    return *this;
}
Operand& Operand::acoth(double dx)
{
    getOperand()->acoth(dx);
    return *this;
}

Operand& Operand::operator-()
{
    switch (name)
    {
    case OperandsEnum::NUM:
        -dynamic_cast<Double&>(*getOperand());
        break;
    case OperandsEnum::VECTOR:
        -dynamic_cast<Vector&>(*getOperand());
        break;
    case OperandsEnum::MATRIX:
        -dynamic_cast<Matrix&>(*getOperand());
        break;
    }
    return *this;
}

std::string Operand::toString() const
{
    return getOperand()->toString();
}

Operand::operator bool() const
{
    if(name != OperandsEnum::NUM)
    {
        throw std::logic_error("This operation can't be completed");
    }
    return dynamic_cast<const Double&>(*getOperand());
}

Operand operator+(const Operand& lhs, const Operand& rhs)
{
    Operand res(lhs);
    res += rhs;
    return res;
}
Operand operator-(const Operand& lhs, const Operand& rhs)
{
    Operand res(lhs);
    res -= rhs;
    return res;
}
Operand operator*(const Operand& lhs, const Operand& rhs)
{
    Operand res(lhs);
    res *= rhs;
    return res;
}
Operand operator/(const Operand& lhs, const Operand& rhs)
{
    Operand res(lhs);
    res /= rhs;
    return res;
}

Operand determine(const Operand& operand)
{
    if(operand.getOperandType() != OperandsEnum::MATRIX)
    {
        throw std::logic_error("Use matrix to find determinant");
    }
    return dynamic_cast<const Matrix&>(*operand.getOperand()).determine();
}

Operand scalarMultiplication(Operand& firstOperand, Operand& secondOperand)
{
    if(firstOperand.getOperandType() != OperandsEnum::VECTOR || secondOperand.getOperandType() != OperandsEnum::VECTOR)
    {
       throw std::logic_error("Use vectors to find scalarMultiplication");
    }
    return scalarMultiplication(dynamic_cast<Vector&>(*firstOperand.getOperand()),
                                dynamic_cast<Vector&>(*secondOperand.getOperand()));
}
Operand vectorMultiplication(Operand& firstOperand, Operand& secondOperand)
{
    if(firstOperand.getOperandType() != OperandsEnum::VECTOR || secondOperand.getOperandType() != OperandsEnum::VECTOR)
    {
       throw std::logic_error("Use vectors to find scalarMultiplication");
    }
    return vectorMultiplication(dynamic_cast<Vector&>(*firstOperand.getOperand()),
                                dynamic_cast<Vector&>(*secondOperand.getOperand()));
}

Operand operator+(const Operand& lhs, const double& rhs)
{
    Operand res(lhs);
    res += Double(rhs);
    return res;
}
Operand operator-(const Operand& lhs, const double& rhs)
{
    Operand res(lhs);
    res -= Double(rhs);
    return res;
}
Operand operator*(const Operand& lhs, const double& rhs)
{
    Operand res(lhs);
    res *= Double(rhs);
    return res;
}
Operand operator/(const Operand& lhs, const double& rhs)
{
    Operand res(lhs);
    res /= Double(rhs);
    return res;
}

Operand operator+(const double& lhs, const Operand& rhs)
{
    Operand res(rhs);
    res += Double(lhs);
    return res;
}
Operand operator-(const double& lhs, const Operand& rhs)
{
    Operand res(rhs);
    res -= Double(lhs);
    return res;
}
Operand operator*(const double& lhs, const Operand& rhs)
{
    Operand res(rhs);
    res *= Double(lhs);
    return res;
}
Operand operator/(const double& lhs, const Operand& rhs)
{
    Operand res(rhs);
    res /= Double(lhs);
    return res;
}

//Operand pow(const Operand& num, const Operand& power)
//{
//    if(power.getOperandType() != OperandsEnum::NUM)
//    {
//        throw std::logic_error("Used uncorrect argument at \'^\'");
//    }
//    Operand res(num);
//    return res.pow(dynamic_cast<Double&>(*power.getOperand()));
//}
//Operand sqrt(const Operand& num)
//{
//    Operand res(num);
//    return res.sqrt();
//}
//Operand abs(const Operand& num)
//{
//    Operand res(num);
//    return res.abs();
//}

//Operand log(const Operand& num)
//{
//    Operand res(num);
//    return res.log();
//}

//Operand sin(const Operand& num)
//{
//    Operand res(num);
//    return res.sin();
//}
//Operand cos(const Operand& num)
//{
//    Operand res(num);
//    return res.cos();
//}
//Operand tan(const Operand& num)
//{
//    Operand res(num);
//    return res.tan();
//}
//Operand cot(const Operand& num)
//{
//    Operand res(num);
//    return res.cot();
//}

//Operand asin(const Operand& num)
//{
//    Operand res(num);
//    return res.asin();
//}
//Operand acos(const Operand& num)
//{
//    Operand res(num);
//    return res.acos();
//}
//Operand atan(const Operand& num)
//{
//    Operand res(num);
//    return res.atan();
//}
//Operand acot(const Operand& num)
//{
//    Operand res(num);
//    return res.acot();
//}

//Operand sinh(const Operand& num)
//{
//    Operand res(num);
//    return res.sinh();
//}
//Operand cosh(const Operand& num)
//{
//    Operand res(num);
//    return res.cosh();
//}
//Operand tanh(const Operand& num)
//{
//    Operand res(num);
//    return res.tanh();
//}
//Operand coth(const Operand& num)
//{
//    Operand res(num);
//    return res.coth();
//}

//Operand asinh(const Operand& num)
//{
//    Operand res(num);
//    return res.asinh();
//}
//Operand acosh(const Operand& num)
//{
//    Operand res(num);
//    return res.acosh();
//}
//Operand atanh(const Operand& num)
//{
//    Operand res(num);
//    return res.atanh();
//}
//Operand acoth(const Operand& num)
//{
//    Operand res(num);
//    return res.acoth();
//}

//Operand atan2(const Operand& lhs, const Operand rhs)
//{
//    if(lhs.getOperandType() != OperandsEnum::NUM || rhs.getOperandType() != OperandsEnum::NUM)
//    {
//        throw std::logic_error("Use numbers");
//    }
//    double lhs_temp = dynamic_cast<const Double&>(*lhs.getOperand());
//    double rhs_temp = dynamic_cast<const Double&>(*rhs.getOperand());
//    Operand res = ::atan2(lhs_temp, rhs_temp);
//    return res;
//}
