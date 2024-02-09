// A simple deque implementation.

// Copyright (C) 2024 Robin Ye (robinyqc@163.com).

// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.

// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
// See the GNU General Public License for more details.

// You should have received a copy of the GNU General Public License 
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include <vector>
#include <cstddef>
#include <iostream>
#include <utility>
#include <algorithm>
#include <type_traits>
#include "AccDefs.h"

#ifndef _ACC_DEQUE
#define _ACC_DEQUE

namespace acc
{

#ifndef USE_EXTRA_ACC_DEQUE_OPT
#ifdef USE_EXTRA_ACC_OPT
#define USE_EXTRA_ACC_DEQUE_OPT
#endif
#endif

#if _CPPVERSION >= 201100L

template<typename T, typename Container = std::vector<T>>
class Deque
{

private:

    typedef Deque<T> Self;
    typedef Container Vec;

public:

    template<typename Ref, typename Ptr>
    class _Iterator
    {
    private:

        typedef _Iterator<Ref, Ptr> Iter;

    public:
        typedef T                                           value_type;
        typedef size_t                                      size_type;
        typedef ptrdiff_t                                   difference_type;
        typedef Ref                                         reference;
        typedef Ptr                                         pointer;
        typedef std::random_access_iterator_tag             iterator_category;
    
    private:

        pointer get() const { return s->get_unsafe(cur); }

    public:

        difference_type cur;
        Self* s;

        _Iterator(): cur(), s(nullptr) { }
        _Iterator(difference_type _c, Self* _s): cur(_c), s(_s) { }
        _Iterator(difference_type _c, const Self* _s): cur(_c), s(const_cast<Self*>(_s)) { }
        template<typename _Iter,
            typename = std::enable_if<
                std::is_same<Iter, _Iterator<const T&, typename Self::const_pointer>>::value 
                &&std::is_same<_Iter, _Iterator<T&, typename Self::pointer>>::value>>
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
        difference_type operator-(const Iter& t) const { return cur - t.cur; }

        bool operator==(const Iter& t) const { return s == t.s && cur == t.cur; }
        bool operator!=(const Iter& t) const { return s != t.s || cur != t.cur; }
        bool operator<(const Iter& t) const { return (t - (*this)) > 0; }
        bool operator>(const Iter& t) const { return t < (*this); }
        bool operator>=(const Iter& t) const { return !((*this) < t); }
        bool operator<=(const Iter& t) const { return !((*this) > t); }
    };

    typedef T                                       value_type;
    typedef typename Vec::allocator_type            allocator_type;
    typedef typename Vec::size_type                 size_type;
    typedef typename Vec::difference_type           difference_type;
    typedef typename Vec::reference                 reference;
    typedef typename Vec::const_reference           const_reference;
    typedef typename Vec::iterator                  pointer;
    typedef typename Vec::const_iterator            const_pointer;
    typedef _Iterator<T&, pointer>                  iterator;
    typedef _Iterator<const T&, const_pointer>      const_iterator;
    typedef std::reverse_iterator<iterator>         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;


    explicit Deque(const allocator_type& alloc): pre(alloc), suf(alloc) { }
    Deque(): Deque(allocator_type()) { }
    Deque(size_type count, const value_type& value, 
        const allocator_type& alloc = allocator_type()): pre(alloc), suf(count, value, alloc) { }

    explicit Deque(size_type count, const allocator_type& alloc = allocator_type())
        : pre(alloc), suf(count, alloc) { }

    template<typename InputIt> 
    Deque(InputIt first, InputIt last, const allocator_type& alloc = allocator_type())
        : pre(alloc), suf(first, last, alloc) { }

    Deque(const Self& other, const allocator_type& alloc = allocator_type())
        : pre(other.pre, alloc), suf(other.suf, alloc) { }
    Deque(Self&& other): Deque() 
    {
        pre.swap(other.pre);
        suf.swap(other.suf);
    }
    Deque(std::initializer_list<value_type> init, const allocator_type& alloc = allocator_type())
        : pre(alloc), suf(init, alloc) { }

    Deque operator=(const Self& other)
    {
        pre = other.pre, suf = other.suf;
    }
    Deque operator=(Self&& other)
    {
        pre.swap(other.pre);
        suf.swap(other.suf);
    }
    Deque operator=(std::initializer_list<value_type> ilist)
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
        return *get_unsafe(static_cast<difference_type>(pos) 
                          - static_cast<difference_type>(pre.size())); 
    }
    const_reference operator[](size_type pos) const 
    { 
        return *get_const_unsafe(static_cast<difference_type>(pos) 
                                - static_cast<difference_type>(pre.size())); 
    }

    reference at(size_type pos)
    {
        range_check(pos);
        return *get_unsafe(static_cast<difference_type>(pos) 
                          - static_cast<difference_type>(pre.size()));
    }
    const_reference at(size_type pos) const
    {
        range_check(pos);
        return *get_const_unsafe(static_cast<difference_type>(pos) 
                                - static_cast<difference_type>(pre.size()));
    }

    size_type size() const { return pre.size() + suf.size(); }

    reference front()
    {
        if (pre.empty()) return suf.front();
        return pre.back();
    }
    const_reference front() const
    {
        if (pre.empty()) return suf.front();
        return pre.back();
    }

    reference back()
    {
        if (suf.empty()) return pre.front();
        return suf.back();
    }
    const_reference back() const
    {
        if (suf.empty()) return pre.front();
        return suf.back();
    }

    iterator begin() { return iterator(-static_cast<difference_type>(pre.size()), this); }
    const_iterator begin() const { return iterator(-static_cast<difference_type>(pre.size()), this); }
    const_iterator cbegin() const { return const_iterator(-static_cast<difference_type>(pre.size()), this); }
    iterator end() { return iterator(suf.size(), this); }
    const_iterator end() const { return iterator(suf.size(), this); }
    const_iterator cend() const { return const_iterator(suf.size(), this); }
    reverse_iterator rbegin()
    { 
        return reverse_iterator(static_cast<difference_type>(suf.size()) - 1, this); 
    }
    const_reverse_iterator rbegin() const
    { 
        return reverse_iterator(static_cast<difference_type>(suf.size()) - 1, this); 
    }
    const_reverse_iterator crbegin() const
    { 
        return const_reverse_iterator(static_cast<difference_type>(suf.size()) - 1, this); 
    }
    reverse_iterator rend() 
    { 
        return reverse_iterator(-static_cast<difference_type>(pre.size()) - 1, this); 
    }
    const_reverse_iterator rend() const
    { 
        return reverse_iterator(-static_cast<difference_type>(pre.size()) - 1, this); 
    }
    const_reverse_iterator crend() const 
    { 
        return const_reverse_iterator(-static_cast<difference_type>(pre.size()) - 1, this); 
    }

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

    iterator insert(const_iterator pos, const value_type& val) 
    {
        difference_type dis = pos.cur;
        if (dis < 0) {
            pre.insert(pre.cbegin() - dis, val); ///
        }
        else {
            suf.insert(suf.cbegin() + dis, val);
        }
        return pos;
    }
    iterator insert(const_iterator pos, value_type&& val) 
    {
        difference_type dis = pos.cur;
        if (dis < 0) {
            pre.insert(pre.cbegin() - dis, std::move(val)); ///
        }
        else {
            suf.insert(suf.cbegin() + dis, std::move(val));
        }
        return pos;
    }
    
    iterator insert(const_iterator pos, size_type count, const value_type& val) 
    {
        difference_type dis = pos.cur;
        if (dis < 0) {
            Vec temp(count, val);
            std::reverse(temp.begin(), temp.end());
            pre.insert(pre.cbegin() - dis, temp.begin(), temp.end()); 
        }
        else {
            suf.insert(suf.cbegin() + dis, count, val);
        }
        return pos;
    }
    template<class InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) 
    {
        difference_type dis = pos.cur;
        if (dis < 0) {
            Vec temp(first, last);
            std::reverse(temp.begin(), temp.end());
            pre.insert(pre.cbegin() - dis, temp.begin(), temp.end()); 
        }
        else {
            suf.insert(suf.cbegin() + dis, first, last);
        }
        return pos;
    }
    iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) 
    {
        difference_type dis = pos.cur;
        if (dis < 0) {
            std::reverse(ilist.begin(), ilist.end());
            pre.insert(pre.cbegin() - dis, ilist); 
        }
        else {
            suf.insert(suf.cbegin() + dis, ilist);
        }
        return pos;
    }

    template<class... Args>
    iterator emplace(const_iterator pos, Args&&... args) 
    {
        difference_type dis = pos.cur;
        if (dis < 0) {
            pre.emplace(pre.cbegin() - dis, args...); ///
        }
        else {
            suf.emplace(suf.cbegin() + dis, args...);
        }
        return pos;
    }

    iterator erase(const_iterator pos) 
    {
        difference_type dis = pos.cur;
        if (dis < 0) {
            pre.erase(pre.cbegin() - dis - 1); ///
        }
        else suf.erase(suf.cbegin() + dis);
        return pos;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        difference_type from = first.cur, to = last.cur;
        if (from < 0 && to >= 0) {
            pre.erase(pre.cbegin(), pre.cbegin() - from);
            suf.erase(suf.cbegin(), suf.cbegin() + to);
        }
        else if (from >= 0) {
            suf.erase(suf.cbegin() + from, suf.cbegin() + to);
        }
        else { ///
            pre.erase(pre.cbegin() - to, pre.cbegin() - from);
        }
        return last;
    }

#ifdef USE_EXTRA_ACC_DEQUE_OPT

    void insert(size_type pos, const value_type& val) 
    {
        difference_type dis = static_cast<difference_type>(pos) 
                            - static_cast<difference_type>(pre.size());
        if (dis < 0) {
            pre.insert(pre.cbegin() - dis, val); ///
        }
        else {
            suf.insert(suf.cbegin() + dis, val);
        }
    }

    template<class... Args>
    void emplace(size_type pos, Args&&... args) 
    {
        difference_type dis = static_cast<difference_type>(pos) 
                            - static_cast<difference_type>(pre.size());
        if (dis < 0) {
            pre.emplace(pre.cbegin() - dis, args...); ///
        }
        else {
            suf.emplace(suf.cbegin() + dis, args...);
        }
    }

    size_type erase(size_type pos) 
    {
        difference_type dis = static_cast<difference_type>(pos) 
                            - static_cast<difference_type>(pre.size());
        if (dis < 0) {
            pre.erase(pre.cbegin() - dis - 1); ///
        }
        else suf.erase(suf.cbegin() + dis);
        return pos;
    }

    size_type erase(size_type first, size_type last)
    {
        difference_type from = static_cast<difference_type>(first) 
                             - static_cast<difference_type>(pre.size());
        difference_type to = static_cast<difference_type>(to) 
                        - static_cast<difference_type>(pre.size());
        if (from < 0 && to >= 0) {
            pre.erase(pre.cbegin(), pre.cbegin() - from);
            suf.erase(suf.cbegin(), suf.cbegin() + to);
        }
        else if (from >= 0) {
            suf.erase(suf.cbegin() + from, suf.cbegin() + to);
        }
        else { ///
            pre.erase(pre.cbegin() - to, pre.cbegin() - from);
        }
        return last;
    }
#endif

    void push_back(const value_type& val) 
    {
        suf.push_back(val);
    }
    void push_back(value_type&& val) 
    {
        suf.push_back(std::move(val));
    }

#if _CPPVERSION >= 201700L
    template<class... Args>
    reference emplace_back(Args&&... args) 
    {
        return suf.emplace_back(args...);
    }
#else

    template<class... Args>
    void emplace_back(Args&&... args) 
    {
        suf.emplace_back(args...);
    }

#endif

    void pop_back() 
    {
        if (suf.empty()) rebuild();
        suf.pop_back();
    }

    void push_front(const value_type& val) 
    {
        pre.push_back(val);
    }
    void push_front(value_type&& val) 
    {
        pre.push_back(std::move(val));
    }

#if _CPPVERSION >= 201700L
    template<class... Args>
    reference emplace_front(Args&&... args) 
    {
        return pre.emplace_back(args...);
    }
#else

    template<class... Args>
    void emplace_front(Args&&... args) 
    {
        pre.emplace_back(args...);
    }

#endif

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
        else if (new_size < size()) {
            erase(cbegin() + new_size, cend());
        }
    }
    void resize(size_type new_size, const value_type& val)
    {
        if (new_size > size()) {
            suf.resize(suf.size() + (new_size - size()), val);
        }
        else if (new_size < size()) {
            erase(cbegin() + new_size, cend());
        }
    }

    void reserve(size_type new_size)
    {
        reserve_front(new_size);
        reserve_back(new_size);
    }

#ifndef USE_EXTRA_ACC_DEQUE_OPT
private:
#endif
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

    pointer get_unsafe(difference_type pos)
    {
        if (pos < 0) return (pre.begin() - pos - 1);
        return (suf.begin() + pos);
    }
    const_pointer get_const_unsafe(difference_type pos) const
    {
        if (pos < 0) return (pre.cbegin() - pos - 1);
        return (suf.cbegin() + pos);
    }

    void range_check(size_type pos) const ///
    {
        if (pos >= size())
        {
            // throw std::out_of_range;
        }
    }

    void rebuild()
    {
        if (pre.empty()) {
            size_type mid = suf.size() / 2;
            pre = Vec(suf.rbegin() + mid, suf.rend());
            suf = Vec(suf.end() - mid, suf.end());
        }
        else {
            size_type mid = pre.size() / 2;
            suf = Vec(pre.rbegin() + mid, pre.rend());
            pre = Vec(pre.end() - mid, pre.end());
        }
    }

};

#ifndef __TEMPL_DECLARE
#define __TEMPL_DECLARE template<typename T, typename Container>
#endif

#ifndef __TEMPL_DQ
#define __TEMPL_DQ Deque<T, Container>
#endif 

__TEMPL_DECLARE void swap(__TEMPL_DQ& lhs, __TEMPL_DQ& rhs) 
{
    lhs.swap(rhs);
}

__TEMPL_DECLARE bool operator==(const __TEMPL_DQ& lhs, const __TEMPL_DQ& rhs)
{
    if (lhs.size() != rhs.size()) return false;
    for (size_t i = 0, len = lhs.size(); i < len; i++) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}

__TEMPL_DECLARE bool operator!=(const __TEMPL_DQ& lhs, const __TEMPL_DQ& rhs)
{
    return !(lhs == rhs);
}

__TEMPL_DECLARE bool operator<(const __TEMPL_DQ& lhs, const __TEMPL_DQ& rhs)
{
    size_t len = min(lhs.size(), rhs.size());
    for (size_t i = 0; i < len; i++) {
        if (lhs[i] != rhs[i]) {
            return lhs[i] < rhs[i];
        }
    }
    return lhs.size() < rhs.size();
}

__TEMPL_DECLARE bool operator>(const __TEMPL_DQ& lhs, const __TEMPL_DQ& rhs)
{
    size_t len = min(lhs.size(), rhs.size());
    for (size_t i = 0; i < len; i++) {
        if (lhs[i] != rhs[i]) {
            return lhs[i] > rhs[i];
        }
    }
    return lhs.size() > rhs.size();
}

__TEMPL_DECLARE bool operator<=(const __TEMPL_DQ& lhs, const __TEMPL_DQ& rhs)
{
    size_t len = min(lhs.size(), rhs.size());
    for (size_t i = 0; i < len; i++) {
        if (lhs[i] != rhs[i]) {
            return lhs[i] < rhs[i];
        }
    }
    return lhs.size() <= rhs.size();
}

__TEMPL_DECLARE bool operator>=(const __TEMPL_DQ& lhs, const __TEMPL_DQ& rhs)
{
    size_t len = min(lhs.size(), rhs.size());
    for (size_t i = 0; i < len; i++) {
        if (lhs[i] != rhs[i]) {
            return lhs[i] > rhs[i];
        }
    }
    return lhs.size() >= rhs.size();
}


#ifdef USE_EXTRA_ACC_DEQUE_OPT

__TEMPL_DECLARE std::ostream& operator<<(std::ostream& out, const __TEMPL_DQ& x)
{
    size_t len = x.size();
    if (len == 0) return out;
    for (size_t i = 0; i + 1 < len; i++) {
        out << x[i] << ' ';
    }
    out << x.back();
    return out;
}

__TEMPL_DECLARE __TEMPL_DQ& operator+=(__TEMPL_DQ& x, const __TEMPL_DQ& y)
{
    size_t len_y = y.size();
    for (size_t i = 0; i < len_y; i++) {
        x.emplace_back(y[i]);
    }
    return x;
}

__TEMPL_DECLARE __TEMPL_DQ operator+(const __TEMPL_DQ& x, const __TEMPL_DQ& y)
{
    size_t len_y = y.size();
    __TEMPL_DQ z(x);
    for (size_t i = 0; i < len_y; i++) {
        z.emplace_back(y[i]);
    }
    return z;
}

#endif

#else

static_assert(false, "Require C++11 or later for acc::deque.");

#endif

}

#endif