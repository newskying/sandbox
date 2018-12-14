#include <memory>
#include <iostream>

template<typename T>
struct classA {
  struct classB
  {
    //template<typename U>
		friend inline std::ostream& operator<< (std::ostream &out, const typename classA<U>::classB &b) {
			return out;
		}
  };

  classB root;

  //template<typename U>
	  friend std::ostream& operator<< (std::ostream &out, const classA<int> &tree) {
		  out << tree.root;
		  return out;
	  }
};

//template<typename T>
//inline std::ostream& operator<< (std::ostream &out,
                                 //const classA<T> &tree)
//{
  //out << tree.root;
  //return out;
//}

//template<typename T>
//inline std::ostream& operator<< (std::ostream &out,
                                 //const typename classA<T>::classB &b)
//{
  //return out;
//}

int main()
{
  classA<int> a;
  std::cout << a;
}

