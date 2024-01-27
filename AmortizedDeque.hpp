#include <vector>
#include <cstddef>
#include <cassert>
#include <iostream>
#include <type_traits>

#ifndef _AMORTIZED_DEQUE
#define _AMORTIZED_DEQUE

template<typename T, typename Alloc = std::allocator<T>>
class AmortizedDeque
{

private:

    using Self = AmortizedDeque<T>;
    using Vec = std::vector<T, Alloc>;

public:

    template<typename Ref, typename Ptr>
    class _Iterator
    {
    private:

        template<typename _Ptr, typename _Tp>
            using __ptr_rebind = typename std::pointer_traits<_Ptr>::template rebind<_Tp>;

        template<typename _CvTp> using IterTempl = _Iterator<_CvTp&, __ptr_rebind<Ptr, _CvTp>>;

        using Iter = _Iterator<Ref, Ptr>;

    public:
        typedef T                                           value_type;
        typedef size_t                                      size_type;
        typedef ptrdiff_t                                   difference_type;
        typedef Ref                                         reference;
        typedef Ptr                                         pointer;
        typedef IterTempl<T>                                iterator;
        typedef IterTempl<const T>                          const_iterator;
        typedef std::random_access_iterator_tag             iterator_category;
    
    private:

        pointer get() const { return s->at_unsafe(cur); }

    public:

        size_type cur;
        Self* s;

        _Iterator(): cur(), s(nullptr) { }
        _Iterator(size_type _c, Self* _s): cur(_c), s(_s) { }
        template<typename _Iter,
            typename = std::enable_if<std::is_same<Iter, const_iterator>::value 
                                      &&std::is_same<_Iter, iterator>::value>>
        _Iterator(const _Iter& __x): cur(__x.cur), s(__x.s) { }
        _Iterator(const _Iterator& __x): cur(__x.cur), s(__x.s) { }
        _Iterator& operator=(const _Iterator& t) = default;

        reference operator*() const { return *get(); }
        pointer operator->() const { return get(); }
        reference operator[](difference_type n) const { return *((*this) + n); }

        Iter& operator++() { ++cur; return (*this); }
        Iter operator++(int) 
        {
            Iter copy = *this;
            ++cur;
            return copy;
        }
        Iter& operator+=(difference_type n) { cur += n; return (*this); }
        Iter operator+(difference_type n) const { return Iter(cur + n, s); }
        friend Iter operator+(difference_type n, Iter th) { return Iter(th.cur + n, th.s); }

        Iter& operator--() { --cur; return (*this); }
        Iter operator--(int) 
        {
            Iter copy = (*this);
            --cur;
            return copy;
        }
        Iter operator-=(difference_type n) { cur -= n; return (*this); }
        Iter operator-(difference_type n) const { return Iter(cur - n, s); }
        difference_type operator-(const Iter& t) const
        {
            return static_cast<difference_type>(cur)
                  -static_cast<difference_type>(t.cur);
        }

        bool operator==(const Iter& t) const { return s == t.s && cur == t.cur; }
        bool operator!=(const Iter& t) const { return s != t.s || cur != t.cur; }
        bool operator<(const Iter& t) const { return (t - (*this)) > 0; }
        bool operator>(const Iter& t) const { return t < (*this); }
        bool operator>=(const Iter& t) const { return !((*this) < t); }
        bool operator<=(const Iter& t) const { return !((*this) > t); }
    };

    typedef T                                       value_type;
    typedef Alloc                                   allocator_type;
    typedef typename Vec::size_type                 size_type;
    typedef typename Vec::difference_type           difference_type;
    typedef typename Vec::reference                 reference;
    typedef const typename Vec::const_reference     const_reference;
    typedef typename Vec::iterator                  pointer;
    typedef typename Vec::const_iterator            const_pointer;
    typedef _Iterator<T&, pointer>                  iterator;
    typedef _Iterator<const T&, const_pointer>      const_iterator;
    typedef std::reverse_iterator<iterator>         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;


    explicit AmortizedDeque(const allocator_type& alloc): pre(alloc), suf(alloc) { }
    AmortizedDeque(): AmortizedDeque(allocator_type()) { }
    AmortizedDeque(size_type count, const value_type& value, 
        const allocator_type& alloc = allocator_type()): pre(alloc), suf(count, value, alloc) { }

    explicit AmortizedDeque(size_type count, const allocator_type& alloc = allocator_type())
        : pre(alloc), suf(count, alloc) { }

    template<typename InputIt> 
    AmortizedDeque(InputIt first, InputIt last, const allocator_type& alloc = allocator_type())
        : pre(alloc), suf(first, last, alloc) { }

    AmortizedDeque(const Self& other): pre(other.pre), suf(other.suf) { }
    AmortizedDeque(const Self& other, const allocator_type& alloc = allocator_type())
        : pre(other.pre, alloc), suf(other.suf, alloc) { }
    AmortizedDeque(Self&& other): AmortizedDeque() 
    {
        pre.swap(other.pre);
        suf.swap(other.suf);
    }
    AmortizedDeque(std::initializer_list<value_type> init, const allocator_type& alloc = allocator_type())
        : pre(alloc), suf(init, alloc) { }

    AmortizedDeque operator=(const Self& other)
    {
        pre = other.pre, suf = other.suf;
    }
    AmortizedDeque operator=(Self&& other)
    {
        pre.swap(other.pre);
        suf.swap(other.suf);
    }
    AmortizedDeque operator=(std::initializer_list<value_type> ilist)
    {
        pre.clear();
        suf.assign(ilist);
    }

    void assign(size_type count, const value_type& value)
    {
        pre.clear();
        suf.assign(count, value);
    }
    template<class InputIt>
    void assign(InputIt first, InputIt last)
    {
        pre.clear();
        suf.assign(first, last);
    }
    void assign(std::initializer_list<value_type> ilist)
    {
        pre.clear();
        suf.assign(ilist);
    }
    
    allocator_type get_allocator() const noexcept
    {
        return suf.get_allocator();
    }

    reference operator[](size_type pos)
    {
        return *at_unsafe(pos);
    }

    size_type const size() { return pre.size() + suf.size(); }

    reference at(size_type pos) const
    {
        range_check(pos);
        return *at_unsafe(pos);
    }

    reference front() 
    {
        if (pre.empty()) return suf.front();
        return pre.back();
    }
    reference back() 
    {
        if (suf.empty()) return pre.front();
        return suf.back();
    }

    iterator const begin() { return iterator(0u, this); }
    const_iterator const cbegin() { return const_iterator(0u, this); }
    iterator const end() { return iterator(size(), this); }
    const_iterator const cend() { return const_iterator(size(), this); }
    reverse_iterator const rbegin() { return reverse_iterator(size() - 1u, this); }
    const_reverse_iterator const crbegin() { return const_reverse_iterator(size() - 1u, this); }
    reverse_iterator const rend() { return reverse_iterator(1u, this); }
    const_reverse_iterator const crend() { return const_reverse_iterator(1u, this); }

    bool empty() const
    {
        return pre.empty() && suf.empty();
    }

    size_type max_size() const { return pre.max_size(); }

    void shrink_to_fit() 
    {
        pre.shrink_to_fit();
        suf.shrink_to_fit();
    }

    void clear() 
    {
        pre.clear();
        suf.clear();
    }

    void insert(const_iterator pos, const value_type& val) 
    {
        size_type dis = pos.cur;
        if (dis < pre.size()) {
            pre.insert(pre.cbegin() + (pre.size() - dis + 1u), val); ///
        }
        else {
            suf.insert(suf.cbegin() + (dis - pre.size()), val);
        }
    }

    template<class... Args>
    void emplace(const_iterator pos, Args&&... args) 
    {
        size_type dis = pos.cur;
        if (dis < pre.size()) {
            pre.emplace(pre.cbegin() + (pre.size() - dis + 1u), args...); ///
        }
        else {
            suf.emplace(suf.cbegin() + (dis - pre.size()), args...);
        }
    }

    iterator earse(const_iterator pos) 
    {
        size_type dis = pos.cur;
        iterator ret(pos.cur + 1, pos.s);
        if (dis < pre.size()) {
            pre.erase(pre.cbegin() + (pre.size() - dis)); ///
        }
        else suf.erase(suf.cbegin() + (dis - pre.size()));
        return ret;
    }

    iterator earse(const_iterator first, const_iterator last)
    {
        size_type from = first.cur, to = last.cur;
        if (from < pre.size() && to > pre.size()) {
            pre.erase(pre.cbegin(), pre.cbegin() + (pre.size() - from));
            suf.erase(suf.cbegin(), suf.cbegin() + (to - pre.size()));
        }
        else if (from > pre.size()) {
            suf.erase(suf.cbegin() + (from - pre.size()),
                      suf.cbegin() + (to - pre.size()));
        }
        else { ///
            pre.erase(pre.cbegin() + (pre.size() - to),
                      pre.cbegin() + (pre.size() - from));
        }
    }

    void push_back(const value_type& val) 
    {
        suf.push_back(val);
    }

    template<class... Args>
    void emplace_back(Args&&... args) 
    {
        suf.emplace_back(args...);
    }

    void pop_back() 
    {
        if (suf.empty()) rebuild();
        suf.pop_back();
    }

    void push_front(const value_type& val) 
    {
        pre.push_back(val);
    }

    template<class... Args>
    void emplace_front(Args&&... args) 
    {
        pre.emplace_back(args...);
    }

    void pop_front() 
    {
        if (pre.empty()) rebuild();
        pre.pop_back();
    }

    void swap(Self& t) {
        pre.swap(t.pre);
        suf.swap(t.suf);
    }

    void resize(size_type new_size)
    {
        if (new_size > size()) {
            suf.resize(suf.size() + (new_size - size()));
        }
    }

    void reserve_front(size_type new_size)
    {
        if (new_size > pre.size()) pre.reserve(new_size);
    }

    void reserve_back(size_type new_size)
    {
        if (new_size > suf.size()) suf.reserve(new_size);
    }

private:

    Vec pre;
    Vec suf;

    pointer at_unsafe(size_type pos)
    {
        if (pos < pre.size()) return (pre.begin() + pre.size() - pos - 1);
        return (suf.begin() + (pos - pre.size()));
    }

    void range_check(size_type pos)
    {
        if (pos >= size())
        {
            std::cerr << "AmortizedDeque::range_check: pos (which is " << pos 
                    << ") >= this->size() (which is " << size() << ")\n";
            assert(false);
        }
    }

    void rebuild()
    {
        if (pre.empty()) {
            size_type mid = suf.size() / 2;
            pre = Vec(suf.rbegin() + mid, suf.rend());
            suf = Vec(suf.end() - mid, suf.end());
        }
        else if (suf.empty()) {
            size_type mid = pre.size() / 2;
            suf = Vec(pre.rbegin() + mid, pre.rend());
            pre = Vec(pre.end() - mid, pre.end());
        }
    }

};

#endif