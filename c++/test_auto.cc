#include<iostream>

class cs {
   public:
      cs(int i=0) : i_(i) {
      }

      ~cs() {}

      cs& operator=(const cs& other) {
         i_ = other.i_;
         return *this;
      }

      void change(int i) {
         i_ = i;
      }

      int get_i() const {
         return i_;
      }

   private:
      int i_;
};


cs get_cs() {
   static int i = 0;
   return cs(i++);
}

int main(int argc, char* argv[]) {
   const cs& ref = get_cs();
   get_cs().change(232);
   cs a;

   cs&& a1         = static_cast<cs&&>(a);
   auto a2         = static_cast<cs&&>(a);
   //auto& a3        = static_cast<cs&&>(a); //can't compile
   auto&& a4       = static_cast<cs&&>(a);
   const auto a5   = static_cast<cs&&>(a);
   const auto& a6  = static_cast<cs&&>(a);
   const auto&& a7 = static_cast<cs&&>(a);

   cs&& b1         = static_cast<cs&&>(a1);
   const cs&& b2   = static_cast<cs&&>(a1);
   auto b3         = static_cast<cs&&>(a1);
   //auto& b4        = static_cast<cs&&>(a1); //can't compile
   auto&& b5       = static_cast<cs&&>(a1);
   const auto b6   = static_cast<cs&&>(a1);
   const auto& b7  = static_cast<cs&&>(a1);
   const auto&& b8 = static_cast<cs&&>(a1);

   const cs&& c1   = static_cast<const cs&&>(a5);
   auto c2         = static_cast<const cs&&>(a5);
   auto& c3        = static_cast<const cs&&>(a5);
   auto&& c4       = static_cast<const cs&&>(a5);
   const auto& c5  = static_cast<const cs&&>(a5);
   const auto&& c6 = static_cast<const cs&&>(a5);

   return 0;
}

