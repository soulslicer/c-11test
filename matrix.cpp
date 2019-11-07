#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <tuple>
#include <chrono>
#include <iomanip>


using namespace std;


template <class T>
class Matrix{
private:
    int rows_, cols_;
    std::unique_ptr<T> ptr_;

public:
    Matrix(int rows, int cols, T val=0);
    ~Matrix();
    void copyFrom(const Matrix<T>& obj);

    Matrix<T>& operator=(const T val);

    Matrix<T>& operator=(const Matrix<T>& obj);

    Matrix<T>(const Matrix<T> & obj);

    //Matrix<T> operator+(const Matrix<T> value) const;

    std::string toString() const;
    template <typename U>      // all instantiations of this template are my friends
    friend std::ostream& operator<<( std::ostream&, const Matrix<T>& );
};

template <class T>
Matrix<T>::Matrix(int rows, int cols, T val) : rows_(rows), cols_(cols){
    ptr_ = std::unique_ptr<T>(new T[rows_*cols_]);
    if(val) *this = val;
}

template <class T>
Matrix<T>::~Matrix(){

}

template <class T>
void Matrix<T>::copyFrom(const Matrix<T> & obj){
    rows_ = obj.rows_;
    cols_ = obj.cols_;
    ptr_ = std::unique_ptr<T>(new T[obj.rows_*obj.cols_]);
    for(auto r=0; r<this->rows_; r++){
        for(auto c=0; c<this->cols_; c++){
            this->ptr_.get()[r*cols_ + c] = obj.ptr_.get()[r*cols_ + c];
        }
    }
}

template <class T>
Matrix<T>::Matrix(const Matrix<T> & obj){
    this->copyFrom(obj);
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& obj)
{
    this->copyFrom(obj);
}


template<typename T>
Matrix<T>& Matrix<T>::operator=(const T value)
{
    for(auto r=0; r<this->rows_; r++){
        for(auto c=0; c<this->cols_; c++){
            this->ptr_.get()[r*cols_ + c] = value;
        }
    }

    return *this;
}

//template<typename T>
//Matrix<T> Matrix<T>::operator+(const Matrix<T>& point) const
//{


//    try
//    {
//        return Point<T>{T(x + point.x), T(y + point.y)};
//    }
//    catch (const std::exception& e)
//    {
//        error(e.what(), __LINE__, __FUNCTION__, __FILE__);
//        return Point<T>{};
//    }
//}

template <class T>
std::string Matrix<T>::toString() const{
    std::string mString;
    int rowCount = 5;
    int colCount = 5;

    std::cout << std::setprecision(3) << std::fixed;
    mString += "[";
    for(auto i=0; i<rows_; i++){
        if(i < rowCount || i >= rows_-rowCount){
            mString += "[";
            for(int j=0; j<cols_; j++){
                if(j < colCount || j >= cols_-colCount)
                    mString += std::to_string(ptr_.get()[i*cols_ + j]) + "     ";
                else if(j == colCount)
                    mString += " ... ";
            }
            mString += "]\n";
        }else if(i == rowCount){
            mString += "...\n";
            mString += "...\n";
        }
    }
    mString += "]\n";

    return mString;
}

template <typename T>
std::ostream& operator<<( std::ostream& ostream, const Matrix<T>& obj) {
    ostream << obj.toString();
    return ostream;
}


int main (int argc, char ** argv)
{
    Matrix<float> matrix(5,5);
    matrix = 5;

    Matrix<float> matrix2 = matrix;

    Matrix<float> matrix3(3,3, 2.0);
    //matrix2 = matrix;

//    Matrix<float> matrix2(5,5);
//    matrix2 = matrix;

    std::cout << matrix << std::endl;

    std::cout << matrix2 << std::endl;

    std::cout << matrix3 << std::endl;

    return (0);
}
