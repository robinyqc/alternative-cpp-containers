#define USE_EXTRA_DQ_OPT
#include "../AmortizedDeque.hpp"

signed main()
{
    using std::cout;
    AmortizedDeque<int> a({0, 1, 2, 3, 4, 5});
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