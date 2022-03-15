#pragma once

#include <vector>

class Matrix {
public:
    Matrix(size_t n_rows, size_t n_cols) : n_rows_(n_rows), n_cols_(n_cols) {
        matrix_.resize(n_rows_);
        for (size_t i = 0; i < n_rows_; ++i) {
            matrix_[i].resize(n_cols_);
        }
    }

    Matrix(size_t n) : n_rows_(n), n_cols_(n) {
        matrix_.resize(n_rows_);
        for (size_t i = 0; i < n_rows_; ++i) {
            matrix_[i].resize(n_cols_);
        }
    }

    Matrix(std::vector<std::vector<double>> matrix)
        : matrix_(matrix), n_rows_(matrix.size()), n_cols_(matrix[0].size()) {
    }

    size_t Rows() const {
        return n_rows_;
    }

    size_t Columns() const {
        return n_cols_;
    }

    double operator()(size_t i, size_t j) const {
        return matrix_[i][j];
    }

    double& operator()(size_t i, size_t j) {
        return matrix_[i][j];
    }

    friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);

    Matrix& operator+=(const Matrix& rhs) {
        return *this = *this + rhs;
    }

    friend Matrix operator-(const Matrix& lhs, const Matrix& rhs);

    Matrix& operator-=(const Matrix& rhs) {
        return *this = *this - rhs;
    }

    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);

    Matrix& operator*=(const Matrix& rhs) {
        return *this = *this * rhs;
    }

private:
    std::vector<std::vector<double>> matrix_;
    size_t n_rows_, n_cols_;
};

Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
    std::vector<std::vector<double>> result;
    result.resize(lhs.Rows());

    for (size_t i = 0; i < lhs.Rows(); ++i) {
        result[i].resize(lhs.Columns());
        for (size_t j = 0; j < lhs.Columns(); ++j) {
            result[i][j] = lhs(i, j) + rhs(i, j);
        }
    }

    return Matrix(result);
}

Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
    std::vector<std::vector<double>> result;
    result.resize(lhs.Rows());

    for (size_t i = 0; i < lhs.Rows(); ++i) {
        result[i].resize(lhs.Columns());
        for (size_t j = 0; j < lhs.Columns(); ++j) {
            result[i][j] = lhs(i, j) - rhs(i, j);
        }
    }

    return Matrix(result);
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    std::vector<std::vector<double>> result;
    result.resize(lhs.Rows());
    for (size_t i = 0; i < lhs.Rows(); ++i) {
        result[i].resize(rhs.Columns());
        for (size_t j = 0; j < rhs.Columns(); ++j) {
            for (size_t k = 0; k < lhs.Columns(); ++k) {
                result[i][j] += lhs(i, k) * rhs(k, j);
            }
        }
    }
    return Matrix(result);
}

Matrix Transpose(const Matrix& in) {
    std::vector<std::vector<double>> result;
    result.resize(in.Columns());
    for (size_t i = 0; i < in.Columns(); ++i) {
        result[i].resize(in.Rows());
        for (size_t j = 0; j < in.Rows(); ++j) {
            result[i][j] = in(j, i);
        }
    }

    return Matrix(result);
}

Matrix Identity(size_t n) {
    std::vector<std::vector<double>> result;
    result.resize(n);
    for (size_t i = 0; i < n; ++i) {
        result[i].resize(n);
        result[i][i] = 1.0;
    }

    return Matrix(result);
}
