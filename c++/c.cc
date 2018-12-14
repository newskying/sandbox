#include <map>

template<typename T>
struct TMap
{
    typedef std::map<T, T> Type;
};

template<>
struct TMap<int>
{
    typedef std::map<double, double> Type;
};

template<class T>
struct tmap_t : TMap<T>::Type {};

template<typename T>
T test(tmap_t<T> &tmap) { return 0.0; }

int main(int argc, char* argv[])
{
	//TMap<double>::Type tmap;
	tmap_t<double> tmap;
    tmap[1.1] = 5.2;
    double d = test(tmap); //Error: could not deduce template argument for T
    return 0;
}

