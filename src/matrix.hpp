#ifndef __MATRIX_HPP__
/*
 * Liberally borrowed from a gist by Tomoatsu Shimada (@shimarin)
 * Used for the same purpose - to provide a chunk of storage for a console view
 */
template <typename T> class Matrix {
    T* buf;
    int rows, cols;
public:
    Matrix(int _rows, int _cols) : rows(_rows), cols(_cols) {
        buf = new T[cols * rows];
    }
    ~Matrix() {
        delete buf;
    }
    void fill(const T& by) {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                buf[cols * row + col] = by;
            }
        }
    }
    T& operator()(int row, int col) {
        if (row < 0 || col < 0 || row >= rows || col >= cols) throw std::runtime_error("invalid position");
        //else
        return buf[cols * row + col];
    }
    int getRows() const { return rows; }
    int getCols() const { return cols; }
};

#define __MATRIX_HPP__
#endif