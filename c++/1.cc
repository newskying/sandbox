#include <iostream>
#include <iterator>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <sstream>
#include <typeinfo>

using namespace std;

void test_bool() {
   cout << "------test_bool------" << endl;
    bool b1{}; //default bool{} is false
    bool b2{false};
    bool b3{true};
    bool b4(); //b4 is not a bool, but a function!!!
    bool b5(false);
    bool b6(true);
    cout << b1 << ":bool:b1{}" << endl;
    cout << b2 << ":bool:b2{false}" << endl;
    cout << b3 << ":bool:b3{true}" << endl;
    cout << b4 << ":bool:b4()" << endl;
    cout << b5 << ":bool:b5(false)" << endl;
    cout << b6 << ":bool:b6(true)" << endl;
    cout << "bool():" << bool() << ":bool{}:" << bool{} << endl;
    cout << typeid(b1).name() << endl
       << typeid(b2).name() << endl
       << typeid(b3).name() << endl
       << typeid(b4).name() << endl
       << typeid(b5).name() << endl
       << typeid(b6).name() << endl;
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

class TestDefaultMember {
   public:
      int i;
      string s;
      TestDefaultMember() {}
};

void test_default_member() {
   cout << "------test_default_member------" << endl;
   TestDefaultMember t;
   cout << t.i << endl  //NOT an fixed number
      << t.s << endl
      << t.s.empty() << endl;
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
   enum PureEnum {
      PE_A = 0,
      PE_B,
   };
   cout << "PE_A: " << PE_A << endl;
   cout << "PE_B: " << PE_B << endl;
   cout << "PE_A==PE_B:" << (PureEnum::PE_A == PureEnum::PE_B) << endl;
   cout << "PE_A==PE_A:" << (PureEnum::PE_A == PureEnum::PE_A) << endl;

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
   printf("true : c:%c i:%i d:%d\n", t, t, t);
   printf("false: c:%c i:%i d:%d\n", f, f, f);

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

void test_transfer_string() {
   uint32_t ui(0);
   std::string s = std::to_string(ui);
   cout << "uint32:" << ui << "--string:" << s << endl;
   int i(0);
   s = std::to_string(i);
   cout << "int:" << i << "--string:" << s << endl;
}

void test_type_size() {
   cout << "unsinged: " << sizeof(unsigned) << endl;
   cout << "unsigned int: " << sizeof(unsigned int) << endl;
   cout << "unsigned long: " << sizeof(unsigned long) << endl;
   cout << "unsigned long long: " << sizeof(unsigned long long) << endl;
}

void test_sstream_transform() {
   std::ostringstream os;
   unsigned u(15);
   std::cout << "unsigned u:" << u << std::endl
      <<"unsigned u sci:" << std::scientific << u << std::endl
      <<"unsigned u hex:" << std::hex << u << std::endl
      <<"unsigned u again:" << u << std::endl
      <<"unsigned u oct:" << std::oct << u << std::endl
      <<"unsigned u again:" << u << std::endl;

   os << u;
   std::cout << "string: " << os.str() << std::endl;
   os.str("");
   os << std::hex << u;
   std::cout << "string hex: " << os.str() << std::endl;
}

template<class K, class T>
void out_map(std::map<K, T> const& m) {
   for (auto i : m) {
      std::cout << i.first << ":" << i.second << std::endl;
   }
}

void test_map() {
   map<int, int> m = {
      {1, 1},
      {2, 2}
   };
   out_map(m);
   int i = m.erase(1);  //has key in map
   std::cout << "i:" << i << std::endl;
   i = m.erase(3);      //no key in map
   std::cout << "i:" << i << std::endl;
   std::cout << "-----" << std::endl;
   out_map(m);
}

#include <unistd.h>
void test_circle(int time) {
   //sleep(time);
   //usleep(time*1000000);//microseconds
}

void test_auto() {
   auto n = 2 * 1024 * 1024 * 1024; //error: beyond limit of integer
   std::cout << n << std::endl;
}

void test_number_size() {
   uint64_t s = 0xffffffffffffffff;
   cout << s << std::endl;
   printf("%u, %lu, %llu, %ju\n", s, s, s, s);
   printf("%x", &s);
}

enum class EEE : unsigned {
   EE_AA = 0,
   EE_BB,
   EE_CC, //has "," here
};

void test_enum_class() {
   EEE e1{EEE::EE_AA};
   EEE e2{EEE::EE_BB};
   EEE e3{EEE::EE_CC};
   cout << (unsigned) e1 << std::endl;
   cout << (unsigned) e2 << std::endl;
   cout << (unsigned) e3 << std::endl;
}

void test_string_compare() {
   std::string s1("hello");
   std::string s2("he");
   cout << (s1 == s2) << ":s1 == s2" << std::endl;
   cout << s1.compare(s2) << ":s1.compare(s2)" << std::endl;
}

void test_substr() {
   std::string ss = "abc;123";
   cout << ss.substr(0, ss.find_first_of(";")) << endl;;
   cout << ss.substr(ss.find_first_of(";")+1) << endl;;
}

void test_bind() {
   //error: cannot bind ‘std::ostream {aka std::basic_ostream<char>}’ lvalue to ‘std::basic_ostream<char>&&’
   //cout << vector<int>{1, 2, 3} << endl;  //produce compile error!
   //2nd way to output vector
   auto v = vector<int>{1, 2, 3};
   //std::copy(std::begin(v), std::end(v), std::ostream_iterator<int>(std::cout, " "));
   std::ostringstream os;
   //std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
   std::copy(v.begin(), v.end(), std::ostream_iterator<int>(os, " "));
   std::cout << os.str() << std::endl;
}

struct People {
   string name;
   int id;
};

//std::move will not cause CORE DUMP by access staled temp obj 
void fun_stack_obj(People & p) {
   string s = "Nancy";
   int id = 88;
   cout << "fun_stack_obj: " << s << id << endl;
   p.name = std::move(s);    //for string: swap(p.name, s)
   p.id   = std::move(id);   //for int   : p.id = id;
   cout << "fun_stack_obj: after move: " << s << id << endl;
   return ;
}
void fun_obstruct() {
   string s1 = "Mark";
   string s2 = "Dany";
   int id1 = 77;
   int id2 = 66;
   cout << s1 << ":" << s2 << endl;
}
void test_stack_temp_obj() {
   cout << "------test_stack_temp_obj------" << endl;
   People p1 {"default", 1};
   People p2 {"default", 2};
   cout << "main:" << p1.name << p1.id << endl;
   fun_stack_obj(p1);
   cout << "main: after move: " << p1.name << p1.id << endl;
   fun_obstruct();
   cout << "main: after obstruct: " << p1.name << p1.id << endl;
}

void fun_overload(People p) {
   cout << "fun1(People)" << endl;
}

//void fun_overload(People const& p) { // ambiguous function of fun1(People)
   //cout << "fun1(People const&)" << endl;
//}

void test_function_overload() {
   cout << "------test_function_overload------" << endl;
   People p{"hali", 8};
   fun_overload(p);
}

class Obj {
   public:
      Obj(int i) {
         cout << "in Obj(int)..." << endl;
      }
      Obj() {
         cout << "in Obj()..." << endl;
      }
      Obj(Obj const& o) {
         cout << "in Obj(const&)..." << endl;
      }
      Obj& operator=(Obj const& o) {
         cout << "in operator=(const&)..." << endl;
      }
      Obj(Obj &&o) {
         cout << "in Obj(&&)..." << endl;
      }
      Obj& operator=(Obj && o) {
         cout << "in operator=(&&)..." << endl;
      }
      ~Obj() {
         cout << "in ~Obj()..." << endl;
      }
      void set(int i) {
         cout << "in set(" << i << ")" << endl;
      }
};

void test_sub_fun(Obj const&o) {
   cout << "------test_sub_fun------" << endl;
   cout << "   " << &o << endl;
   cout << "------test_sub_fun------end" << endl;
   //return (o);
}

void test_copy_move() {
   cout << "------test_copy_move------" << endl;
   Obj o{};
   o.set(5);
   test_sub_fun(move(o)); //only use Obj(int) 1 time!
   //cout << "   " << &o1 << endl;
   //vector<Obj> v;
   //v.push_back(o); //use Obj(const&) to copy into vector
   //v.push_back(std::move(o)); //use Obj(&&)
   //v.push_back(std::move(Obj())); //use Obj(&&)
   //v.push_back(Obj()); //use Obj(&&)
}

int main(int argc, char* argv[]) {
   test_copy_move();
   //test_function_overload();
   //test_bool();
   //test_default_cast();
   //test_default_member();
   //test_function();
   //test_enum();
   //test_printf();
   //test_output_map();
   //test_transfer_string();
   //test_type_size();
   //test_sstream_transform();
   //test_map();
   //test_auto();
   //test_number_size();
   //test_enum_class();
   //test_string_compare();
   //test_substr();
   //test_bind();
   //test_stack_temp_obj();
}

