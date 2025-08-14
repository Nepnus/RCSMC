#include <typeinfo>
#include "arr2D.h"
#include "compressionByRow.h"
#ifndef COMPRESS_H_CREATE_
#define COMPRESS_H_CREATE_

namespace CSR{
    template <class t>
    CSR_container<t>::CSR_container(const t* data_, const int* data_col_indices_, const int* rows_ptr_, const int rows_, const int cols_, const bool addrCopy){
        rows = rows_; cols = cols_;
        changeFlag = true;
        arr2d_ptr_revert = arr2D::arr2d_container<t>();
        if(addrCopy){
            data = const_cast<t*>(data_);
            data_col_indices = const_cast<int*>(data_col_indices_);
            rows_ptr = const_cast<int*>(rows_ptr_);
        }
        else{
            int data_len = rows_ptr_[rows_];
            data = new t[data_len]{(t)0};
            data_col_indices = new int[data_len]{0};
            rows_ptr = new int[rows_+1]{0};
            for(int i = 0; i<data_len; i++){
                data[i] = data_[i];
                data_col_indices[i] = data_col_indices_[i];
            }
            for(int i = 1; i<rows_+1; i++){
                rows_ptr[i] = rows_ptr_[i];
            }
        }
    }

    template <class t>
    CSR_container<t>::CSR_container(const arr2D::arr2d_container<t>& arr2d_origin){
        rows = arr2d_origin.showRowNums(); cols = arr2d_origin.showColNums();
        rows_ptr = new int[rows+1]{0};
        data = new t[rows*cols]{(t)0};
        data_col_indices = new int[rows*cols]{0};
        changeFlag = true;
        arr2d_ptr_revert = arr2D::arr2d_container<t>();
        int data_p = 0;
        for(int i = 0; i<rows; i++){
            const auto rowArr = arr2d_origin[i];
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
    }

    template <class t>
    CSR_container<t>::CSR_container(const CSR_container<t>& CSR_copy){
        arr2d_ptr_revert = arr2D::arr2d_container<t>();
        rows = CSR_copy.rows; cols = CSR_copy.cols;
        changeFlag = true;
        int data_len = CSR_copy.rows_ptr[rows];
        data = new t[data_len]{(t)0};
        data_col_indices = new int[data_len]{0};
        rows_ptr = new int[rows+1]{0};
        for(int i = 0; i<data_len; i++){
            data[i] = CSR_copy.data[i];
            data_col_indices[i] = CSR_copy.data_col_indices[i];
        }
        for(int i = 1; i<rows+1; i++){
            rows_ptr[i] = CSR_copy.rows_ptr[i];
        }
    }

    template <class t>
    CSR_container<t>::CSR_container(CSR_container<t>&& CSR_copy){
        arr2d_ptr_revert = arr2D::arr2d_container<t>();
        rows = CSR_copy.rows; cols = CSR_copy.cols;
        changeFlag = true;
        data = CSR_copy.data;
        data_col_indices = CSR_copy.data_col_indices;
        rows_ptr = CSR_copy.rows_ptr;
        CSR_copy.data = nullptr;
        CSR_copy.data_col_indices = nullptr;
        CSR_copy.rows_ptr = nullptr;
        CSR_copy.arr2d_ptr_revert.deleteArr2d();
    }

    template <class t>
        template <class t1>
            CSR_container<t>::CSR_container(const CSR_container<t1>& CSR_convert, typename std::enable_if<!is_CSR_container<t1>::value && !std::is_same<t,t1>::value>::type* useless_){
                arr2d_ptr_revert = arr2D::arr2d_container<t>();
                auto CSR_convert_proxy = CSR_convert.showBasicInfo();
                rows = CSR_convert_proxy.rows; cols = CSR_convert_proxy.cols;
                changeFlag = true;
                int data_len = CSR_convert_proxy.rows_ptr[rows];
                data = new t[data_len]{(t)0};
                data_col_indices = new int[data_len]{0};
                rows_ptr = new int[rows+1]{0};
                for(int i = 0; i<data_len; i++){
                    data[i] = (t)CSR_convert_proxy.data[i];
                    data_col_indices[i] = CSR_convert_proxy.data_col_indices[i];
                }
                for(int i = 1; i<rows+1; i++){
                    rows_ptr[i] = CSR_convert_proxy.rows_ptr[i];
                }
            }

    template <class t>
        template <class t1>
            CSR_container<t>::CSR_container(CSR_container<t1>&& CSR_convert, typename std::enable_if<!is_CSR_container<t1>::value && !std::is_same<t,t1>::value>::type* useless_){
                arr2d_ptr_revert = arr2D::arr2d_container<t>();
                auto CSR_convert_proxy = CSR_convert.showBasicInfo();
                rows = CSR_convert_proxy.rows; cols = CSR_convert_proxy.cols;
                changeFlag = true;
                int data_len = CSR_convert_proxy.rows_ptr[rows];
                data = new t[data_len]{(t)0};
                for(int i = 0; i<data_len; i++)
                    data[i] = (t)CSR_convert_proxy.data[i];
                data_col_indices = CSR_convert_proxy.data_col_indices;
                rows_ptr = CSR_convert_proxy.rows_ptr;
                CSR_convert.setPtrNull();
                delete[] CSR_convert_proxy.data;
            }

    template <class t>
    CSR_container<t>::~CSR_container(){
        arr2d_ptr_revert.deleteArr2d();
        delete[] data;
        delete[] data_col_indices;
        delete[] rows_ptr;
        rows = 0; cols = 0;
        changeFlag = false;
    }

}

#endif

