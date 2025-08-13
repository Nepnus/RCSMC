#include "arr2D.h"
#include <type_traits>
#ifndef COMPRESS_H_
#define COMPRESS_H_

namespace CSR{
    template <class t> class CSR_container;

    template <typename T>
    struct is_CSR_container : std::false_type {};

    template <typename U>
    struct is_CSR_container<CSR_container<U> > : std::true_type {};

    template <class t1, class t2>
    void operator_right(const t2& a, t1& result, const char modeFlag, typename std::enable_if<is_CSR_container<t2>::value>::type* useless = nullptr);

    template <class t1, class t2>
    void operator_right(const t2 a, t1& result, const char modeFlag, typename std::enable_if<!is_CSR_container<t2>::value>::type* useless = nullptr);
    
    template <class t1, class t2>
    void operator_left(const t2& a, t1& result, const char modeFlag, typename std::enable_if<is_CSR_container<t2>::value>::type* useless = nullptr);
    
    template <class t1, class t2>
    void operator_left(const t2 a, t1& result, const char modeFlag, typename std::enable_if<!is_CSR_container<t2>::value>::type* useless = nullptr);

    template <class t1, typename = typename std::enable_if<is_CSR_container<t1>::value>::type>
    void operator_negativeSign(t1& result);

    template <class t>
    class CSR_container{
        private:
            mutable arr2D::arr2d_container<t> arr2d_ptr_revert;
            int rows, cols;
            t* data;
            int* data_col_indices;
            int* rows_ptr;
            mutable bool changeFlag;
        public:
            CSR_container(){
                arr2d_ptr_revert = arr2D::arr2d_container<t>();
                data = nullptr;
                data_col_indices = nullptr;
                rows_ptr = nullptr;
                rows = 0; cols = 0;
                changeFlag = false;
            }
            CSR_container(const arr2D::arr2d_container<t>& arr2d_origin);
            CSR_container(const t* data_, const int* data_col_indices_, const int* rows_ptr_, const int rows_, const int cols_);
            CSR_container(const CSR_container<t>& CSR_copy);
            CSR_container(CSR_container<t>&& CSR_copy);
            template <class t1>
            explicit CSR_container(const CSR_container<t1>& CSR_convert, typename std::enable_if<is_CSR_container<t1>::value && !std::is_same<t, t1>::value>::type* useless = nullptr);
            template <class t1>
            explicit CSR_container(CSR_container<t1>&& CSR_convert, typename std::enable_if<is_CSR_container<t1>::value && !std::is_same<t, t1>::value>::type* useless = nullptr);
            ~CSR_container();
            CSR_container<t>& operator=(const CSR_container<t>& CSR_eq);
            CSR_container<t>& operator=(CSR_container<t>&& CSR_eq);
            void rowSum(t* sumResult) const;
            t sum() const;
            void revert() const;
            void objDataCompress();
            void toCSC();
            void deleteRevertedArr() const;
            void printArr() const;
            void printRevertArr() const;
            auto operator[](const int row);
            const auto operator[](const int row) const;
            template <class t1>
            bool operator==(const t1& a) const;
            template <class t1, class t2>
            friend void operator_right(const t2& a, t1& result, const char modeFlag, typename std::enable_if<is_CSR_container<t2>::value>::type* useless);
            template <class t1, class t2>
            friend void operator_right(const t2 a, t1& result, const char modeFlag, typename std::enable_if<!is_CSR_container<t2>::value>::type* useless);
            template <class t1, class t2>
            friend void operator_left(const t2& a, t1& result, const char modeFlag, typename std::enable_if<is_CSR_container<t2>::value>::type* useless);
            template <class t1, class t2>
            friend void operator_left(const t2 a, t1& result, const char modeFlag, typename std::enable_if<!is_CSR_container<t2>::value>::type* useless);
            template <class t1, class t2>
            friend void operator_negativeSign(t1& result);
    };

}


#include "compressionByRow_create.inl"
#include "compressionByRow_methods.inl"
#include "compressionByRow_operators.inl"
#endif
