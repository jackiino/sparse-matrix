#pragma once
#define SPARSEMATRIX_HPP
#include <vector>

class SparseMatrix {
public:
    virtual unsigned int getRows() const = 0;
    virtual unsigned int getColumns() const = 0;
    virtual unsigned int getNonZeros() const = 0;
    virtual void operator()(unsigned int row, unsigned int col, double value) = 0;
    virtual void show() const = 0;
    virtual double operator()(unsigned int row, unsigned int col) const = 0;
    virtual std::vector<double> operator*(const std::vector<double>& vector) const = 0;
};

class SparseMatrixCOO : public SparseMatrix {
public:
    SparseMatrixCOO(const unsigned int rows, const unsigned int cols);

    unsigned int getRows() const override;
    unsigned int getColumns() const override;
    unsigned int getNonZeros() const override;
    void operator()(unsigned int row, unsigned int col, double value) override;
    void show() const override;
    double operator()(unsigned int row, unsigned int col) const override;
    std::vector<double> operator*(const std::vector<double>& vector) const override;


    unsigned int rows_;
    unsigned int cols_;
    std::vector<double> values;
    std::vector<unsigned int> rows;
    std::vector<unsigned int> columns;
};

class SparseMatrixCSR : public SparseMatrix {
public:
    SparseMatrixCSR(const unsigned int rows, const unsigned int cols);

    unsigned int getRows() const override;
    unsigned int getColumns() const override;
    unsigned int getNonZeros() const override;
    void operator()(unsigned int row, unsigned int col, double value) override;
    void show() const override;
    double operator()(unsigned int row, unsigned int col) const override;
    std::vector<double> operator*(const std::vector<double>& vector) const override;
    

    unsigned int rows_;
    unsigned int cols_;
    std::vector<double> values;
    std::vector<unsigned int> row_idxs;
    std::vector<unsigned int> columns;
};

SparseMatrixCSR convertCOOtoCSR(const SparseMatrixCOO& cooMatrix);

SparseMatrixCOO convertCSRtoCOO(const SparseMatrixCSR& csrMatrix);


