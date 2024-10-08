#include "SparseMatrix.hpp"
#include <iostream>
#include <stdexcept>

// Implementation of the SparseMatrixCOO class
SparseMatrixCOO::SparseMatrixCOO(const unsigned int rows, const unsigned int cols) : rows_(rows), cols_(cols) {}

unsigned int SparseMatrixCOO::getRows() const {
    return rows_;
}

unsigned int SparseMatrixCOO::getColumns() const {
    return cols_;
}

unsigned int SparseMatrixCOO::getNonZeros() const {
    return values.size();
}

void SparseMatrixCOO::operator()(unsigned int row, unsigned int col, double value) { //overloading operator() to write the entries
    if (value == 0.0) {
        throw std::invalid_argument("Value cannot be zero.");
    }
    if (row < rows_ && col < cols_) {
        rows.push_back(row);
        columns.push_back(col);
        values.push_back(value);
    } else {
        throw std::out_of_range("Row or column index is out of bounds.");
    }
}

double SparseMatrixCOO::operator()(unsigned int row, unsigned int col) const { //overload operator() to read the entries
    if (row < rows_ && col < cols_) {
        for (unsigned int i = 0; i < rows.size(); ++i) {
            if (rows[i] == row && columns[i] == col) {
                return values[i];
            }
        }
        return 0.0; // Return a default value if the element is not found.
    } else {
        throw std::out_of_range("Row or column index is out of bounds.");
    }
}

std::vector<double> SparseMatrixCOO::operator*(const std::vector<double>& vector) const { //formula for product sampled from https://www.it.uu.se/education/phd_studies/phd_courses/pasc/lecture-1
    if (vector.size() != cols_) {
        throw std::invalid_argument("Vector dimension does not match the matrix.");
    }

    std::vector<double> result(rows_, 0.0);

    for (unsigned int i = 0; i < values.size(); ++i) {
        result[rows[i]] += values[i] * vector[columns[i]];
    }

    return result;
}

void SparseMatrixCOO::show() const {
    for (unsigned int i = 0; i < rows_; ++i) {
        for (unsigned int j = 0; j < cols_; ++j) {
            double value = 0.0;  // Initialize to 0
            for (unsigned int k = 0; k < values.size(); ++k) {
                if (rows[k] == i && columns[k] == j) {
                    value = values[k]; // Set to the non-zero value if found
                    break;
                }
            }
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

// Implementation of the SparseMatrixCSR class
SparseMatrixCSR::SparseMatrixCSR(const unsigned int rows, const unsigned int cols) : rows_(rows), cols_(cols) {
    row_idxs.resize(rows + 1, 0); //since we know that row_idx vector has one more row with respect to the number of rows of the matrix
}

unsigned int SparseMatrixCSR::getRows() const {
    return rows_;
}

unsigned int SparseMatrixCSR::getColumns() const {
    return cols_;
}

unsigned int SparseMatrixCSR::getNonZeros() const {
    return values.size();
}

void SparseMatrixCSR::operator()(unsigned int row, unsigned int col, double value) {
    if (col < cols_ && row < rows_) {
        values.push_back(value);
        columns.push_back(col);
        for (unsigned int i = row + 1; i < row_idxs.size(); ++i) {
            row_idxs[i]++;
        }
    } else {
        throw std::out_of_range("Row or column index is out of bounds.");
    }
}

void SparseMatrixCSR::show() const {
    for (unsigned int i = 0; i < rows_; ++i) {
        for (unsigned int j = 0; j < cols_; ++j) {
            double value = 0.0; // Initialize to 0
            for (unsigned int k = row_idxs[i]; k < row_idxs[i + 1]; ++k) {
                if (columns[k] == j) {
                    value = values[k]; // Set to the non-zero value if found
                    break;
                }
            }
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}


double SparseMatrixCSR::operator()(unsigned int row, unsigned int col) const {
    for (unsigned int i = row_idxs[row]; i < row_idxs[row + 1]; ++i) {
        if (columns[i] == col) {
            return values[i];
        }
    }
    return 0.0;
}

std::vector<double> SparseMatrixCSR::operator*(const std::vector<double>& vector) const {   //formula for product sampled from https://www.it.uu.se/education/phd_studies/phd_courses/pasc/lecture-1
    if (vector.size() != cols_) {
        throw std::invalid_argument("Vector dimension does not match the matrix.");
    }

    std::vector<double> result(rows_, 0.0);

    for (unsigned int i = 0; i < rows_; ++i) {
        for (unsigned int k = row_idxs[i]; k < row_idxs[i + 1]; ++k) {
            result[i] += values[k] * vector[columns[k]];
        }
    }

    return result;
}

// Function to convert SparseMatrixCOO to SparseMatrixCSR
SparseMatrixCSR convertCOOtoCSR(const SparseMatrixCOO& cooMatrix) {
    unsigned int rows = cooMatrix.getRows();
    unsigned int cols = cooMatrix.getColumns();
    SparseMatrixCSR csrMatrix(rows, cols);

    for (size_t i = 0; i < cooMatrix.getNonZeros(); ++i) {
        unsigned int row = cooMatrix.rows[i];  // Get row from COO
        unsigned int col = cooMatrix.columns[i];  // Get column from COO
        double value = cooMatrix(row, col);      // Get value from COO

        // Insert the element into the CSR matrix
        csrMatrix(row, col, value);
    }

    return csrMatrix;
}

// Function to convert SparseMatrixCSR to SparseMatrixCOO
SparseMatrixCOO convertCSRtoCOO(const SparseMatrixCSR& csrMatrix) {
    unsigned int rows = csrMatrix.getRows();
    unsigned int cols = csrMatrix.getColumns();
    SparseMatrixCOO cooMatrix(rows, cols);

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int k = csrMatrix.row_idxs[i]; k < csrMatrix.row_idxs[i + 1]; ++k) {
            unsigned int col = csrMatrix.columns[k];
            double value = csrMatrix.values[k];
            cooMatrix(i, col, value);
        }
    }

    return cooMatrix;
}
