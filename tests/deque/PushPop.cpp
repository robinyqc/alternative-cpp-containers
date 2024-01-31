#include "deque"

signed main()
{
    using std::cout;
    acc::deque<int> dq;
    dq.emplace_back(1);
    dq.emplace_back(2);
    dq.push_back(3);
    dq.push_front(4);
    dq.emplace_front(5);
    for (auto it = dq.begin(); it != dq.end(); ++it) cout << *it << ' ';
    cout << '\n';
    for (const auto& it: dq) cout << it << ' ';
    cout << '\n';
    dq.pop_front();
    dq.pop_front();
    dq.pop_front();
    for (auto it: dq) cout << it << ' ';
    cout << '\n';
    dq.push_back(6);
    dq.pop_front();
    for (auto it: dq) cout << it << ' ';
    cout << '\n';
}