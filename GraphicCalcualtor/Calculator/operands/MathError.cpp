#include "Calculator/operands.h"

DomainError::DomainError(double value_): value(value_){}

const char* DomainError::what() const noexcept
{
    return "Out of range in domain of the function";
}
