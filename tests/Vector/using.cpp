namespace amns
{
    void func() { }
}

namespace acc
{
    using amns::func;
}

using namespace amns;
using namespace acc;

signed main() 
{
    func();
    acc::func();
    return 0;
}