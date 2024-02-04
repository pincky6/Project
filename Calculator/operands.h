#ifndef OPERANDS_H
#define OPERANDS_H
#include <string>
#include <vector>
#include <iostream>
#include <memory>

enum class OperandsEnum
{
    NUM, VECTOR, MATRIX
};


struct DomainError: std::exception
{
    DomainError(double);
    const char* what() const noexcept override;
    virtual ~DomainError(){}
    double value;
};


class IOperand
{
public:
    /*  virtual IOperand* operator-=(const IOperand*) = 0;
    virtual IOperand* operator+=(const IOperand*) = 0;
    virtual IOperand* operator*=(const IOperand*) = 0;
    virtual IOperand* operator/=(const IOperand*) = 0;*/
    virtual IOperand& sqrt() = 0;
    virtual IOperand& abs() = 0;

    virtual IOperand& log(double dx = 0) = 0;

    virtual IOperand& sin() = 0;
    virtual IOperand& cos() = 0;
    virtual IOperand& tan(double dx = 0) = 0;
    virtual IOperand& cot(double dx = 0) = 0;

    virtual IOperand& asin() = 0;
    virtual IOperand& acos() = 0;
    virtual IOperand& atan() = 0;
    virtual IOperand& acot() = 0;

    virtual IOperand& sinh() = 0;
    virtual IOperand& cosh() = 0;
    virtual IOperand& tanh() = 0;
    virtual IOperand& coth() = 0;

    virtual IOperand& asinh() = 0;
    virtual IOperand& acosh() = 0;
    virtual IOperand& atanh(double dx = 0) = 0;
    virtual IOperand& acoth(double dx = 0) = 0;

    virtual std::string toString() const = 0;
    virtual ~IOperand(){}
};

class Operand;

class INum: public IOperand
{
public:

    virtual bool operator==(const INum&) const = 0;
    virtual bool operator!=(const INum&) const = 0;

    virtual bool operator<(const INum&) const = 0;
    virtual bool operator>(const INum&) const = 0;
    virtual bool operator<=(const INum&) const = 0;
    virtual bool operator>=(const INum&) const = 0;

    virtual IOperand* operator+=(const INum&) = 0;
    virtual IOperand* operator-=(const INum&) = 0;
    virtual IOperand* operator*=(const INum&) = 0;
    virtual IOperand* operator/=(const INum&) = 0;

    virtual INum& operator++() = 0;
    virtual INum& operator--() = 0;

    virtual INum& operator-() = 0;
    virtual operator bool() const= 0;

    virtual INum& pow(const INum&) = 0;
    virtual INum& sqrt() override = 0;
    virtual INum& abs() override = 0;

    virtual INum& log(double) override = 0;

    virtual INum& sin() override = 0;
    virtual INum& cos() override = 0;
    virtual INum& tan(double dx = 0) override = 0;
    virtual INum& cot(double dx = 0) override = 0;

    virtual INum& asin() override = 0;
    virtual INum& acos() override = 0;
    virtual INum& atan() override = 0;
    virtual INum& acot() override = 0;

    virtual INum& sinh() override = 0;
    virtual INum& cosh() override = 0;
    virtual INum& tanh() override = 0;
    virtual INum& coth() override = 0;

    virtual std::string toString() const override = 0;

    virtual ~INum(){}
};

class Double: public INum
{
public:
    Double(): value(0){}
    Double(double);


    bool operator==(const INum&) const override;
    bool operator!=(const INum&) const override;

    bool operator<(const INum&) const override;
    bool operator>(const INum&) const override;
    bool operator<=(const INum&) const override;
    bool operator>=(const INum&) const override;

    IOperand* operator+=(const INum&) override;
    IOperand* operator-=(const INum&) override;
    IOperand* operator*=(const INum&) override;
    IOperand* operator/=(const INum&) override;

    Double& operator++() override;
    Double operator++(int);
    Double& operator--() override;
    Double operator--(int);

    Double& operator-() override;

    operator bool() const override;
    operator double() const;

    Double& pow(const INum&) override;
    Double& sqrt() override;
    Double& abs() override;

    Double& log(double dx = 0) override;

    Double& sin() override;
    Double& cos() override;
    Double& tan(double dx = 0) override;
    Double& cot(double dx = 0) override;

    Double& asin() override;
    Double& acos() override;
    Double& atan() override;
    Double& acot() override ;

    Double& sinh() override;
    Double& cosh() override;
    Double& tanh() override;
    Double& coth() override;

    Double& asinh() override;
    Double& acosh() override;
    Double& atanh(double dx = 0) override;
    Double& acoth(double dx = 0) override;

    std::string toString() const override;

    ~Double(){}
private:
    double value;
};

Double operator+(const Double& rhs, const Double& lhs);
Double operator-(const Double& rhs, const Double& lhs);
Double operator*(const Double& rhs, const Double& lhs);
Double operator/(const Double& rhs, const Double& lhs);



//Double pow(const Double& num, const Double& power);
//Double sqrt(const Double& num);
//Double abs(const Double& num);

//Double log(const Double& num);

//Double sin(const Double& num);
//Double cos(const Double& num);
//Double tan(const Double& num);
//Double cot(const Double& num);

//Double asin(const Double& num);
//Double acos(const Double& num);
//Double atan(const Double& num);
//Double acot(const Double& num);

//Double sinh(const Double& num);
//Double cosh(const Double& num);
//Double tanh(const Double& num);
//Double coth(const Double& num);

//Double asinh(const Double& num);
//Double acosh(const Double& num);
//Double atanh(const Double& num);
//Double acoth(const Double& num);


template <typename T>
class IVector: public IOperand
{
public:
    virtual IOperand* operator+=(const IVector&) = 0;
    virtual IOperand* operator-=(const IVector&) = 0;

    virtual IOperand* operator+=(const INum&) = 0;
    virtual IOperand* operator-=(const INum&) = 0;
    virtual IOperand* operator*=(const INum&) = 0;
    virtual IOperand* operator/=(const INum&) = 0;

    virtual IOperand* operator-() = 0;

    virtual T& operator[](const std::size_t&) = 0;
    virtual const T& operator[](const std::size_t&) const = 0;

    virtual IVector& pow(const INum&) = 0;
    virtual IVector& sqrt() override = 0;
    virtual IVector& abs() override = 0;

    virtual IVector& log(double dx = 0) override = 0;

    virtual IVector& sin() override = 0;
    virtual IVector& cos() override = 0;
    virtual IVector& tan(double dx = 0) override = 0;
    virtual IVector& cot(double dx = 0) override = 0;

    virtual IVector& asin() override = 0;
    virtual IVector& acos() override = 0;
    virtual IVector& atan() override = 0;
    virtual IVector& acot() override = 0;

    virtual IVector& sinh() override = 0;
    virtual IVector& cosh() override = 0;
    virtual IVector& tanh() override = 0;
    virtual IVector& coth() override = 0;

    virtual std::size_t size() const = 0;

    virtual std::string toString() const override= 0;

    virtual ~IVector(){}
};


class Vector: public IVector<Operand>
{
public:

    Vector();
    Vector(const std::vector<Operand>&);
    Vector(std::vector<Operand>&&) noexcept;
    Vector(const Vector&);
    Vector(Vector&&) noexcept;
    Vector(const std::size_t);
    IOperand* operator=(const Vector&);
    IOperand* operator=(Vector&&) noexcept;

    bool operator==(const Vector&) const;
    bool operator!=(const Vector&) const;

    IOperand* operator+=(const IVector&) override;
    IOperand* operator-=(const IVector&) override;
    void operator*=(const IVector&);
    void operator/=(const IVector&);

    IOperand* operator+=(const INum&) override;
    IOperand* operator-=(const INum&) override;
    IOperand* operator*=(const INum&) override;
    IOperand* operator/=(const INum&) override;

    IOperand* operator-() override;

    Vector& pow(const INum&) override;
    Vector& sqrt() override;
    Vector& abs() override;

    Vector& log(double dx = 0) override;

    Vector& sin() override;
    Vector& cos() override;
    Vector& tan(double dx = 0) override;
    Vector& cot(double dx = 0) override;

    Vector& asin() override;
    Vector& acos() override;
    Vector& atan() override;
    Vector& acot() override ;

    Vector& sinh() override;
    Vector& cosh() override;
    Vector& tanh() override;
    Vector& coth() override;

    Vector& asinh() override;
    Vector& acosh() override;
    Vector& atanh(double dx = 0) override;
    Vector& acoth(double dx = 0) override;

    Operand& operator[](const std::size_t& i) override;
    const Operand& operator[](const std::size_t& i) const override;

    std::size_t size() const override;
    std::string toString() const override;


private:
    std::vector<Operand> vector;
};

Vector operator+(const Vector&, const Double&);
Vector operator-(const Vector&, const Double&);
Vector operator*(const Vector&, const Double&);
Vector operator/(const Vector&, const Double&);

Vector operator+(const Double&, const Vector&);
Vector operator-(const Double&, const Vector&);
Vector operator*(const Double&, const Vector&);
Vector operator/(const Double&, const Vector&);

Vector operator+(const Vector&, const Vector&);
Vector operator-(const Vector&, const Vector&);
Vector operator*(const Vector&, const Vector&);
Vector operator/(const Vector&, const Vector&);

Operand scalarMultiplication(Vector&, Vector&);
Operand vectorMultiplication(Vector&, Vector&);

//Vector pow(const Vector& num, const Double& power);
//Vector sqrt(const Vector& num);
//Vector abs(const Vector& num);

//Vector log(const Vector& num);

//Vector sin(const Vector& num);
//Vector cos(const Vector& num);
//Vector tan(const Vector& num);
//Vector cot(const Vector& num);

//Vector asin(const Vector& num);
//Vector acos(const Vector& num);
//Vector atan(const Vector& num);
//Vector acot(const Vector& num);

//Vector sinh(const Vector& num);
//Vector cosh(const Vector& num);
//Vector tanh(const Vector& num);
//Vector coth(const Vector& num);

//Vector asinh(const Vector& num);
//Vector acosh(const Vector& num);
//Vector atanh(const Vector& num);
//Vector acoth(const Vector& num);

template<typename T>
struct Wrapper
{
    Wrapper(std::vector<T>& row_);
    T& operator[](const std::size_t& i);
private:
    std::vector<T>& row;
};

template<typename T>
struct ConstWrapper
{
    ConstWrapper(const std::vector<T>& row_);
    const T& operator[](const std::size_t& i) const;
private:
    const std::vector<T>& row;
};

template<typename T>
class IMatrix: public IOperand
{
public:
    virtual IOperand* operator+=(const INum&) = 0;
    virtual IOperand* operator-=(const INum&) = 0;
    virtual IOperand* operator*=(const INum&) = 0;
    virtual IOperand* operator/=(const INum&) = 0;

    virtual IOperand* operator+=(const IVector<T>&) = 0;
    virtual IOperand* operator-=(const IVector<T>&) = 0;
    virtual IOperand* operator*=(const IVector<T>&) = 0;
    virtual IOperand* operator/=(const IVector<T>&) = 0;

    virtual IOperand* operator+=(const IMatrix&) = 0;
    virtual IOperand* operator-=(const IMatrix&) = 0;
    virtual IOperand* operator*=(const IMatrix&) = 0;
    virtual IOperand* operator/=(const IMatrix&) = 0;

    virtual IOperand* operator-() = 0;

    virtual T determine() const = 0;
    virtual void inverseMatrix() = 0;
    virtual void transpose() = 0;

    virtual IMatrix& pow(const INum&) = 0;
    virtual IMatrix& sqrt() override = 0;
    virtual IMatrix& abs() override = 0;

    virtual IMatrix& log(double dx = 0) override = 0;

    virtual IMatrix& sin() override = 0;
    virtual IMatrix& cos() override = 0;
    virtual IMatrix& tan(double dx = 0) override = 0;
    virtual IMatrix& cot(double dx = 0) override = 0;

    virtual IMatrix& asin() override = 0;
    virtual IMatrix& acos() override = 0;
    virtual IMatrix& atan() override = 0;
    virtual IMatrix& acot() override = 0;

    virtual IMatrix& sinh() override = 0;
    virtual IMatrix& cosh() override = 0;
    virtual IMatrix& tanh() override = 0;
    virtual IMatrix& coth() override = 0;

    virtual Wrapper<T> operator[](const std::size_t&) = 0;
    virtual ConstWrapper<T> operator[](const std::size_t&) const = 0;


    virtual std::size_t getRow() const = 0;
    virtual std::size_t getColumn() const = 0;

    std::string toString() const override = 0;

    virtual ~IMatrix(){}
};

class Matrix: public IMatrix<Operand>
{
    using matrix = std::vector<std::vector<Operand> >;
public:
    Matrix(const std::size_t&, const std::size_t&);
    Matrix(const Matrix&);
    Matrix(Matrix&&) noexcept;
    Matrix(const matrix&);
    Matrix(matrix&&) noexcept;
    IMatrix<Operand>* operator=(const Matrix&);
    IMatrix<Operand>* operator=(Matrix&&) noexcept;

    bool operator==(const Matrix&) const;
    bool operator!=(const Matrix&) const;

    IOperand* operator+=(const INum&) override;
    IOperand* operator-=(const INum&) override;
    IOperand* operator*=(const INum&) override;
    IOperand* operator/=(const INum&) override;

    IOperand* operator+=(const IVector<Operand>&) override;
    IOperand* operator-=(const IVector<Operand>&) override;
    IOperand* operator*=(const IVector<Operand>&) override;
    IOperand* operator/=(const IVector<Operand>&) override;


    IOperand* operator+=(const IMatrix&) override;
    IOperand* operator-=(const IMatrix&) override;
    IOperand* operator*=(const IMatrix&) override;
    IOperand* operator/=(const IMatrix&) override;

    IOperand* operator-() override;

    Operand determine() const override;
    void inverseMatrix() override;
    void transpose() override;

    Matrix& pow(const INum&) override;
    Matrix& sqrt() override;
    Matrix& abs() override;

    Matrix& log(double dx = 0) override;

    Matrix& sin() override;
    Matrix& cos() override;
    Matrix& tan(double dx = 0) override;
    Matrix& cot(double dx = 0) override;

    Matrix& asin() override;
    Matrix& acos() override;
    Matrix& atan() override;
    Matrix& acot() override ;

    Matrix& sinh() override;
    Matrix& cosh() override;
    Matrix& tanh() override;
    Matrix& coth() override;

    Matrix& asinh() override;
    Matrix& acosh() override;
    Matrix& atanh(double dx = 0) override;
    Matrix& acoth(double dx = 0) override;

    Wrapper<Operand> operator[](const std::size_t&) override;
    ConstWrapper<Operand> operator[](const std::size_t&) const override;

    std::size_t getRow() const override;
    std::size_t getColumn() const override;

    std::string toString() const override;
private:
    Operand determinantHelper(const matrix&, const std::size_t&) const;
private:
    matrix matr;
    std::size_t row;
    std::size_t column;
};

Matrix operator+(const Matrix&, const Double&);
Matrix operator-(const Matrix&, const Double&);
Matrix operator*(const Matrix&, const Double&);
Matrix operator/(const Matrix&, const Double&);

Matrix operator+(const Double&, const Matrix&);
Matrix operator-(const Double&, const Matrix&);
Matrix operator*(const Double&, const Matrix&);
Matrix operator/(const Double&, const Matrix&);

Matrix operator+(const Matrix&, const Vector&);
Matrix operator-(const Matrix&, const Vector&);
Matrix operator*(const Matrix&, const Vector&);
Matrix operator/(const Matrix&, const Vector&);

Matrix operator+(const Vector&, const Matrix&);
Matrix operator-(const Vector&, const Matrix&);
Matrix operator*(const Vector&, const Matrix&);
Matrix operator/(const Vector&, const Matrix&);

Matrix operator+(const Matrix&, const Matrix&);
Matrix operator-(const Matrix&, const Matrix&);
Matrix operator*(const Matrix&, const Matrix&);
Matrix operator/(const Matrix&, const Matrix&);

Operand determine(const Matrix&);

//Matrix pow(const Matrix& num, const Double& power);
//Matrix sqrt(const Matrix& num);
//Matrix abs(const Matrix& num);

//Matrix log(const Matrix& num);

//Matrix sin(const Matrix& num);
//Matrix cos(const Matrix& num);
//Matrix tan(const Matrix& num);
//Matrix cot(const Matrix& num);

//Matrix asin(const Matrix& num);
//Matrix acos(const Matrix& num);
//Matrix atan(const Matrix& num);
//Matrix acot(const Matrix& num);

//Matrix sinh(const Matrix& num);
//Matrix cosh(const Matrix& num);
//Matrix tanh(const Matrix& num);
//Matrix coth(const Matrix& num);

//Matrix asinh(const Matrix& num);
//Matrix acosh(const Matrix& num);
//Matrix atanh(const Matrix& num);
//Matrix acoth(const Matrix& num);

class Operand
{
public:
    Operand();

    Operand(OperandsEnum, IOperand*);
    Operand(const Operand&);
    Operand(Operand&&) noexcept;

    Operand(const double&);
    Operand(const Double&);
    Operand(const Vector&);
    Operand(const Matrix&);

    Operand(Double*);
    Operand(Vector*);
    Operand(Matrix*);

    void setOperand(OperandsEnum, IOperand*);
    IOperand* getOperand() const;

    OperandsEnum getOperandType() const;


    Operand& operator=(const Operand&);
    Operand& operator=(Operand&&) noexcept;

    Operand& operator=(const double&);
    Operand& operator=(Double*);
    Operand& operator=(Vector*);
    Operand& operator=(Matrix*);

    Operand& operator=(const Double&);
    Operand& operator=(const Vector&);
    Operand& operator=(const Matrix&);

    bool operator==(const Operand&) const;
    bool operator!=(const Operand&) const;

    bool operator<(const Double&) const;
    bool operator>(const Double&) const;
    bool operator<=(const Double&) const;
    bool operator>=(const Double&) const;

    Operand& operator+=(const Operand&);
    Operand& operator-=(const Operand&);
    Operand& operator*=(const Operand&);
    Operand& operator/=(const Operand&);

    Operand& operator+=(const Double&);
    Operand& operator-=(const Double&);
    Operand& operator*=(const Double&);
    Operand& operator/=(const Double&);

    Operand& operator+=(const Vector&);
    Operand& operator-=(const Vector&);
    Operand& operator*=(const Vector&);
    Operand& operator/=(const Vector&);

    Operand& operator+=(const Matrix&);
    Operand& operator-=(const Matrix&);
    Operand& operator*=(const Matrix&);
    Operand& operator/=(const Matrix&);

    Operand determine();

    Operand& pow(const INum&);
    Operand& sqrt();
    Operand& abs();

    Operand& log(double dx = 0);
    Operand& lg(double dx = 0);

    Operand& sin();
    Operand& cos();
    Operand& tan(double dx = 0);
    Operand& cot(double dx = 0);

    Operand& asin();
    Operand& acos();
    Operand& atan();
    Operand& acot();

    Operand& sinh();
    Operand& cosh();
    Operand& tanh();
    Operand& coth();

    Operand& asinh();
    Operand& acosh();
    Operand& atanh(double dx = 0);
    Operand& acoth(double dx = 0);

    Operand& operator-();
    std::string toString() const;

    operator bool() const;

private:
    std::unique_ptr<IOperand> operand;
    OperandsEnum name;
};


Operand operator+(const Operand&, const Operand&);
Operand operator-(const Operand&, const Operand&);
Operand operator*(const Operand&, const Operand&);
Operand operator/(const Operand&, const Operand&);

Operand operator+(const Operand&, const double&);
Operand operator-(const Operand&, const double&);
Operand operator*(const Operand&, const double&);
Operand operator/(const Operand&, const double&);

Operand operator+(const double&, const Operand&);
Operand operator-(const double&, const Operand&);
Operand operator*(const double&, const Operand&);
Operand operator/(const double&, const Operand&);

Operand determine(const Operand&);

Operand scalarMultiplication(Operand&, Operand&);
Operand vectorMultiplication(Operand&, Operand&);

//Operand pow(const Operand&, const Operand&);
//Operand sqrt(const Operand&);
//Operand abs(const Operand&);

//Operand log(const Operand&);

//Operand sin(const Operand&);
//Operand cos(const Operand&);
//Operand tan(const Operand&);
//Operand cot(const Operand&);

//Operand asin(const Operand&);
//Operand acos(const Operand&);
//Operand atan(const Operand&);
//Operand acot(const Operand&);

//Operand sinh(const Operand&);
//Operand cosh(const Operand&);
//Operand tanh(const Operand&);
//Operand coth(const Operand&);

//Operand asinh(const Operand&);
//Operand acosh(const Operand&);
//Operand atanh(const Operand&);
//Operand acoth(const Operand&);

//Operand atan2(const Operand& lhs, const Operand rhs);

template<OperandsEnum operandsEnum, std::enable_if_t<(operandsEnum == OperandsEnum::VECTOR), bool> = true>
Vector& operandCast(IOperand* op)
{
    return dynamic_cast<Vector&>(*op);
}

template<OperandsEnum operandsEnum, std::enable_if_t<(operandsEnum == OperandsEnum::MATRIX), bool> = true>
Matrix& operandCast(IOperand* op)
{
    return dynamic_cast<Matrix&>(*op);
}
template<OperandsEnum operandsEnum, std::enable_if_t<(operandsEnum == OperandsEnum::NUM), bool> = true>
Double& operandCast(IOperand* op)
{
    return dynamic_cast<Double&>(*op);
}


#endif // OPERANDS_H
