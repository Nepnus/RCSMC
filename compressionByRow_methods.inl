#include <cmath>
#include <type_traits>
#include <iostream>
#include <typeinfo>
#include "compressionByRow.h"
#ifndef COMPRESS_H_METHODS_
#define COMPRESS_H_METHODS_

namespace CSR{
    template <class t>
    void CSR_container<t>::rowSum(t* sumResult) const{
        int data_point = 0;
        for(int i = 0; i<rows; i++){
            int rowNotZeroNums = rows_ptr[i+1] - rows_ptr[i];
            t sumtemp = (t)0;
            for(int j = 0; j<rowNotZeroNums; j++){
                sumtemp += data[data_point];
                data_point++;
            }
            sumResult[i] = sumtemp;
        }
    }

    template <class t>
    t CSR_container<t>::sum() const{
        t sumResult = (t)0;
        int data_len = rows_ptr[rows];
        for(int i = 0; i<data_len; i++){
            sumResult += data[i];
        }
        return sumResult;
    }

    template <class t>
    void CSR_container<t>::revert() const{
        if(changeFlag){
            arr2d_ptr_revert.deleteArr2d();
            t* arr = new t[rows*cols]{(t)0};
            arr2d_ptr_revert.changePram(arr);
            arr2d_ptr_revert.changePram(rows, cols);
            int data_point = 0;
            for(int i = 0; i<rows; i++){
                int rowNotZeroNums = rows_ptr[i+1] - rows_ptr[i];
                auto arr2d_ptr_revert_row = arr2d_ptr_revert[i];
                for(int j = 0; j<rowNotZeroNums; j++){
                    t data_element = data[data_point];
                    int col = data_col_indices[data_point];
                    arr2d_ptr_revert_row[col] = data_element;
                    data_point++;
                }
            }
            changeFlag = false;
        }
    }

    template <class t>
    void CSR_container<t>::objDataCompress(){
        int data_len = rows_ptr[rows];
        t* data_temp = new t[data_len];
        int* data_col_indices_temp = new int[data_len];
        for(int i = 0; i<data_len; i++){
            data_temp[i] = data[i];
            data_col_indices_temp[i] = data_col_indices[i];
        }
        delete[] data;
        delete[] data_col_indices;
        data = data_temp;
        data_col_indices = data_col_indices_temp;
    }

    template <class t>
    void CSR_container<t>::toCSC(){
        int data_len = rows_ptr[rows];
        t* data_col = new t[data_len]{0};
        int* data_row_ind = new int[data_len]{0};
        int* cols_ptr = new int[cols+1]{0};

        int* col_count = new int[cols]{0};
        for(int i = 0; i<data_len; i++)
            col_count[data_col_indices[i]]++;
        for(int i = 1; i<cols+1; i++)
            cols_ptr[i] = cols_ptr[i-1] + col_count[i-1];
        
        int* col_count_newPlace = new (col_count) int[cols]{0};
        for (int r = 0; r < rows; r++) {
            for (int k = rows_ptr[r]; k < rows_ptr[r + 1]; k++) {
                int c = data_col_indices[k];
                int point = cols_ptr[c] + col_count_newPlace[c];
                data_col[point] = data[k];
                data_row_ind[point] = r;
                col_count_newPlace[c]++;
            }}

        delete[] col_count;
        delete[] data;
        delete[] data_col_indices;
        delete[] rows_ptr;
        data = data_col;
        data_col_indices = data_row_ind;
        rows_ptr = cols_ptr;
        int temp = rows;
        rows = cols;
        cols = temp;
    }

    template <class t>
    void CSR_container<t>::deleteRevertedArr() const{
        arr2d_ptr_revert.deleteArr2d();
        arr2d_ptr_revert.changePram(0, 0);
        arr2d_ptr_revert.changePram(false);
        changeFlag = true;
    }

    template <class t>
    void CSR_container<t>::printArr() const{
        std::cout << "rows=" << rows << "; ";
        std::cout << "cols=" << cols << std::endl;
        int data_len = rows_ptr[rows];
        for(int i = 0; i<data_len; i++){
            t temp = data[i];
            std::cout << temp;
            if(i < data_len-1)
                std::cout << ", ";
        }
        std::cout << std::endl;
        for(int i = 0; i<data_len; i++){
            int temp = data_col_indices[i];
            std::cout << temp;
            if(i < data_len-1)
                std::cout << ", ";
        }
        std::cout << std::endl;
        for(int i = 0; i<rows+1; i++){
            int temp = rows_ptr[i];
            std::cout << temp;
            if(i < rows)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    template <class t>
    void CSR_container<t>::printRevertArr() const{
        revert();
        arr2d_ptr_revert.printArr();
    }

    template <class t>
    typename CSR_container<t>::privateProxy CSR_container<t>::showBasicInfo() const{
        privateProxy proxy;
        proxy.rows = rows; proxy.cols = cols;
        proxy.data = const_cast<t*>(data);
        proxy.data_col_indices = const_cast<int*>(data_col_indices);
        proxy.rows_ptr = const_cast<int*>(rows_ptr);
        return proxy;
    }

    template <class t>
    void CSR_container<t>::setPtrNull(){
        data = nullptr;
        data_col_indices = nullptr;
        rows_ptr = nullptr;
        arr2d_ptr_revert.deleteArr2d();
        changeFlag = true;
    }

    template <class t1, class t2, class t3>
    void operator_right(const t1 a, CSR_container<t2>& result, const char modeFlag){
        int data_len = result.rows_ptr[result.rows];
        switch(modeFlag){
            case (char)0:
                for(int i = 0; i<data_len; i++)
                    result.data[i] += (t2)a;
                break;
            case (char)1:
                for(int i = 0; i<data_len; i++)
                    result.data[i] -= (t2)a;
                break;
            case (char)2:
                for(int i = 0; i<data_len; i++)
                    result.data[i] *= (t2)a;
                break;
            case (char)3:
                for(int i = 0; i<data_len; i++)
                    result.data[i] /= (t2)a;
                break;
            case (char)4:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t2)std::pow((double)result.data[i], (double)a);
                break;
        }
    }

    template <class t1, class t2, class t3>
    void operator_right(const t1& a, CSR_container<t2>& result, const char modeFlag){
        int data_len = result.rows_ptr[result.rows];
        switch(modeFlag){
            case (char)0:
                for(int i = 0; i<data_len; i++)
                    result.data[i] += (t2)a.data[i];
                break;
            case (char)1:
                for(int i = 0; i<data_len; i++)
                    result.data[i] -= (t2)a.data[i];
                break;
            case (char)2:
                for(int i = 0; i<data_len; i++)
                    result.data[i] *= (t2)a.data[i];
                break;
            case (char)3:
                for(int i = 0; i<data_len; i++)
                    result.data[i] /= (t2)a.data[i];
                break;
            case (char)4:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t2)std::pow((double)result.data[i], (double)a.data[i]);
                break;
        }
    }

    template <class t1, class t2, class t3>
    void operator_left(const t1 a, CSR_container<t2>& result, const char modeFlag){
        int data_len = result.rows_ptr[result.rows];
        switch(modeFlag){
            case (char)1:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t2)a - result.data[i];
                break;
            case (char)3:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t2)a / result.data[i];
                break;
            case (char)4:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t2)std::pow((double)a, (double)result.data[i]);
                break;
        }
    }

    template <class t1, class t2, class t3>
    void operator_left(const t1& a, CSR_container<t2>& result, const char modeFlag){
        int data_len = result.rows_ptr[result.rows];
        switch(modeFlag){
            case (char)1:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t2)a.data[i] - result.data[i];
                break;
            case (char)3:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t2)a.data[i] / result.data[i];
                break;
            case (char)4:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t2)std::pow((double)a.data[i], (double)result.data[i]);
                break;
        }
    }

    template <class t1, class t2>
    void operator_negativeSign(t1& result){
        int data_len = result.rows_ptr[result.rows];
        for(int i = 0; i<data_len; i++)
            result.data[i] = -result.data[i];
    }

}

#endif
