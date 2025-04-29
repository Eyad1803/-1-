// Email: e-abo@hotmail.com

#include "SquareMat.hpp"
#include <stdexcept>
#include <cmath>

namespace mat_ops {

// Helper methods
void SquareMat::allocateMemory() {
    if (size > 0) {
        data = new double*[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = new double[size]();  // Initialize with zeros
        }
    } else {
        data = nullptr;
    }
}

void SquareMat::deallocateMemory() {
    if (data) {
        for (size_t i = 0; i < size; ++i) {
            delete[] data[i];
        }
        delete[] data;
        data = nullptr;
    }
}

void SquareMat::copyFrom(const SquareMat& other) {
    size = other.size;
    allocateMemory();
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

// Constructors and Destructor
SquareMat::SquareMat(size_t size) : size(size), data(nullptr) {
    allocateMemory();
}

SquareMat::SquareMat(size_t size, double initialValue) : size(size), data(nullptr) {
    allocateMemory();
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            data[i][j] = initialValue;
        }
    }
}

SquareMat::SquareMat(const SquareMat& other) : size(0), data(nullptr) {
    copyFrom(other);
}

SquareMat::~SquareMat() {
    deallocateMemory();
}

// Assignment operator
SquareMat& SquareMat::operator=(const SquareMat& other) {
    if (this != &other) {
        deallocateMemory();
        copyFrom(other);
    }
    return *this;
}

// Helper for comparison operators
double SquareMat::getSum() const {
    double sum = 0.0;
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            sum += data[i][j];
        }
    }
    return sum;
}

// Arithmetic operators
SquareMat SquareMat::operator+(const SquareMat& other) const {
    if (size != other.size) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    
    SquareMat result(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

SquareMat SquareMat::operator-(const SquareMat& other) const {
    if (size != other.size) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    }
    
    SquareMat result(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

SquareMat SquareMat::operator-() const {
    SquareMat result(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            result.data[i][j] = -data[i][j];
        }
    }
    return result;
}

SquareMat SquareMat::operator*(const SquareMat& other) const {
    if (size != other.size) {
        throw std::invalid_argument("Matrix dimensions must match for multiplication");
    }
    
    SquareMat result(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            result.data[i][j] = 0;
            for (size_t k = 0; k < size; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

SquareMat SquareMat::operator*(double scalar) const {
    SquareMat result(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            result.data[i][j] = data[i][j] * scalar;
        }
    }
    return result;
}

SquareMat operator*(double scalar, const SquareMat& mat) {
    return mat * scalar;  // Reuse the matrix * scalar implementation
}

SquareMat SquareMat::operator%(const SquareMat& other) const {
    if (size != other.size) {
        throw std::invalid_argument("Matrix dimensions must match for element-wise multiplication");
    }
    
    SquareMat result(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            result.data[i][j] = data[i][j] * other.data[i][j];  // Element-wise multiplication
        }
    }
    return result;
}

SquareMat SquareMat::operator%(int scalar) const {
    SquareMat result(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            result.data[i][j] = static_cast<double>(static_cast<int>(data[i][j]) % scalar);
        }
    }
    return result;
}

SquareMat SquareMat::operator/(double scalar) const {
    if (scalar == 0) {
        throw std::invalid_argument("Division by zero");
    }
    
    SquareMat result(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            result.data[i][j] = data[i][j] / scalar;
        }
    }
    return result;
}

// Helper for determinant calculation
double SquareMat::calculateDeterminant() const {
    if (size == 0) {
        return 0;
    }
    if (size == 1) {
        return data[0][0];
    }
    if (size == 2) {
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    }
    
    // For larger matrices, use cofactor expansion
    double det = 0;
    for (size_t j = 0; j < size; ++j) {
        // Create submatrix by excluding first row and column j
        SquareMat submat(size - 1);
        for (size_t i = 1; i < size; ++i) {
            for (size_t k = 0, l = 0; k < size; ++k) {
                if (k != j) {
                    submat.data[i-1][l++] = data[i][k];
                }
            }
        }
        
        // Add or subtract the cofactor
        double cofactor = data[0][j] * submat.calculateDeterminant();
        det += (j % 2 == 0) ? cofactor : -cofactor;
    }
    
    return det;
}

SquareMat SquareMat::operator^(int power) const {
    if (power < 0) {
        throw std::invalid_argument("Negative power not supported");
    }
    if (size == 0) {
        throw std::invalid_argument("Cannot raise empty matrix to a power");
    }
    
    if (power == 0) {
        // Return identity matrix
        SquareMat result(size);
        for (size_t i = 0; i < size; ++i) {
            result.data[i][i] = 1.0;
        }
        return result;
    }
    
    if (power == 1) {
        return *this;
    }
    
    // Use exponentiation by squaring for efficiency
    if (power % 2 == 0) {
        SquareMat half = (*this) ^ (power / 2);
        return half * half;
    } else {
        SquareMat half = (*this) ^ (power / 2);
        return half * half * (*this);
    }
}

// Increment/Decrement operators
SquareMat& SquareMat::operator++() {
    // Pre-increment
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            data[i][j] += 1.0;
        }
    }
    return *this;
}

SquareMat SquareMat::operator++(int) {
    // Post-increment
    SquareMat temp(*this);
    ++(*this);
    return temp;
}

SquareMat& SquareMat::operator--() {
    // Pre-decrement
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            data[i][j] -= 1.0;
        }
    }
    return *this;
}

SquareMat SquareMat::operator--(int) {
    // Post-decrement
    SquareMat temp(*this);
    --(*this);
    return temp;
}

// Transpose and determinant
SquareMat SquareMat::operator~() const {
    SquareMat result(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            result.data[i][j] = data[j][i];
        }
    }
    return result;
}

double SquareMat::operator!() const {
    return calculateDeterminant();
}

// Element access
SquareMat::ProxyRow SquareMat::operator[](size_t row) {
    if (row >= size) {
        throw std::out_of_range("Row index out of range");
    }
    return ProxyRow(data[row]);
}

const SquareMat::ProxyRow SquareMat::operator[](size_t row) const {
    if (row >= size) {
        throw std::out_of_range("Row index out of range");
    }
    return ProxyRow(data[row]);
}

// Comparison operators
bool SquareMat::operator==(const SquareMat& other) const {
    return std::abs(getSum() - other.getSum()) < 1e-9;
}

bool SquareMat::operator!=(const SquareMat& other) const {
    return !(*this == other);
}

bool SquareMat::operator<(const SquareMat& other) const {
    return getSum() < other.getSum();
}

bool SquareMat::operator>(const SquareMat& other) const {
    return getSum() > other.getSum();
}

bool SquareMat::operator<=(const SquareMat& other) const {
    return getSum() <= other.getSum();
}

bool SquareMat::operator>=(const SquareMat& other) const {
    return getSum() >= other.getSum();
}

// Compound assignment operators
SquareMat& SquareMat::operator+=(const SquareMat& other) {
    if (size != other.size) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            data[i][j] += other.data[i][j];
        }
    }
    return *this;
}

SquareMat& SquareMat::operator-=(const SquareMat& other) {
    if (size != other.size) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    }
    
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            data[i][j] -= other.data[i][j];
        }
    }
    return *this;
}

SquareMat& SquareMat::operator*=(const SquareMat& other) {
    *this = *this * other;
    return *this;
}

SquareMat& SquareMat::operator*=(double scalar) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            data[i][j] *= scalar;
        }
    }
    return *this;
}

SquareMat& SquareMat::operator/=(double scalar) {
    if (scalar == 0) {
        throw std::invalid_argument("Division by zero");
    }
    
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            data[i][j] /= scalar;
        }
    }
    return *this;
}

SquareMat& SquareMat::operator%=(const SquareMat& other) {
    if (size != other.size) {
        throw std::invalid_argument("Matrix dimensions must match for element-wise multiplication");
    }
    
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            data[i][j] *= other.data[i][j];  // Element-wise multiplication
        }
    }
    return *this;
}

SquareMat& SquareMat::operator%=(int scalar) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            data[i][j] = static_cast<double>(static_cast<int>(data[i][j]) % scalar);
        }
    }
    return *this;
}

// Output operator
std::ostream& operator<<(std::ostream& os, const SquareMat& mat) {
    for (size_t i = 0; i < mat.size; ++i) {
        os << '[';
        for (size_t j = 0; j < mat.size; ++j) {
            os << mat.data[i][j];
            if (j < mat.size - 1) {
                os << ", ";
            }
        }
        os << ']';
        if (i < mat.size - 1) {
            os << ' ';
        }
    }
    return os;
}

} // namespace mat_ops
