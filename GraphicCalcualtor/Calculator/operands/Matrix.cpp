#include "Calculator/operands.h"
#include <cmath>


template<typename T>
Wrapper<T>::Wrapper(std::vector<T>& row_): row(row_)
{}
template<typename T>
T& Wrapper<T>::operator[](const std::size_t& i)
{
    return row[i];
}

template<typename T>
ConstWrapper<T>::ConstWrapper(const std::vector<T>& row_): row(row_)
{}
template<typename T>
const T& ConstWrapper<T>::operator[](const std::size_t& i) const
{
    return row[i];
}


template<typename Pred>
IOperand* doMatrixOperation(Matrix& lhs, const IMatrix<Operand>& rhs, Pred pred)
{
    for(std::size_t i = 0; i < lhs.getRow(); i++)
    {
        for(std::size_t j = 0; j < lhs.getColumn(); j++)
        {
            pred(lhs[i][j], rhs[i][j]);
        }
    }
    return &lhs;
}

template<typename Pred>
IOperand* doMatrixNumOperation(Matrix& lhs, const Double& rhs, Pred pred)
{
    for(std::size_t i = 0; i < lhs.getRow(); i++)
    {
        for(std::size_t j = 0; j < lhs.getColumn(); j++)
        {
            pred(lhs[i][j], (rhs));
        }
    }
    return &lhs;
}

template<typename Pred>
IOperand* doMatrixVectorOperation(Matrix& lhs, const Vector& rhs, Pred pred)
{
    for(std::size_t i = 0; i < lhs.getRow(); i++)
    {
        for(std::size_t j = 0; j < lhs.getColumn(); j++)
        {
            pred(lhs[i][j], rhs);
        }
    }
    return &lhs;
}



Matrix::Matrix(const std::size_t& row_, const std::size_t& column_): matr(row_), row(row_), column(column_)
{
    for(std::size_t i = 0; i < row; i++)
    {
        matr[i].resize(column);
    }
}
Matrix::Matrix(const Matrix& otherMatrix):matr(otherMatrix.matr), row(otherMatrix.row), column(otherMatrix.column) {}
Matrix::Matrix(Matrix&& otherMatrix) noexcept: matr(std::move(otherMatrix.matr)), row(otherMatrix.row), column(otherMatrix.column) {}
Matrix::Matrix(const matrix& otherMatrix): matr(otherMatrix), row(otherMatrix.size()), column(otherMatrix[0].size()){}
Matrix::Matrix(matrix&& otherMatrix) noexcept: matr(std::move(otherMatrix)), row(otherMatrix.size()), column(otherMatrix[0].size()){}
IMatrix<Operand>* Matrix::operator=(const Matrix& otherMatrix)
{
    if(this == std::addressof(otherMatrix))
    {
        return this;
    }
    matr = otherMatrix.matr;
    row = otherMatrix.row;
    column = otherMatrix.column;
    return this;
}
IMatrix<Operand>* Matrix::operator=(Matrix&& otherMatrix) noexcept
{
    if(this == std::addressof(otherMatrix))
    {
        return this;
    }
    matr = std::move(otherMatrix.matr);
    row = otherMatrix.row;
    column = otherMatrix.column;
    return this;
}


bool Matrix::operator==(const Matrix& otherMatrix) const
{
    for(std::size_t i = 0; i < getRow(); i++)
        for(std::size_t j = 0; j < getColumn(); j++)
            if(matr[i][j] != otherMatrix[i][j])
            {
                return false;
            }
    return true;
}
bool Matrix::operator!=(const Matrix& otherMatrix) const
{
    return !(*this == otherMatrix);
}

IOperand* Matrix::operator+=(const INum& num)
{
    return doMatrixNumOperation(*this, dynamic_cast<const Double&>(num), [](auto&& lhs, auto&& rhs){return lhs += rhs;});
}

IOperand* Matrix::operator-=(const INum& num)
{
    return doMatrixNumOperation(*this, dynamic_cast<const Double&>(num), [](auto&& lhs, auto&& rhs){return lhs -= rhs;});
}

IOperand* Matrix::operator*=(const INum& num)
{
    return doMatrixNumOperation(*this, dynamic_cast<const Double&>(num), [](auto&& lhs, auto&& rhs){return lhs *= rhs;});
}

IOperand* Matrix::operator/=(const INum& num)
{
    return doMatrixNumOperation(*this, dynamic_cast<const Double&>(num), [](auto&& lhs, auto&& rhs){return lhs /= rhs;});
}

IOperand* Matrix::operator+=(const IVector<Operand>& vector)
{
   const Vector tmp = dynamic_cast<const Vector&>(vector);
   return doMatrixVectorOperation(*this, tmp, [](auto&& lhs, auto&& rhs){return lhs += rhs;});

}
IOperand* Matrix::operator-=(const IVector<Operand>& vector)
{
    const Vector tmp = dynamic_cast<const Vector&>(vector);
    return doMatrixVectorOperation(*this, tmp, [](auto&& lhs, auto&& rhs){return lhs -= rhs;});
}
IOperand* Matrix::operator*=(const IVector<Operand>& vector)
{
    const Vector tmp = dynamic_cast<const Vector&>(vector);
    return doMatrixVectorOperation(*this, tmp, [](auto&& lhs, auto&& rhs){return lhs *= rhs;});
}
IOperand* Matrix::operator/=(const IVector<Operand>& vector)
{
    const Vector tmp = dynamic_cast<const Vector&>(vector);
    return doMatrixVectorOperation(*this, tmp, [](auto&& lhs, auto&& rhs){return lhs /= rhs;});
}

IOperand* Matrix::operator+=(const IMatrix& otherMatrix)
{
    if(otherMatrix.getRow() != row || otherMatrix.getColumn() != column)
    {
        throw std::logic_error("Dimensions of matrices are not equal");
    }
    return doMatrixOperation(*this, otherMatrix, [](auto&& lhs, auto&& rhs){return lhs += rhs;});
}
IOperand* Matrix::operator-=(const IMatrix& otherMatrix)
{
    if(otherMatrix.getRow() != row || otherMatrix.getColumn() != column)
    {
        throw std::logic_error("Dimensions of matrices are not equal");
    }
    return doMatrixOperation(*this, otherMatrix, [](auto&& lhs, auto&& rhs){return lhs -= rhs;});
}
IOperand* Matrix::operator*=(const IMatrix& otherMatrix)
{
    if (column != otherMatrix.getRow())
    {
        throw std::logic_error("Number of columns in first matrix and number of rows in second matrix are not equal ");
    }
    matrix result(row);
    std::size_t otherColumn = otherMatrix.getColumn();
    for (std::size_t i = 0; i < row; i++)
    {
        result[i].resize(otherColumn);
    }

    for (std::size_t i = 0; i < row; i++)
    {
        for (std::size_t j = 0; j < otherColumn; j++)
        {
            for (std::size_t k = 0; k < column; k++)
            {
                result[i][j] += matr[i][k] * otherMatrix[k][j];
            }
        }
    }
    matr= std::move(result);
    column = otherColumn;

    return this;
}
IOperand* Matrix::operator/=(const IMatrix&)
{
    throw std::logic_error("Unknow operation with matrix: /");
    return this;
}

IOperand* Matrix::operator-()
{
    for(std::size_t i = 0; i < row; i++)
    {
        for(std::size_t j = 0; j < column; j++)
        {
            matr[i][j] = -matr[i][j];
        }
    }
    return this;
}

Operand Matrix::determine() const
{
    if (row != column)
    {
        throw std::logic_error("Determinant can be find if matrix are squared");
    }
    return determinantHelper(matr, row);
}

Operand Matrix::determinantHelper(const matrix& tmpMatr, const std::size_t& n) const
{
    switch (n)
    {
    case 1:
    {
        return tmpMatr[0][0];
    }
    case 2:
    {
        Operand res(tmpMatr[0][0]), tmp(tmpMatr[1][0]);
        res *= tmpMatr[1][1];
        tmp *= tmpMatr[0][1];
        res -= tmp;
        return res;
    }
    default:
        if (n < 1) throw std::range_error("Dimension of matrix below zero");

        Operand det(OperandsEnum::NUM, new Double(0)) ;
        int tempIter = 0, s = 1;
        matrix subMatr;

        subMatr.resize(n - 1);
        for (std::size_t i = 0; i < n - 1; i++)
        {
            subMatr[i].resize(n - 1);
        }
        for (std::size_t k = 0; k < n; k++)
        {
            for (std::size_t i = 1; i < n; i++)
            {
                tempIter = 0;
                for (std::size_t j = 0; j < n; j++)
                {
                    if (j == k)
                    {
                        continue;
                    }
                    subMatr[i - 1][tempIter++] = tmpMatr[i][j];
                }
            }
            Operand tmp = tmpMatr[0][k];
            tmp *= Double(::pow(-1, k + 2));
            tmp *= determinantHelper(subMatr, n - 1);
            det += tmp;
            s = -s;
        }
        return det;
    }
}

void Matrix::inverseMatrix()
{
    if (row != column)
    {
        throw std::logic_error("Inverse can\'t be calcuated, becouse it\'s not a square matrix");
    }
    Double dx = 0.00001;

    int n = row;
    matrix resultMatr(n);
    for (int i = 0; i < n; i++)
    {
        resultMatr[i].resize(n * 2);
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            resultMatr[i][j] = matr[i][j];
            resultMatr[i][i + n] = Double(1);
        }
    }
    if (resultMatr[0][0] > -dx && resultMatr[0][0] < dx)
    {
        bool swap = false;
        for (int i = 0; i < n; i++)
        {
            if ( resultMatr[i][0] < -dx   || resultMatr[i][0] > dx)
            {
                swap = true;
                std::swap(resultMatr[0], resultMatr[i]);
                std::swap(matr[0], matr[i]);
                break;
            }
        }
        if (!swap)
        {
            throw std::logic_error("Inverse can\'t be calcuated, becouse it\'s have a null-column");
        }
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < 2 * n; i++)
        {
            resultMatr[k][i] /= matr[k][k];
        }
        for (int i = k + 1; i < n; i++)
        {
              Operand K = resultMatr[i][k] / resultMatr[k][k];
            for (int j = 0; j < 2 * n; j++)
            {
                resultMatr[i][j] = resultMatr[i][j] - resultMatr[k][j] * K;
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                matr[i][j] = resultMatr[i][j];
            }
        }
    }
    for (int k = n - 1; k > -1; k--)
    {
        for (int i = 2 * n - 1; i > -1; i--)
        {
            resultMatr[k][i] = resultMatr[k][i] / matr[k][k];
        }
        for (int i = k - 1; i > -1; i--)
        {
            Operand K = resultMatr[i][k] / resultMatr[k][k];
            for (int j = 2 * n - 1; j > -1; j--)
            {
                resultMatr[i][j] = resultMatr[i][j] - resultMatr[k][j] * K;
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matr[i][j] = resultMatr[i][j + n];
        }
    }
}

void Matrix::transpose()
{
    matrix transpMatr(column);
    for (std::size_t i = 0; i < column; i++)
    {
        transpMatr[i].resize(row);
    }
    for (std::size_t i = 0; i < column; i++)
    {
        for (std::size_t j = 0; j < row; j++)
        {
            transpMatr[i][j] = matr[j][i];
        }
    }
    matr = std::move(transpMatr);
    std::swap(row, column);
}

Matrix& Matrix::pow(const INum& power)
{
    long power_ = (double)dynamic_cast<const Double&>(power);
    if(power_ < 0)
    {
        inverseMatrix();
        power_ = -power_;
    }
    for(std::size_t i = 1; i < power_; i++)
    {
        *this *= *this;
    }
    return *this;
}
Matrix& Matrix::sqrt()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].sqrt();
    return *this;
}
Matrix& Matrix::abs()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].abs();
    return *this;
}

Matrix& Matrix::log(double dx)
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].log(dx);
    return *this;
}

Matrix& Matrix::sin()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].sin();
    return *this;
}
Matrix& Matrix::cos()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].cos();
    return *this;
}
Matrix& Matrix::tan(double dx)
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].tan(dx);
    return *this;
}
Matrix& Matrix::cot(double dx)
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].cot(dx);
    return *this;
}

Matrix& Matrix::asin()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].asin();
    return *this;
}
Matrix& Matrix::acos()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].acos();
    return *this;
}
Matrix& Matrix::atan()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].atan();
    return *this;
}
Matrix& Matrix::acot()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].acot();
    return *this;
}

Matrix& Matrix::sinh()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].sinh();
    return *this;
}
Matrix& Matrix::cosh()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].cosh();
    return *this;
}
Matrix& Matrix::tanh()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].tanh();
    return *this;
}
Matrix& Matrix::coth()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].coth();
    return *this;
}

Matrix& Matrix::asinh()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].asinh();
    return *this;
}
Matrix& Matrix::acosh()
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].acosh();
    return *this;
}
Matrix& Matrix::atanh(double dx)
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].atanh(dx);
    return *this;
}
Matrix& Matrix::acoth(double dx)
{
    for(std::size_t i = 0; i < row; i++)
        for(std::size_t j = 0; j < column; j++)
            matr[i][j].acoth(dx);
    return *this;
}

Wrapper<Operand> Matrix::operator[](const std::size_t& i)
{
    return Wrapper<Operand>(matr[i]);
}
ConstWrapper<Operand> Matrix::operator[](const std::size_t& i) const
{
    return ConstWrapper<Operand>(matr[i]);
}

std::size_t Matrix::getRow() const
{
    return row;
}

std::size_t Matrix::getColumn() const
{
    return column;
}

std::string Matrix::toString() const
{
    std::string str = "[";
    for(std::size_t i = 0; i < row; i++)
    {
        str.push_back('[');
        for(std::size_t j = 0; j < column; j++)
        {
            str += matr[i][j].toString() + ", ";
        }
        str.pop_back();
        str.back() = ']';
    }
    str.push_back(']');
    return str;
}

Matrix operator+(const Matrix& lhs, const Double& rhs)
{
    Matrix res(lhs);
    res += rhs;
    return res;
}
Matrix operator-(const Matrix& lhs, const Double& rhs)
{
    Matrix res(lhs);
    res -= rhs;
    return res;
}
Matrix operator*(const Matrix& lhs, const Double& rhs)
{
    Matrix res(lhs);
    res *= rhs;
    return res;
}
Matrix operator/(const Matrix& lhs, const Double& rhs)
{
    Matrix res(lhs);
    res /= rhs;
    return res;
}

Matrix operator+(const Double& lhs, const Matrix& rhs)
{
    Matrix res(rhs);
    res += lhs;
    return res;
}
Matrix operator-(const Double& lhs, const Matrix& rhs)
{
    Matrix res(rhs);
    res -= lhs;
    return res;
}
Matrix operator*(const Double& lhs, const Matrix& rhs)
{
    Matrix res(rhs);
    res *= lhs;
    return res;
}
Matrix operator/(const Double& lhs, const Matrix& rhs)
{
    Matrix res(rhs);
    res /= lhs;
    return res;
}

Matrix operator+(const Matrix& lhs, const Vector& rhs)
{
    Matrix res(lhs);
    res += rhs;
    return res;
}
Matrix operator-(const Matrix& lhs, const Vector& rhs)
{
    Matrix res(lhs);
    res -= rhs;
    return res;
}
Matrix operator*(const Matrix& lhs, const Vector& rhs)
{
    Matrix res(lhs);
    res *= rhs;
    return res;
}
Matrix operator/(const Matrix& lhs, const Vector& rhs)
{
    Matrix res(lhs);
    res /= rhs;
    return res;
}

Matrix operator+(const Vector& lhs, const Matrix& rhs)
{
    Matrix res(rhs);
    res += lhs;
    return res;
}

Matrix operator-(const Vector& lhs, const Matrix& rhs)
{
    Matrix res(rhs);
    res -= lhs;
    return res;
}
Matrix operator*(const Vector& lhs, const Matrix& rhs)
{
    Matrix res(rhs);
    res *= lhs;
    return res;
}
Matrix operator/(const Vector& lhs, const Matrix& rhs)
{
    Matrix res(rhs);
    res /= lhs;
    return res;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
    Matrix res(lhs);
    res += rhs;
    return res;
}
Matrix operator-(const Matrix& lhs, const Matrix& rhs)
{
    Matrix res(lhs);
    res -= rhs;
    return res;
}
Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    Matrix res(lhs);
    res *= rhs;
    return res;
}
Matrix operator/(const Matrix& lhs, const Matrix& rhs)
{
    Matrix res(lhs);
    res /= rhs;
    return res;
}


Operand determine(const Matrix& matr)
{
    return matr.determine();
}

//Matrix pow(const Matrix& vec, const Double& power)
//{
//    Matrix res(vec);
//    return res.pow(power);
//}
//Matrix sqrt(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.sqrt();
//}
//Matrix abs(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.abs();
//}
//Matrix log(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.log();
//}

//Matrix sin(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.sin();
//}
//Matrix cos(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.cos();
//}
//Matrix tan(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.tan();
//}
//Matrix cot(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.cot();
//}

//Matrix asin(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.asin();
//}
//Matrix acos(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.acos();
//}
//Matrix atan(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.atan();
//}
//Matrix acot(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.acot();
//}

//Matrix sinh(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.sinh();
//}
//Matrix cosh(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.cosh();
//}
//Matrix tanh(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.tanh();
//}
//Matrix coth(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.coth();
//}

//Matrix asinh(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.asinh();
//}
//Matrix acosh(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.acosh();
//}
//Matrix atanh(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.atanh();
//}
//Matrix acoth(const Matrix& vec)
//{
//    Matrix res(vec);
//    return res.acoth();
//}
