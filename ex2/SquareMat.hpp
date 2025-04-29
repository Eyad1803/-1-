#pragma once
// Email: e-abo@hotmail.com

#include <iostream>

namespace mat_ops {

class SquareMat {
private:
    double** data;  // 2D array to store matrix elements
    size_t size;    // Size of the square matrix (n x n)

    // Helper methods
    void allocateMemory();
    void deallocateMemory();
    void copyFrom(const SquareMat& other);
    double calculateDeterminant() const;

public:
    // Constructors and Destructor
    SquareMat(size_t size = 0);                 // Default constructor and constructor with size
    SquareMat(size_t size, double initialValue); // Constructor with size and initial value
    SquareMat(const SquareMat& other);          // Copy constructor (Rule of Three)
    ~SquareMat();                               // Destructor (Rule of Three)

    // Assignment operator (Rule of Three)
    SquareMat& operator=(const SquareMat& other);

    // Basic getters
    size_t getSize() const { return size; }
    double getSum() const;  // Helper for comparison operators

    // Arithmetic operators
    SquareMat operator+(const SquareMat& other) const;
    SquareMat operator-(const SquareMat& other) const;
    SquareMat operator-() const;  // Unary minus
    SquareMat operator*(const SquareMat& other) const;  // Matrix multiplication
    SquareMat operator*(double scalar) const;  // Matrix * scalar
    friend SquareMat operator*(double scalar, const SquareMat& mat);  // scalar * Matrix
    SquareMat operator%(const SquareMat& other) const;  // Element-wise multiplication
    SquareMat operator%(int scalar) const;  // Modulo with scalar
    SquareMat operator/(double scalar) const;  // Division by scalar
    SquareMat operator^(int power) const;  // Power operator

    // Increment/Decrement operators
    SquareMat& operator++();    // Pre-increment
    SquareMat operator++(int);  // Post-increment
    SquareMat& operator--();    // Pre-decrement
    SquareMat operator--(int);  // Post-decrement

    // Transpose and determinant
    SquareMat operator~() const;  // Transpose
    double operator!() const;     // Determinant

    // Element access
    class ProxyRow {
    private:
        double* row;
    public:
        ProxyRow(double* row) : row(row) {}
        double& operator[](size_t col) { return row[col]; }
        const double& operator[](size_t col) const { return row[col]; }
    };

    ProxyRow operator[](size_t row);
    const ProxyRow operator[](size_t row) const;

    // Comparison operators
    bool operator==(const SquareMat& other) const;
    bool operator!=(const SquareMat& other) const;
    bool operator<(const SquareMat& other) const;
    bool operator>(const SquareMat& other) const;
    bool operator<=(const SquareMat& other) const;
    bool operator>=(const SquareMat& other) const;

    // Compound assignment operators
    SquareMat& operator+=(const SquareMat& other);
    SquareMat& operator-=(const SquareMat& other);
    SquareMat& operator*=(const SquareMat& other);  // Matrix multiplication
    SquareMat& operator*=(double scalar);  // Scalar multiplication
    SquareMat& operator/=(double scalar);  // Division by scalar
    SquareMat& operator%=(const SquareMat& other);  // Element-wise multiplication
    SquareMat& operator%=(int scalar);  // Modulo with scalar

    // Output operator
    friend std::ostream& operator<<(std::ostream& os, const SquareMat& mat);
};

} // namespace mat_ops
