#include <iostream>
#include <random>
std::ostream& operator<<(std::ostream& out, std::mt19937 x)
{
    return out << x() << ' ';
}

#define USE_EXTRA_DQ_OPT

#include "../AmortizedDeque.hpp"
#include <deque>


signed main()
{
    using mt = std::mt19937;
    using std::cout;
    using AMDQ = AmortizedDeque<mt>;

    AMDQ empty;

    AMDQ count(10);
    cout << count << '\n';

    AMDQ count_value(10, mt(13u));
    cout << count_value << '\n';

    AMDQ from_iter(count_value.begin(), count_value.end());
    cout << from_iter << '\n';

    AMDQ from_other(count_value);
    cout << from_other << '\n';

    AMDQ move_from_other(std::move(count_value));
    cout << move_from_other << '\n';

    cout << count_value << '\n'; // undefined, expected nothing output.

    AMDQ from_ilist({mt(3), mt(9), mt(10), mt()});
    cout << from_ilist << '\n';
    return 0;
}