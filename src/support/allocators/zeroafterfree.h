// Copyright (c) 2007-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_SUPPORT_ALLOCATORS_ZEROAFTERFREE_H
#define BITCOIN_SUPPORT_ALLOCATORS_ZEROAFTERFREE_H

#include "support/cleanse.h"

#include <memory>
#include <vector>

template <typename T>
struct zero_after_free_allocator : public std::allocator<T> {
    // MSVC8 default copy constructor is broken
    typedef std::allocator<T> base;
    typedef typename base::size_type size_type;
    typedef typename base::difference_type difference_type;
#if __cplusplus < 202002L
    // These typedefs were removed in C++20
    typedef typename base::pointer pointer;
    typedef typename base::const_pointer const_pointer;
    typedef typename base::reference reference;
    typedef typename base::const_reference const_reference;
#else
    // For C++20 and later, define these ourselves
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
#endif
    typedef typename base::value_type value_type;
    zero_after_free_allocator() throw() {}
    zero_after_free_allocator(const zero_after_free_allocator& a) throw() : base(a) {}
    template <typename U>
    zero_after_free_allocator(const zero_after_free_allocator<U>& a) throw() : base(a)
    {
    }
    ~zero_after_free_allocator() throw() {}
    template <typename _Other>
    struct rebind {
        typedef zero_after_free_allocator<_Other> other;
    };

    void deallocate(T* p, std::size_t n)
    {
        if (p != nullptr)
            memory_cleanse(p, sizeof(T) * n);
        std::allocator<T>::deallocate(p, n);
    }
};

// Byte-vector that clears its contents before deletion.
using CSerializeData = std::vector<char, zero_after_free_allocator<char> >;

#endif // BITCOIN_SUPPORT_ALLOCATORS_ZEROAFTERFREE_H
