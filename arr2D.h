#include <iostream>
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
                t& operator[](const int& col) {
                    if(!transposeFlag){
                        return row_data_ptr[col];
                    }
                    else{
                        return row_data_ptr[col*rows];
                    }
                }
            };
        public:
            arr2d_container(){
                arr2d_ptr = nullptr;
                rows = 0; cols = 0;
                transposeFlag = false;
            }
            arr2d_container(t* arr, const int& rows, const int& cols): arr2d_ptr(arr), rows(rows), cols(cols), transposeFlag(false){}
            void transpose(){
                int temp = rows;
                rows = cols;
                cols = temp;
                transposeFlag = !transposeFlag;
            }
            RowProxy operator[](const int& row);
            int showRowNums(){ return rows;}
            int showColNums(){ return cols;}
            void showArr();
    };

    template <class t>
    typename arr2d_container<t>::RowProxy arr2d_container<t>::operator[](const int& row){
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
    void arr2d_container<t>::showArr(){
        for(int i = 0; i<rows; i++){
            auto temp2 = (*this)[i];
            for(int j = 0; j<cols; j++){
                t temp = temp2[j];
                std::cout << temp << "\t";
            }
            std::cout << std::endl;
        }
    }

}

#endif
