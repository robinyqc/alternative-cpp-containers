#define USE_EXTRA_ACC_DEQUE_OPT
#include "deque_link.hpp"

signed main()
{
    acc::Deque<double> a({1.0, 2.3, 4.5, 8.8, 5.7}), b({3.0, 4.0, 9.9});
    std::cout << a << '\n';
    a += b;
    std::cout << a << '\n';
    std::cout << a + b << '\n';
}