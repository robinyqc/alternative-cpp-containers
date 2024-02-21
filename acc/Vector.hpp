#ifndef _ACC_VECTOR
#define _ACC_VECTOR

#include <vector>
#include <cstddef>
#include <type_traits>

#include "AccBit.hpp"
#include "IndexingIterator.hpp"

namespace acc
{

template<typename T, typename Alloc = std::allocator<T>>
class Vector
{

private:

    typedef Vector<T, Alloc> Self;

public:

    DERIVE_ACC_INDEXING_ITERATOR(_Iterator, at_unsafe)

    typedef T                                           value_type;
    typedef Alloc                                       allocator_type;
    typedef std::size_t                                 size_type;
    typedef std::ptrdiff_t                              difference_type;
    typedef T&                                          reference;
    typedef const T&                                    const_reference;

private:

    typedef std::allocator_traits<allocator_type>   AllocVal;

public:

    typedef typename AllocVal::pointer              pointer;
    typedef typename AllocVal::const_pointer        const_pointer;
    typedef _Iterator<T&, pointer>                      iterator;
    typedef _Iterator<const T&, const_pointer>          const_iterator;
    typedef std::reverse_iterator<iterator>             reverse_iterator;
    typedef std::reverse_iterator<const_iterator>       const_reverse_iterator;

public:

    constexpr size_type size() const noexcept
    {
        return vec_impl.size;
    }

    void push_back(const value_type& x)
    {
        if ((size() & -size()) == size()) { // 0 or 1 << x
            expand();
        }
        *at_unsafe(size()++) = x;
    }

    allocator_type get_allocator()
    {
        return allocator_type(vec_impl);
    }

private:


    typedef typename AllocVal::rebind_traits<pointer> AllocPtr;

    struct _VecData
    {
        pointer* head;
        size_type size;

        constexpr _VecData() noexcept : head(), size() { }

        constexpr
        _VecData(_VecData&& __x) noexcept
            : head(__x.head), size(__x.size)
        { 
            __x.head = (__x.size = pointer()); 
        }

    };

    struct _VecImpl : allocator_type, _VecData
    {
        constexpr _VecImpl() noexcept(
            std::is_nothrow_default_constructible<allocator_type>::value)
        : allocator_type()
        { }

        constexpr
        _VecImpl(allocator_type const& __a) noexcept
            : allocator_type(__a)
        { }

        constexpr
        _VecImpl(_VecImpl&& __x) noexcept
            : allocator_type(std::move(__x)), _VecData(std::move(__x))
        { }

        constexpr
        _VecImpl(allocator_type&& __a) noexcept
            : allocator_type(std::move(__a))
        { }

        constexpr
        _VecImpl(allocator_type&& __a, _VecImpl&& __rv) noexcept
            : allocator_type(std::move(__a)), _VecData(std::move(__rv))
        { }
    };
    
    _VecImpl vec_impl;

    constexpr pointer* head() const noexcept
    {
        return vec_impl.head;
    }

    void expand()
    {
        size_type head_size = acc::bit_width(size());
        allocator_type alloc = get_allocator();
        pointer* temp = AllocPtr::allocate(alloc, head_size + 1ull);
        for (size_type i = 0; i < head_size; ++i) {
            AllocPtr::construct(alloc, temp + i, *(size() + i));
            AllocPtr::destroy(alloc, head() + i);
        }
        if (size() == 0ull) {
            AllocPtr::construct(alloc, temp + head_size, AllocVal::allocate(alloc, 1ull));
        }
        else {
            AllocPtr::construct(alloc, temp + head_size, AllocVal::allocate(alloc, size()));
            AllocPtr::deallocate(alloc, head(), head_size);
        }
        head() = temp;
    }

    pointer at_unsafe(size_type pos)
    {
        return head()[acc::bit_width(pos)] + (pos - acc::bit_floor(pos));
    }

};

}


#endif