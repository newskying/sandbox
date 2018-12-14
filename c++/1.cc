#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <sstream>

using namespace std;

void test_bool() {
    bool tt{};
    cout << "bool:" << tt << endl;
}


void test_default_cast() {
   std::map<std::string, int> si = {
      {"aaa", 1},
      {"bbb", 2},
      {"ccc", 3}
   };
   const char * s = "ccc";
   auto it = si.find(s);
   cout << it->second << endl;
}

void test_function() {
   std::cout << "-------test_function<<<<<" << endl;
   std::function<void(int& o)> ff = [](int& o) {
      cout << "in lambda func: " << o << endl;
      o |= 1;
   };

   int a(0);
   ff(a);
   cout << "after func: " << a << endl;

   map<int, function<int(int&)> > setterList {
      {1, [](int& o)->int { cout << "in func: " << o << endl; o |= 1; return o;}},
      {2, [](int& o)->int { cout << "in func: " << o << endl; o |= 2; return o;}}
   };

   for (auto i : setterList) {
      cout << i.first << ":" << i.second(a) << endl;
   }
   setterList[3](a);
   cout << a << endl;
   std::cout << "-------test_function end>>>>>" << endl;
}

void test_enum() {
   enum class TestEnum : int {
      T1 = 1,
      T2 = 3,
      T3 = 5,
      Unknown
   };

   cout << int(TestEnum::T2) << endl;
   TestEnum a = (TestEnum)2;
   cout << int(a) << endl;
   cout << "true:" << true << " false:" << false << std::endl;
   int e1 = 3;
   //cout << "e1 == TestEnum::T2 : " << (e1 == TestEnum::T2) << std::endl;
   TestEnum e2 = static_cast<TestEnum>(3);
   cout << "e2 == TestEnum::T2 : " << (e2 == TestEnum::T2) << std::endl;
}

void test_printf() {
   bool t = true;
   bool f = false;
   printf("true : c:%c i:%i d:%d\n", t, t);
   printf("false: c:%c i:%i d:%d\n", f, f);

   uint32_t u32 = 0xffffffff;
   printf("false: u:%u, x:%x i:%i d:%d\n", u32, u32, u32, u32);
}

void test_output_map() {
   std::map<int, int> c = {
      {1, 1},
      {2, 2},
      {3, 3}
   };
   //cout << c << endl;
}

void test_trasfer_string() {
   uint32_t ui(0);
   std::string s = std::to_string(ui);
   cout << ui << ":--" << s << endl;
   int i(0);
   s = std::to_string(i);
   cout << i << ":--" << s << endl;
}

void test_type_size() {
   cout << "unsinged: " << sizeof(unsigned) << endl;
   cout << "unsigned int: " << sizeof(unsigned int) << endl;
   cout << "unsigned long: " << sizeof(unsigned long) << endl;
   cout << "unsigned long long: " << sizeof(unsigned long long) << endl;
}

void test_sstream_transform() {
   std::ostringstream os;
   unsigned u(0);
   os << u;
   std::cout << "u:" << u << std::endl
      <<"hex:" << std::hex << u << std::endl
      << "string: " << os.str() << std::endl;
   os.str("");
   os << std::hex << u;
   std::cout << "hex string: " << os.str() << std::endl;
}

int main(int argc, char* argv[]) {
    //test_bool();
    //test_default_cast();
    //test_function();
    test_enum();
    //test_printf();
    //test_output_map();
    //test_trasfer_string();
    //test_type_size();
    //test_sstream_transform();
}

