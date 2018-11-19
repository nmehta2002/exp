#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

class B
{
public:
    int a;

    ~B() { std::cout<<"~B() \n";}
};

class A
{
public:
    template <typename BPtrType>
    A(BPtrType b)
    {
        auto bRawPtr = getRawPointer(b);
        ownPointer(b);
    }

    template <typename BPtrType>
    B*
    getRawPointer(BPtrType& b)
    {
        static_assert(std::is_pointer<BPtrType>::value, "Function only supported for raw pointers or unique_ptr");
        return b;
    }

    template <typename BPtrType>
    void ownPointer(BPtrType &type)
    {
        static_assert(std::is_pointer<BPtrType>::value, "Function only supported for raw pointers or unique_ptr");
    }

    std::vector<std::unique_ptr<B>> ownedB;
};


template <>
B*
A::getRawPointer(std::unique_ptr<B>& bptr)
{
    std::cout<<"Deduced to unique_ptr \n";
    return bptr.get();
}

template <>
void A::ownPointer(std::unique_ptr<B> &bptr)
{
    ownedB.emplace_back(bptr.release());
}

int main()
{
    B b;

    B *bRawPtr = &b;
    std::unique_ptr<B> bUptr(new B);

    A a1(bRawPtr);
    A a2(std::move(bUptr));
}