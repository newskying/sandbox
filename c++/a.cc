//-std=c++1y
#include <cstdio>

#include <functional>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include "a.h"

int global_num = 0;


using namespace std;
double eval(std::function<double(double)> f, double x = 2.0){return f(x);}

void Base::f() {
    cout << "Base::f" << endl;
    cout << Result::failed << endl;
}

void test_string() {
    cout << "aa " " bb" << endl;
    std::string ss = "";
    cout << "ss:" << ss.empty() << endl;
    printf("ss p:%p\n", ss.c_str());
}

void test_enum() {
    Base b;
    b.f();
    cout << "failed: " << Base::Result::failed << endl;
}

template<typename T>
ostream& operator<<(ostream& out, std::set<T> const& s) {
    //for(auto it = s.begin(); it != s.end(); ++it) {  //also work!
    //for(typename std::set<T>::const_iterator it = s.begin(); it != s.end(); ++it) {
        //out << *it << endl;
    //}
    auto print = [](T const& v){cout << v << endl;};
    for_each(s.begin(), s.end(), print);
    //copy(s.begin(), s.end(), ostream_iterator<T>(), "");
    return out;
}
void test_ostream_vector() {
    std::set<int> si;
    //vi.push_back(1);
    //vi.push_back(2);
    //vi.push_back(3);
    si.insert(1);
    si.insert(2);

    cout << si << endl;
}

void test_c11_function() {
    std::function<double(double)> f0    = [](double x){return 1;};
    auto                          f1    = [](double x){return x;};
    decltype(f0)                  fa[3] = {f0,f1,[](double x){return x*x;}};
    std::vector<decltype(f0)>     fv    = {f0,f1};
    fv.push_back                  ([](double x){return x*x;});
    for(int i=0;i<fv.size();i++)  std::cout << fv[i](2.0) << "\n";
    for(int i=0;i<3;i++)          std::cout << fa[i](2.0) << "\n";
    for(auto &f : fv)             std::cout << f(2.0) << "\n";
    for(auto &f : fa)             std::cout << f(2.0) << "\n";
    std::cout << eval(f0) << "\n";
    std::cout << eval(f1) << "\n";
    auto f4 = [](){++global_num;};
    std::cout << global_num << std::endl;
}

void test_erase_container() {
    std::vector<int> vi{1, 2, 3, 4, 5, 6};
    for(vector<int>::const_iterator it = vi.begin(); it != vi.end(); ++it) {
        cout << *it << endl;
    }
    for(vector<int>::iterator it = vi.begin(); it != vi.end(); ) {
        vi.erase(it++);
    }
    for(vector<int>::const_iterator it = vi.begin(); it != vi.end(); ++it) {
        cout << *it << endl;
    }
}

class A {
};
void test_size() {
    A a;
    cout << "sizeof A:" << sizeof(a) << endl;
}

void test_for_loop() {
    std::vector<int> vi{1, 2, 3, 4, 5};
    for(int i : vi) {
        cout << i << endl;
    }
}

int main()
{
    test_for_loop();
    //test_size();
    //test_erase_container();
    //test_ostream_vector();
    //test_string();
    //test_enum();
    //test_c11_function();
    return 0;
}

//int main(int, char*[])
//{
   //printf("%s\n", "new string");
//}

