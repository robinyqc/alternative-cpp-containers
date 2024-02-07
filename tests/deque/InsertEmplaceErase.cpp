#define USE_EXTRA_ACC_DEQUE_OPT
#include "deque_link.hpp"

signed main()
{
    using std::cout;
    acc::Deque<int> a({0, 1, 2, 3, 4, 5});
    a.insert(a.cbegin() + 3, 6);
    a.insert(a.begin() + 1, 7);
    a.emplace(a.begin() + 6, 8);
    cout << a << '\n';
    a.pop_front();
    cout << a << '\n';
    a.erase(a.begin());
    cout << a << '\n';
    a.erase(a.begin() + 3, a.begin() + 5);
    cout << a << '\n';
}