// Iterator template by indexing.

#ifndef _ACC_INDEXING_ITERATOR
#define _ACC_INDEXING_ITERATOR

#include <type_traits>
#include <iterator>

#define DERIVE_ACC_INDEXING_ITERATOR(iterator_name, to_pointer) \
template<typename Ref, typename Ptr> \
class iterator_name \
{ \
private: \
\
    typedef iterator_name<Ref, Ptr> Iter; \
\
public: \
    typedef typename Self::value_type                   value_type; \
    typedef std::size_t                                 size_type; \
    typedef std::ptrdiff_t                              difference_type; \
    typedef Ref                                         reference; \
    typedef Ptr                                         pointer; \
    typedef std::random_access_iterator_tag             iterator_category; \
\
private: \
\
    pointer get() const { return s->to_pointer(cur); } \
\
public: \
\
    difference_type cur; \
    Self* s; \
\
    iterator_name(): cur(), s(nullptr) { } \
    iterator_name(difference_type _c, Self* _s): cur(_c), s(_s) { } \
    iterator_name(difference_type _c, const Self* _s): cur(_c), s(const_cast<Self*>(_s)) { } \
    template<typename _Iter, \
        typename = std::enable_if< \
            std::is_same<Iter, iterator_name<const T&, typename Self::const_pointer>>::value \
            &&std::is_same<_Iter, iterator_name<T&, typename Self::pointer>>::value>> \
    iterator_name(const _Iter& __x): cur(__x.cur), s(__x.s) { } \
    iterator_name(const iterator_name& __x): cur(__x.cur), s(__x.s) { } \
    iterator_name& operator=(const iterator_name& t) = default; \
\
    reference operator*() const { return *get(); } \
    pointer operator->() const { return get(); } \
    reference operator[](difference_type n) const { return *((*this) + n); } \
\
    Iter& operator++() { ++cur; return (*this); } \
    Iter operator++(int) \
    { \
        Iter copy = *this; \
        ++cur; \
        return copy; \
    } \
    Iter& operator+=(difference_type n) { cur += n; return (*this); } \
    Iter operator+(difference_type n) const { return Iter(cur + n, s); } \
    friend Iter operator+(difference_type n, Iter th) { return Iter(th.cur + n, th.s); } \
\
    Iter& operator--() { --cur; return (*this); } \
    Iter operator--(int) \
    { \
        Iter copy = (*this); \
        --cur; \
        return copy; \
    } \
    Iter operator-=(difference_type n) { cur -= n; return (*this); } \
    Iter operator-(difference_type n) const { return Iter(cur - n, s); } \
    difference_type operator-(const Iter& t) const { return cur - t.cur; } \
\
    bool operator==(const Iter& t) const { return s == t.s && cur == t.cur; } \
    bool operator!=(const Iter& t) const { return s != t.s || cur != t.cur; } \
    bool operator<(const Iter& t) const { return (t - (*this)) > 0; } \
    bool operator>(const Iter& t) const { return t < (*this); } \
    bool operator>=(const Iter& t) const { return !((*this) < t); } \
    bool operator<=(const Iter& t) const { return !((*this) > t); } \
}; 

#endif