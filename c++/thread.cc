#include <iostream>
#include <thread>
#include <array>
#include <vector>
#include <functional>

void fun() {
   std::cout << "A new thread!" << std::endl;
}

void test_thread() {
   std::thread t(fun);
   int n1 = 500;
   int n2 = 600;
   std::thread t1([&](int addNum) {
         n1 += addNum;
         n2 += addNum;
         }, 500);
   std::cout << "main thread!" << std::endl;
   std::cout << n1 << ":" << n2 << std::endl;
   std::cout << "t:" << t.get_id() << std::endl;
   t.join();
   t1.join();
   std::cout << "t1:" << t1.get_id() << std::endl;
}

void test_lambda_capture_value() {
   int v1 = 42;
   auto f=[v1]() {return v1;};
   v1 = 0;
   auto j = f();  //j is 42 not 0;

   std::cout << "v1:" << v1 << " j:" << j << std::endl;

   int a1[5] = {};
   auto f2 = [=]() {
      for (int i : a1) {
         std::cout << i << " ";
      }
   };
   for (int i : a1) {
      a1[i] = i;
   }
   f2();  // output: 0 0 0 0 0  not 1 2 3 4 5
   std::cout << std::endl;

   std::array<int, 5> a2 = {};
   auto f3 = [=]() {
      for (int i : a2) {
         std::cout << i << " ";
      }
   };
   for (int i : a2) {
      a2[i] = i;
   }
   f3();  // output: 0 0 0 0 0  not 1 2 3 4 5
}

class A {
   public:
      A(int m):m_(m) {}
      void fun() {
         //int m_(5);
         auto f = [this] {  //if not capture "this", the inner lambda can't capture this!!!
            std::cout << "outer(): " <<  m_ << std::endl;
            auto f1 = [=] {
               std::cout << "inner lambda(): " << m_ << std::endl;
            };
            return f1();
         };
         f();
      }
   private:
      int m_;
};
void test_lambda_capture_this() {
   A a(3);
   a.fun();
}

typedef std::function<bool(int)> Filter;
std::vector<Filter> filters;
void test_function_bind() {
   Filter f = [](int a) -> bool {
      if (a %2) {
         std::cout << "test a:" << a << ":" << (a %2) << std::endl;
         return true;
      } else {
         std::cout << "test a:" << a << ":" << (a %2) << std::endl;
         return false;
      }
   };
   f(1);
   auto f1 = std::bind(f, 2);
   f1();
   std::cout << typeid(f).name() << std::endl
      << typeid(f1).name() << std::endl;

    auto func4 = [](int i) -> int {
      if(i>10) return 1;
      else return 2.3;
    };
    std::cout << "func4 ret:" << typeid(func4(5)).name() << "=" << func4(5) << std::endl;
    std::cout << "func4 ret:" << typeid(func4(15)).name() << "=" << func4(15) << std::endl;
}

int main(int argc, char* argv[]) {
   //test_thread();
   //test_lambda_capture_value();
   //test_lambda_capture_this();
   test_function_bind();
   return 0;
}

