#include <iostream>
#include <typeinfo>
#ifndef ARR2D_H_
#define ARR2D_H_

namespace arr2D{
    template <class t>
    class arr2d_container{
        private:
            t* arr2d_ptr;
            int rows;
            int cols;
            bool transposeFlag;
            struct RowProxy {
                t* row_data_ptr;
                int len;
                bool transposeFlag;
                int rows;
                t& operator[](const int col) {
                    if(!transposeFlag){
                        return row_data_ptr[col];
                    }
                    else{
                        return row_data_ptr[col*rows];
                    }
                }
                const t& operator[](const int col) const{
                    return const_cast<RowProxy*>(this)->operator[](col);
                }
            };
        public:
            arr2d_container(){
                arr2d_ptr = nullptr;
                rows = 0; cols = 0;
                transposeFlag = false;
            }
            arr2d_container(t* arr, const int rows, const int cols): arr2d_ptr(arr), rows(rows), cols(cols), transposeFlag(false){}
            void transpose(){
                int temp = rows;
                rows = cols;
                cols = temp;
                transposeFlag = !transposeFlag;
            }
            RowProxy operator[](const int row);
            const RowProxy operator[](const int row) const;
            void deleteArr2d();
            t* showArr2dPtr() const{ return arr2d_ptr; }
            int showRowNums() const{ return rows;}
            int showColNums() const{ return cols;}
            void printArr() const;
            void changePram(t* arrPtr){ arr2d_ptr = arrPtr; }
            void changePram(const int rows_, const int cols_);
            void changePram(const int rowsOrCols, const bool rowFlag, const bool useless_);
            void changePram(const bool transposeFlag_){ transposeFlag = transposeFlag_; }
    };

    template <class t>
    typename arr2d_container<t>::RowProxy arr2d_container<t>::operator[](const int row){
        RowProxy rowData;
        rowData.len = cols;
        rowData.transposeFlag = transposeFlag;
        if(!transposeFlag){
            rowData.row_data_ptr = arr2d_ptr + row*cols;
        }
        else{
            rowData.row_data_ptr = arr2d_ptr + row;
            rowData.rows = rows;
        }
        return rowData;
    }

    template <class t>
    const typename arr2d_container<t>::RowProxy arr2d_container<t>::operator[](const int row) const{
        const RowProxy rowData = const_cast<arr2d_container<t>*>(this)->operator[](row);
        return rowData;
    }

    template <class t>
    void arr2d_container<t>::deleteArr2d(){
        delete[] arr2d_ptr;
        arr2d_ptr = nullptr;
    }

    template <class t>
    void arr2d_container<t>::printArr() const{
        for(int i = 0; i<rows; i++){
            auto temp2 = (*this)[i];
            for(int j = 0; j<cols; j++){
                t temp = temp2[j];
                std::cout << temp;
                if(j < cols-1)
                    std::cout << ", ";
            }
            std::cout << std::endl;
        }
    }

    template <class t>
    void arr2d_container<t>::changePram(const int rows_, const int cols_){
        rows = rows_; cols = cols_;
    }

    template <class t>
    void arr2d_container<t>::changePram(const int rowsOrCols, const bool rowFlag, const bool useless_){
        if(rowFlag){
            rows = rowsOrCols;
        }
        else{
            cols = rowsOrCols;
        }
    }

}

#endif
