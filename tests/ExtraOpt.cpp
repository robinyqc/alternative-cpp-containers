#define USE_EXTRA_DQ_OPT
#include "../AmortizedDeque.hpp"

signed main()
{
    AmortizedDeque<double> a({1.0, 2.3, 4.5, 8.8, 5.7}), b({3.0, 4.0, 9.9});
    std::cout << a << '\n';
    a += b;
    std::cout << a << '\n';
    std::cout << a + b << '\n';
    // std::vector<int>::operator[]()
}