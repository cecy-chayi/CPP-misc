#include <iostream>
#include <type_traits>


template<class T>
void A(const T &t )
{
    std::cout<<"\tconst T&"<<std::endl;
}

template<class T>
void A(T &t )
{
    std::cout<<"\tT&"<<std::endl;
}

template<class T>
void A(T &&t )
{
    std::cout<<"\tT&&"<<std::endl;
}

// template<class T>
// T&& my_forward( typename std::remove_reference<T>::type& t ) noexcept {
//     return static_cast<T&&>(t);
// }
 

// template<class T>
// T&& my_forward( typename std::remove_reference<T>::type&& t ) noexcept {
//     static_assert(!std::is_lvalue_reference<T>::value, "std::forward must not be used to convert an rvalue to an lvalue");
//     return static_cast<T&&>(t);
// }

template<class T>
T&& my_forward(T& t) noexcept {
    std::cout << "lvalue: " << std::is_same<T, int>::value << std::endl;
    return static_cast<T&&>(t);
}

template<class T>
T&& my_forward(T&& t) noexcept {
    std::cout << "rvalue: " << std::is_same<T, int&&>::value << std::endl;
    static_assert(!std::is_lvalue_reference<T>::value, "std::forward must not be used to convert an rvalue to an lvalue");
    return static_cast<T&&>(t);
}

template <class T>
void g1(T&& a)
{
    A(my_forward(a));
}

template <class T>
void g2(T&& a)
{
    A(std::forward<T>(a));
}

struct Arg {
    int i = 1;
    int get() && { return i; }
    int& get() & { return i; }
};

int main()
{
    int a = 1;
    const int &b = a;
    int &c = a;
    int &&d = 1;
    
    std::cout << "type: " << std::endl;
    std::cout << "function: " << std::endl;
    g1(a);
    g2(a);
    std::cout << "not function: " << std::endl;
    A(my_forward(a));
    A(std::forward<decltype(a)>(a));
    std::cout << std::endl;

    std::cout << "const type&: " << std::endl;
    std::cout << "function: " << std::endl;
    g1(b);
    g2(b);
    std::cout << "not function: " << std::endl;
    A(my_forward(b));
    A(std::forward<decltype(b)>(b));
    std::cout << std::endl;

    std::cout << "type&: " << std::endl;
    std::cout << "function: " << std::endl;
    g1(c);
    g2(c);
    std::cout << "not function: " << std::endl;
    A(my_forward(c));
    A(std::forward<decltype(c)>(c));
    std::cout << std::endl;

    std::cout << "type&&: " << std::endl;
    std::cout << "function: " << std::endl;
    g1(d);
    g2(d);
    std::cout << "not function: " << std::endl;
    A(my_forward(d));
    A(std::forward<decltype(d)>(d));
    std::cout << std::endl;
    return 0;
}