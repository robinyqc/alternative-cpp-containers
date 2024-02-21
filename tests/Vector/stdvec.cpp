#include <iostream>
#include <vector>

class Base
{
    protected:
        virtual int get() const = 0;
    public:
        void print() { std::cout << get() << '\n'; }
};

class Derive : public Base
{
protected:
    int get() const
    {
        return 1;
    }
};

signed main()
{
    Derive a;
    a.print();
    std::vector<int> a;
}