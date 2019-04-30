#include<iostream> 
using namespace std; 
  
class GfG 
{ 
    int i = 0; 
      
    public: 
    GfG(int _i = 0) 
    { 
        i = _i; 
        cout << "Inside GfG Constructor:" << i << "\n"; 
    } 
      
    ~GfG() 
    { 
        cout << "Inside GfG Destructor:" << i << "\n"; 
    } 
    void fun() {
       cout << i << std::endl;
    }
}; 

class Base {
   public:
   static GfG t;
   Base() {
      cout << "Inside Base Constructor\n";
   }
   ~Base() {
      cout << "Inside Base Destructor\n";
   }
};

GfG Base::t = 1;

int main() 
{ 
    cout << "Begin of main\n";
    int x = 0; 
    if (x==0) 
    { 
        Base obj; 
        obj.t.fun();
       static GfG so{2};
       so.fun();
    } 
    cout << "End of main\n"; 
}
