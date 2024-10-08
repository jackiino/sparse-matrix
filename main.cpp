#include <iostream>
#include <vector>
#include "SparseMatrix.hpp"

int main() {
    const unsigned int numRows = 5; //insert the number of rows and columns
    const unsigned int numCols = 5;

    SparseMatrixCOO cooMatrix(numRows, numCols); 

    try {
        cooMatrix(1, 3 , -3.2); //insert values in the matrix. ("row, column, value")
        cooMatrix(2, 2, 3.0);
        cooMatrix(3, 3, 4.0);
        cooMatrix(4, 4, 5.0);  //add other lines in order to insert more values

        std::cout << "COO Matrix:" << std::endl; 
        cooMatrix.show(); //show the matrix passing by COO format

        try { //if the first try works then we pass forward and we can read a value of the matrix
        unsigned int readRow = 3;
        unsigned int readColumn = 3;
        double value = cooMatrix(readRow, readColumn);
        std::cout << "Value A(" << readRow << "," << readColumn << ") = " << value << std::endl; //A is the name of the matrix
        } catch (const std::out_of_range& e) { //if the indices are out of bounds then print an error
            std::cerr << "Error: " << e.what() << std::endl;
        }

    } catch (const std::out_of_range& e) {  //if the indices are out of bounds then show an error
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::vector<double> cooVector = {3.0, 1.0, 5.0, 0.0,0.0}; //vector to calculate the product matrix-vector
    std::vector<double> resultCOO = cooMatrix * cooVector;

    std::cout << "Matrix-Vector Product (COO): "; //show the result of the product
    for (double val : resultCOO) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;


    //check the corretness of the program multiplying the matrix with vector of ones that should return the sum of the i-th row. We have used the assert function, it has been suggested from ChatGPT.
    std::vector<double> cooVector_check(numCols, 1.0); // Vector of ones
    std::vector<double> resultCOO_check = cooMatrix * cooVector_check;

    // Verify the correctness of the product
    for (unsigned int i = 0; i < numRows; ++i) {
        double sum = 0.0;
        for (unsigned int j = 0; j < numCols; ++j) {
            sum += cooMatrix(i, j) * cooVector_check[j];
        }
        assert(resultCOO_check[i] == sum);
    }

    
    

    
    SparseMatrixCSR csrMatrix(numRows, numCols);

    try {
    csrMatrix(1, 1, 2.0); //insert values in the matrix. ("row, column, value")
    csrMatrix(2, 2, 3.0);
    csrMatrix(3, 3, 4.0);
    csrMatrix(4, 4, 5.0); //add other lines in order to insert more values

    std::cout << "CSR Matrix:" << std::endl;
    csrMatrix.show();  //show the matrix passing by COO format

        try {  //if the first try work then we pass forward and we can read a value of the matrix
            unsigned int readRow = 4;
            unsigned int readColumn = 3;
            double value = csrMatrix(readRow, readColumn);
            std::cout << "Value A(" << readRow << "," << readColumn << ") = " << value << std::endl;
        } catch (const std::out_of_range& e) { //if the indices are out of bounds then print an error
            std::cerr << "Error: " << e.what() << std::endl;
        }

    } catch (const std::out_of_range& e) { //if the indices are out of bounds then show an error
        std::cerr << "Error: " << e.what() << std::endl;
    }

    

    std::vector<double> csrVector = {0.0, 1.0, 0.0, 0.0,0.0}; //vector to calculate the product matrix-vector
    std::vector<double> resultCSR = csrMatrix * csrVector;

    

    std::cout << "Matrix-Vector Product (CSR): "; //show the result of the product
    for (double val : resultCSR) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;



    //check the corretness of the program multiplying the matrix with vector of ones that should return the sum of the i-th row. We have used the assert function, it has been suggested from ChatGPT.
    std::vector<double> csrVector_check(numCols, 1.0); // Vector of ones
    std::vector<double> resultCSR_check = csrMatrix * csrVector_check;

    // Verify the correctness of the product
    for (unsigned int i = 0; i < numRows; ++i) {
        double sum = 0.0;
        for (unsigned int k = csrMatrix.row_idxs[i]; k < csrMatrix.row_idxs[i + 1]; ++k) {
            sum += csrMatrix.values[k] * csrVector_check[csrMatrix.columns[k]];
        }
        assert(resultCSR_check[i] == sum);
    }




    std::cout << "Conversion from COO to CSR: " << std::endl; //here we compute the conversion from COO to CSR and we print the matrix in output which should be the same. Delete CsrMatrix.show(); if you don't want to see the matrix
    SparseMatrixCSR CsrMatrix = convertCOOtoCSR(cooMatrix);
    CsrMatrix.show();
    std::cout << std::endl;


    std::cout << "Conversion from CSR to COO: " << std::endl; //Analogous to above
    SparseMatrixCOO CooMatrix = convertCSRtoCOO(csrMatrix);
    CooMatrix.show();



    return 0;
}

