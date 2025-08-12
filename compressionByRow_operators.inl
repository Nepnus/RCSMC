#include <type_traits>
#include <utility>
#include "arr2D.h"
#include "compressionByRow.h"
#ifndef COMPRESS_H_OPERATORS_
#define COMPRESS_H_OPERATORS_

namespace CSR{
    template <class t>
    CSR_container<t>& CSR_container<t>::operator=(const CSR_container<t>& CSR_eq){
        if(this != &CSR_eq){
            delete[] arr2d_ptr_revert.arr2d_ptr;
            arr2d_ptr_revert.arr2d_ptr = nullptr;
            arr2d_ptr_revert.rows = 0; arr2d_ptr_revert.cols = 0;
            arr2d_ptr_revert.transposeFlag = false;
            rows = CSR_eq.rows; cols = CSR_eq.cols;
            changeFlag = true;
            delete[] data;
            delete[] data_col_indices;
            delete[] rows_ptr;
            int data_len = CSR_eq.rows_ptr[rows];
            data = new t[data_len]{(t)0};
            data_col_indices = new int[data_len]{0};
            rows_ptr = new int[rows+1]{0};
            for(int i = 0; i<data_len; i++){
                data[i] = CSR_eq.data[i];
                data_col_indices[i] = CSR_eq.data_col_indices[i];
            }
            for(int i = 1; i<rows+1; i++){
                rows_ptr[i] = CSR_eq.rows_ptr[i];
            }
        }
        return *this;
    }

    template <class t>
    CSR_container<t>& CSR_container<t>::operator=(CSR_container<t>&& CSR_eq){
        if(this != &CSR_eq){
            delete[] arr2d_ptr_revert.arr2d_ptr;
            arr2d_ptr_revert.arr2d_ptr = nullptr;
            arr2d_ptr_revert.rows = 0; arr2d_ptr_revert.cols = 0;
            arr2d_ptr_revert.transposeFlag = false;
            rows = CSR_eq.rows; cols = CSR_eq.cols;
            changeFlag = true;
            delete[] data;
            delete[] data_col_indices;
            delete[] rows_ptr;
            data = CSR_eq.data;
            data_col_indices = CSR_eq.data_col_indices;
            rows_ptr = CSR_eq.rows_ptr;
            CSR_eq.data = nullptr;
            CSR_eq.data_col_indices = nullptr;
            CSR_eq.rows_ptr = nullptr;
            delete[] CSR_eq.arr2d_ptr_revert.arr2d_ptr;
            CSR_eq.arr2d_ptr_revert.arr2d_ptr = nullptr;
        }
        return *this;
    }

    template <class t>
    typename arr2D::arr2d_container<t>::RowProxy CSR_container<t>::operator[](const int& row){
        revert();
        typename arr2D::arr2d_container<t>::RowProxy rowData = arr2d_ptr_revert[row];
        return rowData;
    }

    template <class t>
        template <class t1>
            bool CSR_container<t>::operator==(const t1& a){
                bool flag = false;
                if(!is_CSR_container<t1>::value)
                    return flag;
                else{
                    if((a.rows != this->rows) || (a.cols != this->cols))
                        return flag;
                    for(int i = 1; i<a.rows+1; i++){
                        if(a.rows_ptr[i] != (*this).rows_ptr[i])
                            return flag;
                    }
                    int data_len = a.rows_ptr[a.rows];
                    for(int i = 0; i<data_len; i++){
                        if(a.data_col_indices[i] != (*this).data_col_indices[i])
                            return flag;
                        if((*this).data[i] != (t)a.data[i])
                            return flag;
                    }
                    flag = true;
                    return flag;
                }
            }

}

template <class t1, class t2, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator+(const t1& a, const t2& b){
    t1 result(a);
    CSR::operator_right(b, result, (char)0);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator+(t1&& a, const t2& b){
    t1 result(std::move(a));
    CSR::operator_right(b, result, (char)0);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<!CSR::is_CSR_container<t1>::value && CSR::is_CSR_container<t2>::value>::type>
t2 operator+(const t1& a, const t2& b){
    t2 result(b);
    CSR::operator_right(a, result, (char)0);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<!CSR::is_CSR_container<t1>::value && CSR::is_CSR_container<t2>::value>::type>
t2 operator+(const t1& a, t2&& b){
    t2 result(std::move(b));
    CSR::operator_right(a, result, (char)0);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator-(const t1& a, const t2& b){
    t1 result(a);
    CSR::operator_right(b, result, (char)1);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator-(t1&& a, const t2& b){
    t1 result(std::move(a));
    CSR::operator_right(b, result, (char)1);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<!CSR::is_CSR_container<t1>::value && CSR::is_CSR_container<t2>::value>::type>
t2 operator-(const t1& a, const t2& b){
    t2 result(b);
    CSR::operator_left(a, result, (char)1);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<!CSR::is_CSR_container<t1>::value && CSR::is_CSR_container<t2>::value>::type>
t2 operator-(const t1& a, t2&& b){
    t2 result(std::move(b));
    CSR::operator_left(a, result, (char)1);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator*(const t1& a, const t2& b){
    t1 result(a);
    CSR::operator_right(b, result, (char)2);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator*(t1&& a, const t2& b){
    t1 result(std::move(a));
    CSR::operator_right(b, result, (char)2);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<!CSR::is_CSR_container<t1>::value && CSR::is_CSR_container<t2>::value>::type>
t2 operator*(const t1& a, const t2& b){
    t2 result(b);
    CSR::operator_right(a, result, (char)2);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<!CSR::is_CSR_container<t1>::value && CSR::is_CSR_container<t2>::value>::type>
t2 operator*(const t1& a, t2&& b){
    t2 result(std::move(b));
    CSR::operator_right(a, result, (char)2);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator/(const t1& a, const t2& b){
    t1 result(a);
    CSR::operator_right(b, result, (char)3);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator/(t1&& a, const t2& b){
    t1 result(std::move(a));
    CSR::operator_right(b, result, (char)3);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<!CSR::is_CSR_container<t1>::value && CSR::is_CSR_container<t2>::value>::type>
t2 operator/(const t1& a, const t2& b){
    t2 result(b);
    CSR::operator_left(a, result, (char)3);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<!CSR::is_CSR_container<t1>::value && CSR::is_CSR_container<t2>::value>::type>
t2 operator/(const t1& a, t2&& b){
    t2 result(std::move(b));
    CSR::operator_left(a, result, (char)3);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator^(const t1& a, const t2& b){
    t1 result(a);
    CSR::operator_right(b, result, (char)4);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator^(t1&& a, const t2& b){
    t1 result(std::move(a));
    CSR::operator_right(b, result, (char)4);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<!CSR::is_CSR_container<t1>::value && CSR::is_CSR_container<t2>::value>::type>
t2 operator^(const t1& a, const t2& b){
    t2 result(b);
    CSR::operator_left(a, result, (char)4);
    return result;
}

template <class t1, class t2, typename = typename std::enable_if<!CSR::is_CSR_container<t1>::value && CSR::is_CSR_container<t2>::value>::type>
t2 operator^(const t1& a, t2&& b){
    t2 result(std::move(b));
    CSR::operator_left(a, result, (char)4);
    return result;
}

template <class t1, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator-(const t1& a){
    t1 result(a);
    CSR::operator_negativeSign(result);
    return result;
}

template <class t1, typename = typename std::enable_if<CSR::is_CSR_container<t1>::value>::type>
t1 operator-(t1&& a){
    t1 result(std::move(a));
    CSR::operator_negativeSign(result);
    return result;
}

#endif