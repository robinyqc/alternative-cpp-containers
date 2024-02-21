#include <iostream>
#define USE_EXTRA_ACC_DEQUE_OPT
#include "DequeLink.hpp"
#include <memory>

signed main()
{
    using std::cout;
    
    using AMDQ = acc::Deque<double>; 

    AMDQ empty;

    AMDQ count(10);
    cout << count << '\n';

    AMDQ count_value(10, 1.45);
    cout << count_value << '\n';

    AMDQ from_iter(count_value.begin(), count_value.end());
    cout << from_iter << '\n';

    AMDQ from_other(count_value);
    cout << from_other << '\n';

    AMDQ move_from_other(std::move(count_value));
    cout << move_from_other << '\n';

    cout << count_value << '\n'; // undefined, expected nothing output.

    AMDQ from_ilist({114, 514, 1919, 810});
    cout << from_ilist << '\n';
    return 0;
}