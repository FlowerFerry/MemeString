
#ifndef MEGOPP_UTIL_TEMPLATE_INDEXER_H_INCLUDED
#define MEGOPP_UTIL_TEMPLATE_INDEXER_H_INCLUDED

#include <type_traits>

namespace megopp {
namespace util {

//! \brief Indexer for types
//! \details
//!     This class is used to get the index of a type in a list of types.
//!     The index is 0-based.
//!     该类用于获取类型在类型列表中的索引。
//!     索引从0开始。
//! \tparam Types List of types
//! \example
//!     using types = type_list<int, float, double>;
//!     template_indexer<types>::index<int>(); // 0
//!     template_indexer<types>::index<float>(); // 1
//!     template_indexer<types>::index<double>(); // 2
template <typename... Types>
struct template_indexer {
    template <typename T>
    static constexpr int index() {
        return find_index<T, Types...>();
    }

private:
    template <typename T, typename U, typename... Rest>
    static constexpr int find_index() {
        if constexpr(std::is_same_v<T, U>) {
            return 0;
        } else {
            return 1 + find_index<T, Rest...>();
        }
    }

    template <typename T>
    static constexpr int find_index() {
        return -1; // Not found
    }
};


};
};

#endif // !MEGOPP_UTIL_TEMPLATE_INDEXER_H_INCLUDED
