#include <vector>
#include <cstddef>
#include "AccDefs.h"
#include <bit>

template<typename T, typename Alloc = std::allocator<T>>
class Vector
{

private:

    typedef Vector<T, Alloc> Self;

public:

    typedef T                                       value_type;
    typedef Alloc                                   allocator_type;
    typedef std::size_t                             size_type;
    typedef std::ptrdiff_t                          difference_type;
    typedef T&                                      reference;
    typedef const T&                                const_reference;

private:

    typedef std::allocator_traits<allocator_type>   AllocVal;

public:

    typedef typename AllocVal::pointer              pointer;
    typedef typename AllocVal::const_pointer        const_pointer;

public:

    void push_back(const value_type& x)
    {
        if ((size & -size) == size) { // 0 or 1 << x
            expand();
        }
        *at_unsafe(size++) = x;
    }

private:

    pointer* head;

    typedef typename AllocVal::rebind_traits<pointer> AllocPtr;

    size_type size;

    allocator_type alloc;

    void expand()
    {
        size_type head_size = std::bit_width(size);
        pointer* temp = AllocPtr::allocate(alloc, head_size + 1ull);
        for (size_type i = 0; i < head_size; ++i) {
            AllocPtr::construct(alloc, temp + i, *(head + i));
            AllocPtr::destroy(alloc, head + i);
        }
        if (size == 0ull) {
            AllocPtr::construct(alloc, temp + head_size, AllocVal::allocate(alloc, 1ull));
        }
        else {
            AllocPtr::construct(alloc, temp + head_size, AllocVal::allocate(alloc, size));
            AllocPtr::deallocate(alloc, head, head_size);
        }
        head = temp;
    }

    pointer at_unsafe(size_type pos)
    {
        return head[std::bit_width(pos)] + (pos - std::bit_floor(pos));
    }

};