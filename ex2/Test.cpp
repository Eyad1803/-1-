// Email: e-abo@hotmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "SquareMat.hpp"
#include <cmath>

using namespace mat_ops;

// Helper functions for tests
bool areMatricesEqual(const SquareMat& m1, const SquareMat& m2, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (std::abs(m1[i][j] - m2[i][j]) > 1e-9) {
                return false;
            }
        }
    }
    return true;
}

TEST_CASE("Constructor tests") {
    SUBCASE("Default constructor creates a 0x0 matrix") {
        SquareMat mat;
        CHECK(mat.getSize() == 0);
    }
    
    SUBCASE("Size constructor creates a matrix of specified size with zeros") {
        SquareMat mat(3);
        CHECK(mat.getSize() == 3);
        
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                CHECK(mat[i][j] == 0.0);
            }
        }
    }
    
    SUBCASE("Constructor with size and initial value") {
        SquareMat mat(2, 5.5);
        CHECK(mat.getSize() == 2);
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(mat[i][j] == 5.5);
            }
        }
    }
    
    SUBCASE("Copy constructor") {
        SquareMat original(2, 3.0);
        SquareMat copy(original);
        
        CHECK(copy.getSize() == original.getSize());
        CHECK(areMatricesEqual(copy, original, 2));
        
        // Modify the original to ensure deep copy
        original[0][0] = 9.9;
        CHECK(copy[0][0] == 3.0);  // Copy remains unchanged
    }
}

TEST_CASE("Assignment operator") {
    SquareMat mat1(2, 1.0);
    SquareMat mat2(3, 2.0);
    
    mat1 = mat2;
    
    CHECK(mat1.getSize() == 3);
    CHECK(areMatricesEqual(mat1, mat2, 3));
    
    // Modify mat2 to ensure deep copy
    mat2[0][0] = 9.9;
    CHECK(mat1[0][0] == 2.0);  // mat1 remains unchanged
}

TEST_CASE("Basic operations") {
    SUBCASE("Addition") {
        SquareMat m1(2, 1.0);
        SquareMat m2(2, 2.0);
        SquareMat result = m1 + m2;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(result[i][j] == 3.0);
            }
        }
        
        // Check exception for mismatched sizes
        SquareMat m3(3, 1.0);
        CHECK_THROWS_AS(m1 + m3, std::invalid_argument);
    }
    
    SUBCASE("Subtraction") {
        SquareMat m1(2, 5.0);
        SquareMat m2(2, 2.0);
        SquareMat result = m1 - m2;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(result[i][j] == 3.0);
            }
        }
        
        // Check exception for mismatched sizes
        SquareMat m3(3, 1.0);
        CHECK_THROWS_AS(m1 - m3, std::invalid_argument);
    }
    
    SUBCASE("Unary minus") {
        SquareMat m(2, 3.0);
        SquareMat result = -m;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(result[i][j] == -3.0);
            }
        }
    }
    
    SUBCASE("Matrix multiplication") {
        // Create a matrix with known values
        SquareMat m1(2);
        m1[0][0] = 1.0; m1[0][1] = 2.0;
        m1[1][0] = 3.0; m1[1][1] = 4.0;
        
        SquareMat m2(2);
        m2[0][0] = 5.0; m2[0][1] = 6.0;
        m2[1][0] = 7.0; m2[1][1] = 8.0;
        
        SquareMat result = m1 * m2;
        
        // Expected result:
        // [1*5 + 2*7, 1*6 + 2*8] = [19, 22]
        // [3*5 + 4*7, 3*6 + 4*8] = [43, 50]
        CHECK(result[0][0] == 19.0);
        CHECK(result[0][1] == 22.0);
        CHECK(result[1][0] == 43.0);
        CHECK(result[1][1] == 50.0);
        
        // Check exception for mismatched sizes
        SquareMat m3(3, 1.0);
        CHECK_THROWS_AS(m1 * m3, std::invalid_argument);
    }
    
    SUBCASE("Scalar multiplication (matrix * scalar)") {
        SquareMat m(2, 3.0);
        SquareMat result = m * 2.0;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(result[i][j] == 6.0);
            }
        }
    }
    
    SUBCASE("Scalar multiplication (scalar * matrix)") {
        SquareMat m(2, 3.0);
        SquareMat result = 2.0 * m;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(result[i][j] == 6.0);
            }
        }
    }
    
    SUBCASE("Element-wise multiplication") {
        SquareMat m1(2, 3.0);
        SquareMat m2(2, 2.0);
        SquareMat result = m1 % m2;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(result[i][j] == 6.0);
            }
        }
        
        // Check exception for mismatched sizes
        SquareMat m3(3, 1.0);
        CHECK_THROWS_AS(m1 % m3, std::invalid_argument);
    }
    
    SUBCASE("Modulo with scalar") {
        SquareMat m(2);
        m[0][0] = 5.0; m[0][1] = 6.0;
        m[1][0] = 7.0; m[1][1] = 8.0;
        
        SquareMat result = m % 3;
        
        CHECK(result[0][0] == 2.0);
        CHECK(result[0][1] == 0.0);
        CHECK(result[1][0] == 1.0);
        CHECK(result[1][1] == 2.0);
    }
    
    SUBCASE("Division by scalar") {
        SquareMat m(2, 6.0);
        SquareMat result = m / 2.0;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(result[i][j] == 3.0);
            }
        }
        
        // Check exception for division by zero
        CHECK_THROWS_AS(m / 0.0, std::invalid_argument);
    }
    
    SUBCASE("Power operator") {
        // Identity matrix
        SquareMat identity(2);
        identity[0][0] = identity[1][1] = 1.0;
        identity[0][1] = identity[1][0] = 0.0;
        
        // Power 0 should give identity matrix
        SquareMat m(2, 2.0);
        SquareMat result = m ^ 0;
        CHECK(areMatricesEqual(result, identity, 2));
        
        // Power 1 should return the original matrix
        result = m ^ 1;
        CHECK(areMatricesEqual(result, m, 2));
        
        // Test with a more complex matrix
        SquareMat m2(2);
        m2[0][0] = 1.0; m2[0][1] = 2.0;
        m2[1][0] = 3.0; m2[1][1] = 4.0;
        
        // m2^2 should be m2 * m2
        result = m2 ^ 2;
        SquareMat expected = m2 * m2;
        CHECK(areMatricesEqual(result, expected, 2));
        
        // Check exception for negative power
        CHECK_THROWS_AS(m ^ (-1), std::invalid_argument);
    }
}

TEST_CASE("Increment/Decrement operators") {
    SUBCASE("Pre-increment") {
        SquareMat m(2, 3.0);
        SquareMat& result = ++m;
        
        // Check that all elements were incremented
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(m[i][j] == 4.0);
            }
        }
        
        // Check that it returns a reference to the same matrix
        CHECK(&result == &m);
    }
    
    SUBCASE("Post-increment") {
        SquareMat m(2, 3.0);
        SquareMat result = m++;
        
        // Check that original was incremented
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(m[i][j] == 4.0);
            }
        }
        
        // Check that result is the original value
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(result[i][j] == 3.0);
            }
        }
    }
    
    SUBCASE("Pre-decrement") {
        SquareMat m(2, 3.0);
        SquareMat& result = --m;
        
        // Check that all elements were decremented
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(m[i][j] == 2.0);
            }
        }
        
        // Check that it returns a reference to the same matrix
        CHECK(&result == &m);
    }
    
    SUBCASE("Post-decrement") {
        SquareMat m(2, 3.0);
        SquareMat result = m--;
        
        // Check that original was decremented
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(m[i][j] == 2.0);
            }
        }
        
        // Check that result is the original value
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(result[i][j] == 3.0);
            }
        }
    }
}

TEST_CASE("Transpose and determinant") {
    SUBCASE("Transpose") {
        SquareMat m(2);
        m[0][0] = 1.0; m[0][1] = 2.0;
        m[1][0] = 3.0; m[1][1] = 4.0;
        
        SquareMat result = ~m;
        
        CHECK(result[0][0] == 1.0);
        CHECK(result[0][1] == 3.0);
        CHECK(result[1][0] == 2.0);
        CHECK(result[1][1] == 4.0);
    }
    
    SUBCASE("Determinant") {
        // 2x2 matrix
        SquareMat m(2);
        m[0][0] = 1.0; m[0][1] = 2.0;
        m[1][0] = 3.0; m[1][1] = 4.0;
        
        double det = !m;
        CHECK(det == -2.0);  // 1*4 - 2*3 = -2
        
        // 3x3 matrix
        SquareMat m3(3);
        m3[0][0] = 1.0; m3[0][1] = 2.0; m3[0][2] = 3.0;
        m3[1][0] = 4.0; m3[1][1] = 5.0; m3[1][2] = 6.0;
        m3[2][0] = 7.0; m3[2][1] = 8.0; m3[2][2] = 9.0;
        
        det = !m3;
        CHECK(det == 0.0);  // This matrix has determinant 0
        
        // Another 3x3 with non-zero determinant
        m3[0][0] = 2.0; m3[0][1] = -3.0; m3[0][2] = 1.0;
        m3[1][0] = 2.0; m3[1][1] = 0.0;  m3[1][2] = -1.0;
        m3[2][0] = 1.0; m3[2][1] = 4.0;  m3[2][2] = 5.0;
        
        det = !m3;
        CHECK(det == 49.0);
    }
}

TEST_CASE("Element access") {
    SquareMat m(2);
    m[0][0] = 1.0; m[0][1] = 2.0;
    m[1][0] = 3.0; m[1][1] = 4.0;
    
    CHECK(m[0][0] == 1.0);
    CHECK(m[0][1] == 2.0);
    CHECK(m[1][0] == 3.0);
    CHECK(m[1][1] == 4.0);
    
    // Test modification
    m[0][0] = 10.0;
    CHECK(m[0][0] == 10.0);
    
    // Test exception on out-of-bounds access
    CHECK_THROWS_AS(m[5][0], std::out_of_range);
}

TEST_CASE("Comparison operators") {
    // These matrices have the same sum (1+2+3+4 = 10)
    SquareMat m1(2);
    m1[0][0] = 1.0; m1[0][1] = 2.0;
    m1[1][0] = 3.0; m1[1][1] = 4.0;
    
    SquareMat m2(2);
    m2[0][0] = 4.0; m2[0][1] = 3.0;
    m2[1][0] = 2.0; m2[1][1] = 1.0;
    
    // Different sum (1+2+3+5 = 11)
    SquareMat m3(2);
    m3[0][0] = 1.0; m3[0][1] = 2.0;
    m3[1][0] = 3.0; m3[1][1] = 5.0;
    
    SUBCASE("Equality and inequality") {
        CHECK(m1 == m2);  // Same sum
        CHECK_FALSE(m1 != m2);
        
        CHECK(m1 != m3);  // Different sum
        CHECK_FALSE(m1 == m3);
    }
    
    SUBCASE("Greater than and less than") {
        CHECK(m3 > m1);  // 11 > 10
        CHECK_FALSE(m1 > m3);
        
        CHECK(m1 < m3);  // 10 < 11
        CHECK_FALSE(m3 < m1);
        
        CHECK_FALSE(m1 > m2);  // Equal sums
        CHECK_FALSE(m1 < m2);
    }
    
    SUBCASE("Greater than or equal and less than or equal") {
        CHECK(m3 >= m1);  // 11 >= 10
        CHECK_FALSE(m1 >= m3);
        
        CHECK(m1 <= m3);  // 10 <= 11
        CHECK_FALSE(m3 <= m1);
        
        CHECK(m1 >= m2);  // Equal sums
        CHECK(m1 <= m2);
    }
}

TEST_CASE("Compound assignment operators") {
    SUBCASE("Addition assignment") {
        SquareMat m1(2, 1.0);
        SquareMat m2(2, 2.0);
        m1 += m2;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(m1[i][j] == 3.0);
            }
        }
        
        // Check exception for mismatched sizes
        SquareMat m3(3, 1.0);
        CHECK_THROWS_AS(m1 += m3, std::invalid_argument);
    }
    
    SUBCASE("Subtraction assignment") {
        SquareMat m1(2, 5.0);
        SquareMat m2(2, 2.0);
        m1 -= m2;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(m1[i][j] == 3.0);
            }
        }
        
        // Check exception for mismatched sizes
        SquareMat m3(3, 1.0);
        CHECK_THROWS_AS(m1 -= m3, std::invalid_argument);
    }
    
    SUBCASE("Matrix multiplication assignment") {
        // Create a matrix with known values
        SquareMat m1(2);
        m1[0][0] = 1.0; m1[0][1] = 2.0;
        m1[1][0] = 3.0; m1[1][1] = 4.0;
        
        SquareMat m2(2);
        m2[0][0] = 5.0; m2[0][1] = 6.0;
        m2[1][0] = 7.0; m2[1][1] = 8.0;
        
        SquareMat expected = m1 * m2;
        m1 *= m2;
        
        CHECK(areMatricesEqual(m1, expected, 2));
        
        // Check exception for mismatched sizes
        SquareMat m3(3, 1.0);
        CHECK_THROWS_AS(m1 *= m3, std::invalid_argument);
    }
    
    SUBCASE("Scalar multiplication assignment") {
        SquareMat m(2, 3.0);
        m *= 2.0;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(m[i][j] == 6.0);
            }
        }
    }
    
    SUBCASE("Division assignment") {
        SquareMat m(2, 6.0);
        m /= 2.0;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(m[i][j] == 3.0);
            }
        }
        
        // Check exception for division by zero
        CHECK_THROWS_AS(m /= 0.0, std::invalid_argument);
    }
    
    SUBCASE("Element-wise multiplication assignment") {
        SquareMat m1(2, 3.0);
        SquareMat m2(2, 2.0);
        m1 %= m2;
        
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                CHECK(m1[i][j] == 6.0);
            }
        }
        
        // Check exception for mismatched sizes
        SquareMat m3(3, 1.0);
        CHECK_THROWS_AS(m1 %= m3, std::invalid_argument);
    }
    
    SUBCASE("Modulo with scalar assignment") {
        SquareMat m(2);
        m[0][0] = 5.0; m[0][1] = 6.0;
        m[1][0] = 7.0; m[1][1] = 8.0;
        
        m %= 3;
        
        CHECK(m[0][0] == 2.0);
        CHECK(m[0][1] == 0.0);
        CHECK(m[1][0] == 1.0);
        CHECK(m[1][1] == 2.0);
    }
}

TEST_CASE("Output operator") {
    // This is mostly a visual test, but we can at least ensure it doesn't crash
    SquareMat m(2, 1.5);
    std::ostringstream oss;
    oss << m;
    CHECK(oss.str().size() > 0);
}
