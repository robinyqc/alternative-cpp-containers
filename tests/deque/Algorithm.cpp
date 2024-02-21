#define USE_EXTRA_ACC_DEQUE_OPT
#include "DequeLink.hpp"
#include <algorithm>

signed main()
{
    acc::Deque<int> a({1, 3, 2, 4, 6, 2, 3});
    std::sort(a.begin(), a.end());
    std::cout << a << '\n';
    std::next_permutation(a.begin(), a.end());
    std::cout << a << '\n';
    std::reverse(a.begin(), a.end());
    std::cout << a << '\n';
}