// Email: e-abo@hotmail.com

#include "SquareMat.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>

using namespace mat_ops;
using namespace std;

// Helper function to print a section header
void printHeader(const string& header) {
    cout << "\n===== " << header << " =====" << endl;
}

// Helper function to run and display a test with expected result
template<typename T>
void runTest(const string& description, const T& result, const T& expected) {
    cout << description << endl;
    cout << "RESULT:" << endl << result << endl;
    cout << "EXPECTED:" << endl << expected << endl;
    cout << "TEST " << (result == expected ? "PASSED ✓" : "FAILED ✗") << endl << endl;
}

// Helper for tests without expected values
template<typename T>
void showResult(const string& description, const T& result) {
    cout << description << endl;
    cout << result << endl << endl;
}

int main() {
    try {
        cout << "===== SQUARE MATRIX OPERATIONS DEMO =====" << endl;
        
        // Create test matrices with distinct values for clearer testing
        printHeader("Matrix Creation");
        
        // Matrix with all same values
        SquareMat uniformMat(3, 2.0);
        cout << "Uniform matrix (3x3 with all elements = 2.0):" << endl << uniformMat << endl << endl;
        
        // Matrix with distinct values
        SquareMat distinctMat(3);
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                distinctMat[i][j] = static_cast<double>(i * 3 + j + 1);  // Values 1-9
            }
        }
        cout << "Matrix with distinct values (1-9):" << endl << distinctMat << endl << endl;
        
        // Identity matrix
        SquareMat identityMat(3);
        for (size_t i = 0; i < 3; ++i) {
            identityMat[i][i] = 1.0;
        }
        cout << "Identity matrix (3x3):" << endl << identityMat << endl << endl;
        
        // Basic Operations with uniform matrices
        printHeader("Basic Operations (Uniform Matrix)");
        
        SquareMat mat1(3, 2.0);
        SquareMat mat2(3, 1.0);
        
        cout << "Matrix 1 (all 2.0):" << endl << mat1 << endl << endl;
        cout << "Matrix 2 (all 1.0):" << endl << mat2 << endl << endl;
        
        SquareMat expectedAdd(3, 3.0);  // All elements = 3.0
        runTest("Addition (Matrix 1 + Matrix 2):", mat1 + mat2, expectedAdd);
        
        SquareMat expectedSub(3, 1.0);  // All elements = 1.0
        runTest("Subtraction (Matrix 1 - Matrix 2):", mat1 - mat2, expectedSub);
        
        SquareMat expectedMul(3, 6.0);  // For 3x3 matrix of 2's * matrix of 1's
        runTest("Matrix Multiplication (Matrix 1 * Matrix 2):", mat1 * mat2, expectedMul);
        
        SquareMat expectedElemMul(3, 2.0);  // Element-wise: 2.0 * 1.0 = 2.0
        runTest("Element-wise Multiplication (Matrix 1 % Matrix 2):", mat1 % mat2, expectedElemMul);
        
        SquareMat expectedScalarMul(3, 6.0);  // 2.0 * 3.0 = 6.0
        runTest("Scalar Multiplication (Matrix 1 * 3):", mat1 * 3.0, expectedScalarMul);
        
        SquareMat expectedScalarMul2(3, 4.0);  // 2.0 * 2.0 = 4.0
        runTest("Scalar Multiplication (2 * Matrix 1):", 2.0 * mat1, expectedScalarMul2);
        
        SquareMat expectedDiv(3, 1.0);  // 2.0 / 2.0 = 1.0
        runTest("Division by Scalar (Matrix 1 / 2):", mat1 / 2.0, expectedDiv);
        
        SquareMat expectedMod(3, 0.0);  // 2 % 2 = 0
        runTest("Modulo with Scalar (Matrix 1 % 2):", mat1 % 2, expectedMod);
        
        // Operations with distinct values matrix
        printHeader("Basic Operations (Distinct Values Matrix)");
        
        cout << "Distinct Matrix:" << endl << distinctMat << endl << endl;
        
        SquareMat negated = -distinctMat;
        cout << "Negation (-Matrix):" << endl << negated << endl << endl;
        
        SquareMat transposed = ~distinctMat;
        cout << "Transpose (~Matrix):" << endl << transposed << endl << endl;
        
        // Check if transpose works correctly
        bool transposeCorrect = true;
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                if (distinctMat[i][j] != transposed[j][i]) {
                    transposeCorrect = false;
                }
            }
        }
        cout << "Transpose Test: " << (transposeCorrect ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        double det = !distinctMat;
        cout << "Determinant (!Matrix): " << det << endl;
        cout << "Expected Determinant: 0" << endl;
        cout << "Determinant Test: " << (det == 0.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        // Create a matrix with non-zero determinant
        SquareMat detMat(3);
        detMat[0][0] = 1; detMat[0][1] = 2; detMat[0][2] = 3;
        detMat[1][0] = 0; detMat[1][1] = 4; detMat[1][2] = 5;
        detMat[2][0] = 1; detMat[2][1] = 0; detMat[2][2] = 6;
        
        det = !detMat;
        cout << "Matrix with non-zero determinant:" << endl << detMat << endl;
        cout << "Determinant: " << det << endl;
        cout << "Expected Determinant: 22" << endl;
        cout << "Determinant Test: " << (abs(det - 22.0) < 1e-9 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        // Power operation
        printHeader("Power Operation");
        
        // Identity matrix raised to any power should remain identity
        SquareMat idPow = identityMat ^ 5;
        cout << "Identity matrix ^ 5:" << endl << idPow << endl;
        cout << "Power Test (Identity): " << (idPow == identityMat ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        // Any matrix raised to power 1 should be itself
        SquareMat pow1 = distinctMat ^ 1;
        cout << "Matrix ^ 1:" << endl << pow1 << endl;
        cout << "Power Test (^1): " << (pow1 == distinctMat ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        // Matrix raised to power 0 should be identity
        SquareMat pow0 = distinctMat ^ 0;
        cout << "Matrix ^ 0:" << endl << pow0 << endl;
        cout << "Power Test (^0): " << (pow0 == identityMat ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        // Matrix raised to power 2
        SquareMat pow2 = distinctMat ^ 2;
        cout << "Matrix ^ 2:" << endl << pow2 << endl;
        SquareMat manualSquare = distinctMat * distinctMat;
        cout << "Manual Matrix * Matrix:" << endl << manualSquare << endl;
        cout << "Power Test (^2): " << (pow2 == manualSquare ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        // Increment/Decrement
        printHeader("Increment/Decrement Operations");
        
        SquareMat incMat(2, 5.0);
        cout << "Original Matrix:" << endl << incMat << endl << endl;
        
        SquareMat preInc = ++incMat;
        cout << "After ++Matrix:" << endl << incMat << endl;
        cout << "Pre-increment Test: " << (incMat[0][0] == 6.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        incMat = SquareMat(2, 5.0);
        SquareMat postInc = incMat++;
        cout << "Result of Matrix++:" << endl << postInc << endl;
        cout << "Matrix after post-increment:" << endl << incMat << endl;
        cout << "Post-increment Test: " << (postInc[0][0] == 5.0 && incMat[0][0] == 6.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        incMat = SquareMat(2, 5.0);
        SquareMat preDec = --incMat;
        cout << "After --Matrix:" << endl << incMat << endl;
        cout << "Pre-decrement Test: " << (incMat[0][0] == 4.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        incMat = SquareMat(2, 5.0);
        SquareMat postDec = incMat--;
        cout << "Result of Matrix--:" << endl << postDec << endl;
        cout << "Matrix after post-decrement:" << endl << incMat << endl;
        cout << "Post-decrement Test: " << (postDec[0][0] == 5.0 && incMat[0][0] == 4.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        // Comparison Operations
        printHeader("Comparison Operations");
        
        // Matrices with same sum
        SquareMat sumMat1(2);
        sumMat1[0][0] = 1.0; sumMat1[0][1] = 2.0;
        sumMat1[1][0] = 3.0; sumMat1[1][1] = 4.0;
        // Sum = 10
        
        SquareMat sumMat2(2);
        sumMat2[0][0] = 4.0; sumMat2[0][1] = 3.0;
        sumMat2[1][0] = 2.0; sumMat2[1][1] = 1.0;
        // Sum = 10
        
        SquareMat sumMat3(2);
        sumMat3[0][0] = 5.0; sumMat3[0][1] = 5.0;
        sumMat3[1][0] = 5.0; sumMat3[1][1] = 5.0;
        // Sum = 20
        
        cout << "Matrix 1 (sum=10):" << endl << sumMat1 << endl;
        cout << "Matrix 2 (sum=10):" << endl << sumMat2 << endl;
        cout << "Matrix 3 (sum=20):" << endl << sumMat3 << endl << endl;
        
        cout << "Matrix 1 == Matrix 2 (same sum): " << (sumMat1 == sumMat2) << " - " 
             << ((sumMat1 == sumMat2) ? "PASSED ✓" : "FAILED ✗") << endl;
        
        cout << "Matrix 1 != Matrix 3 (different sum): " << (sumMat1 != sumMat3) << " - " 
             << ((sumMat1 != sumMat3) ? "PASSED ✓" : "FAILED ✗") << endl;
        
        cout << "Matrix 1 < Matrix 3: " << (sumMat1 < sumMat3) << " - " 
             << ((sumMat1 < sumMat3) ? "PASSED ✓" : "FAILED ✗") << endl;
        
        cout << "Matrix 3 > Matrix 1: " << (sumMat3 > sumMat1) << " - " 
             << ((sumMat3 > sumMat1) ? "PASSED ✓" : "FAILED ✗") << endl;
        
        cout << "Matrix 1 <= Matrix 2 (equal sums): " << (sumMat1 <= sumMat2) << " - " 
             << ((sumMat1 <= sumMat2) ? "PASSED ✓" : "FAILED ✗") << endl;
        
        cout << "Matrix 1 >= Matrix 2 (equal sums): " << (sumMat1 >= sumMat2) << " - " 
             << ((sumMat1 >= sumMat2) ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        // Compound Assignment
        printHeader("Compound Assignment Operations");
        
        SquareMat compMat(2, 3.0);
        SquareMat modMat(2, 2.0);
        
        cout << "Original Matrix:" << endl << compMat << endl;
        compMat += modMat;
        cout << "After Matrix += Matrix2:" << endl << compMat << endl;
        cout << "Addition Assignment Test: " << (compMat[0][0] == 5.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        compMat = SquareMat(2, 3.0);
        cout << "Original Matrix:" << endl << compMat << endl;
        compMat -= modMat;
        cout << "After Matrix -= Matrix2:" << endl << compMat << endl;
        cout << "Subtraction Assignment Test: " << (compMat[0][0] == 1.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        compMat = SquareMat(2, 3.0);
        cout << "Original Matrix:" << endl << compMat << endl;
        compMat *= modMat;
        cout << "After Matrix *= Matrix2:" << endl << compMat << endl;
        cout << "Multiplication Assignment Test: " << (compMat[0][0] == 12.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        compMat = SquareMat(2, 3.0);
        cout << "Original Matrix:" << endl << compMat << endl;
        compMat *= 2.0;
        cout << "After Matrix *= 2.0:" << endl << compMat << endl;
        cout << "Scalar Multiplication Assignment Test: " << (compMat[0][0] == 6.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        compMat = SquareMat(2, 4.0);
        cout << "Original Matrix:" << endl << compMat << endl;
        compMat /= 2.0;
        cout << "After Matrix /= 2.0:" << endl << compMat << endl;
        cout << "Division Assignment Test: " << (compMat[0][0] == 2.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        compMat = SquareMat(2, 3.0);
        cout << "Original Matrix:" << endl << compMat << endl;
        compMat %= modMat;
        cout << "After Matrix %= Matrix2 (element-wise):" << endl << compMat << endl;
        cout << "Element-wise Multiplication Assignment Test: " << (compMat[0][0] == 6.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        compMat = SquareMat(2, 5.0);
        cout << "Original Matrix:" << endl << compMat << endl;
        compMat %= 3;
        cout << "After Matrix %= 3:" << endl << compMat << endl;
        cout << "Modulo Assignment Test: " << (compMat[0][0] == 2.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        // Element Access
        printHeader("Element Access");
        
        SquareMat accessMat(3, 7.0);
        cout << "Original Matrix:" << endl << accessMat << endl;
        cout << "Access Matrix[1][1]: " << accessMat[1][1] << endl;
        cout << "Element Access Test: " << (accessMat[1][1] == 7.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        cout << "Modifying Matrix[0][0] = 10.0" << endl;
        accessMat[0][0] = 10.0;
        cout << "Modified Matrix:" << endl << accessMat << endl;
        cout << "Element Modification Test: " << (accessMat[0][0] == 10.0 ? "PASSED ✓" : "FAILED ✗") << endl << endl;
        
        // Exception Handling
        printHeader("Exception Handling");
        
        bool exceptionCaught = false;
        try {
            SquareMat invalidOp1(3);
            SquareMat invalidOp2(2);
            SquareMat result = invalidOp1 + invalidOp2;  // Should throw exception
        } catch (const exception& e) {
            exceptionCaught = true;
            cout << "Size Mismatch Exception Test: PASSED ✓" << endl;
            cout << "Exception message: " << e.what() << endl << endl;
        }
        if (!exceptionCaught) {
            cout << "Size Mismatch Exception Test: FAILED ✗ - Exception not thrown" << endl << endl;
        }
        
        exceptionCaught = false;
        try {
            SquareMat divByZero(2, 1.0);
            SquareMat result = divByZero / 0.0;  // Should throw exception
        } catch (const exception& e) {
            exceptionCaught = true;
            cout << "Division by Zero Exception Test: PASSED ✓" << endl;
            cout << "Exception message: " << e.what() << endl << endl;
        }
        if (!exceptionCaught) {
            cout << "Division by Zero Exception Test: FAILED ✗ - Exception not thrown" << endl << endl;
        }
        
        exceptionCaught = false;
        try {
            SquareMat outOfBounds(2);
            double value = outOfBounds[5][0];  // Should throw exception
        } catch (const exception& e) {
            exceptionCaught = true;
            cout << "Out of Bounds Exception Test: PASSED ✓" << endl;
            cout << "Exception message: " << e.what() << endl << endl;
        }
        if (!exceptionCaught) {
            cout << "Out of Bounds Exception Test: FAILED ✗ - Exception not thrown" << endl << endl;
        }
        
        cout << "\n===== ALL TESTS COMPLETED =====" << endl;
        
    } catch (const exception& e) {
        cerr << "ERROR: Unexpected exception caught in main: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
