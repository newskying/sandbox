#include <iostream>
#include <type_traits>
#include <functional>
using namespace std;
template <typename T> void foo ( T x )
{
    auto r=ref(x);
    cout<<boolalpha;
    cout<<is_same<T&, decltype(r)>::value << endl;
    cout<<is_same<T&, decltype(r.get())>::value << endl;
}
int main()
{
    int x=5;
    foo (x);
    return 0;
}

