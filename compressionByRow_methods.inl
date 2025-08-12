#include <cmath>
#include <type_traits>
#include "compressionByRow.h"
#ifndef COMPRESS_H_METHODS_
#define COMPRESS_H_METHODS_

namespace CSR{
    template <class t>
    void CSR_container<t>::rowSum(t* sumResult){
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
    t CSR_container<t>::sum(){
        t sumResult = (t)0;
        int data_len = rows_ptr[rows];
        for(int i = 0; i<data_len; i++){
            sumResult += data[i];
        }
        return sumResult;
    }

    template <class t>
    void CSR_container<t>::revert(){
        if(changeFlag){
            delete[] arr2d_ptr_revert.arr2d_ptr;
            t* arr = new t[rows*cols]{(t)0};
            arr2d_ptr_revert.arr2d_ptr = arr;
            arr2d_ptr_revert.rows = rows;
            arr2d_ptr_revert.cols = cols;
            int data_point = 0;
            for(int i = 0; i<rows; i++){
                int rowNotZeroNums = rows_ptr[i+1] - rows_ptr[i];
                auto arr2d_ptr_revert_row = arr2d_ptr_revert[i];
                for(int j = 0; j<rowNotZeroNums; j++){
                    t data_element = data[data_point];
                    int col = rows_ptr[data_point];
                    arr2d_ptr_revert_row[col] = data_element;
                    data_point++;
                }
            }
            changeFlag = false;
        }
    }

    template <class t>
    void CSR_container<t>::objDataCompress(){
        bool canCompressFlag = true;
        int data_len = rows_ptr[rows];
        try{
            data[data_len];
        }catch(...){
            canCompressFlag = false;
        }
        if(canCompressFlag){
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
    }

    template <class t>
    void CSR_container<t>::toCSC(){
        revert();
        int data_len = rows_ptr[rows];
        arr2d_ptr_revert.transpose();
        rows = arr2d_ptr_revert.rows;
        cols = arr2d_ptr_revert.cols;
        delete[] data;
        delete[] data_col_indices;
        delete[] rows_ptr;
        rows_ptr = new int[rows+1]{0};
        data = new t[data_len]{(t)0};
        data_col_indices = new int[data_len]{0};
        int data_p = 0;
        for(int i = 0; i<rows; i++){
            auto rowArr = arr2d_ptr_revert[i];
            int count = 0;
            for(int j = 0; j<cols; j++){
                if(rowArr[j] != (t)0){
                    count++;
                    data[data_p] = rowArr[j];
                    data_col_indices[data_p] = j;
                    data_p++;
                }
            }
            rows_ptr[i+1] = rows_ptr[i] + count;
        }
        rows = arr2d_ptr_revert.cols;
        cols = arr2d_ptr_revert.rows;
    }

    template <class t>
    void CSR_container<t>::deleteRevertedArr(){
        delete[] arr2d_ptr_revert.arr2d_ptr;
        arr2d_ptr_revert.arr2d_ptr = nullptr;
        arr2d_ptr_revert.rows = 0;
        arr2d_ptr_revert.cols = 0;
        arr2d_ptr_revert.transposeFlag = false;
        changeFlag = true;
    }

    template <class t1, class t2>
    void operator_right(const t2& a, t1& result, const char& modeFlag, typename std::enable_if<!is_CSR_container<t2>::value>::type* useless_){
        int data_len = result.rows_ptr[result.rows];
        switch(modeFlag){
            case (char)0:
                for(int i = 0; i<data_len; i++)
                    result.data[i] += (t1)a;
                break;
            case (char)1:
                for(int i = 0; i<data_len; i++)
                    result.data[i] -= (t1)a;
                break;
            case (char)2:
                for(int i = 0; i<data_len; i++)
                    result.data[i] *= (t1)a;
                break;
            case (char)3:
                for(int i = 0; i<data_len; i++)
                    result.data[i] /= (t1)a;
                break;
            case (char)4:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t1)std::pow((double)result.data[i], (double)a);
                break;
        }
    }

    template <class t1, class t2>
    void operator_right(const t2& a, t1& result, const char& modeFlag, typename std::enable_if<is_CSR_container<t2>::value>::type* useless_){
        int data_len = result.rows_ptr[result.rows];
        switch(modeFlag){
            case (char)0:
                for(int i = 0; i<data_len; i++)
                    result.data[i] += (t1)a.data[i];
                break;
            case (char)1:
                for(int i = 0; i<data_len; i++)
                    result.data[i] -= (t1)a.data[i];
                break;
            case (char)2:
                for(int i = 0; i<data_len; i++)
                    result.data[i] *= (t1)a.data[i];
                break;
            case (char)3:
                for(int i = 0; i<data_len; i++)
                    result.data[i] /= (t1)a.data[i];
                break;
            case (char)4:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t1)std::pow((double)result.data[i], (double)a.data[i]);
                break;
        }
    }

    template <class t1, class t2>
    void operator_left(const t2& a, t1& result, const char& modeFlag, typename std::enable_if<!is_CSR_container<t2>::value>::type* useless_){
        int data_len = result.rows_ptr[result.rows];
        switch(modeFlag){
            case (char)1:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t1)a - result.data[i];
                break;
            case (char)3:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t1)a / result.data[i];
                break;
            case (char)4:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t1)std::pow((double)a, (double)result.data[i]);
                break;
        }
    }

    template <class t1, class t2>
    void operator_left(const t2& a, t1& result, const char& modeFlag, typename std::enable_if<is_CSR_container<t2>::value>::type* useless_){
        int data_len = result.rows_ptr[result.rows];
        switch(modeFlag){
            case (char)1:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t1)a.data[i] - result.data[i];
                break;
            case (char)3:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t1)a.data[i] / result.data[i];
                break;
            case (char)4:
                for(int i = 0; i<data_len; i++)
                    result.data[i] = (t1)std::pow((double)a.data[i], (double)result.data[i]);
                break;
        }
    }

    template <class t1, typename = typename std::enable_if<is_CSR_container<t1>::value>::type>
    void operator_negativeSign(t1& result){
        int data_len = result.rows_ptr[result.rows];
        for(int i = 0; i<data_len; i++)
            result.data[i] = -result.data[i];
    }

}

#endif