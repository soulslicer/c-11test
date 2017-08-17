#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <tuple>
//#include <

//range-based for loops
//lambda expressions
//auto
//constexpr
//explicit conversion functions
//std::thread, std::atomic
//move constructors std::move and the && operator
//nullptr
//static assert
//variadic templates
//initializing non-const member variables in the header file
//traits //https://github.com/soulslicer/OctreeOCL/blob/master/src/Octree.hpp

// https://www.toptal.com/c-plus-plus/interview-questions
// https://tests4geeks.com/cpp-interview-questions/
// http://thispointer.com/c11-tutorial/
// http://www.thegeekstuff.com/2016/02/c-plus-plus-11/comment-page-1/

using namespace std;

struct Object{
    int x = 0;
    Object(){}
};

/*****************************************************/

void iterators(){
    std::vector<int> vectorInt = {1, 2};
    for(auto& i : vectorInt) cout << i << endl;

    std::vector<Object> vectorObj = {Object()};
    vectorObj.push_back(Object());
    for(auto& obj : vectorObj){
        Object& objCast = obj;
        cout << objCast.x << endl;
    }

    int counter = 0;
    for (auto it=vectorObj.begin(); it!=vectorObj.end(); ++it, ++counter){
        Object& object = *it;
        cout << object.x << endl;
    }

    std::map<int, Object> mapObj = {{1, Object()}};
    for (auto& kv : mapObj) {
        cout << kv.first << kv.second.x << endl;
    }
}

/*****************************************************/

void execute(const std::vector<std::function<void ()>>& fs){
    for (auto& f : fs)
        f();
}

void simpleFunction(){
    cout << "I'm an old plain function" << endl;
}

int intFunction(int x, int y){
    return x+y;
}

class FunctorClass{
public:
    void operator()() const{
        cout << "I'm a functor" << endl;
    }
};

void algorithmFunction(std::function<int(int)> costFunction){
    for(int i : {0,1,2}){
        int out = costFunction(i);
        cout << out << endl;
    }
}

class TestCallbackClass{
public:
    int x = 1;
    std::function<int(int,int)> callback_;
    TestCallbackClass(){}
    void setCallback(std::function<int(int,int)>& callback){
        callback_ = callback;
    }
    int costFunction(int var){
        return var + x;
    }
    void runAlgorithm(){
        algorithmFunction(std::bind(&TestCallbackClass::costFunction, this, std::placeholders::_1));
    }
};

float mult(float x, float y){
    return x*y;
}

void functor(){
    std::vector<std::function<void ()>> x;
    x.push_back(simpleFunction);
    x.push_back(FunctorClass());
    x.push_back([] ()
    {
        cout << "HI, I'm a lambda expression" << endl;
    });
    execute(x);

    std::function<int(int,int)> intFunc = intFunction;
    cout << intFunc(5,5) << endl;

    TestCallbackClass testCallbackClass;
    testCallbackClass.setCallback(intFunc);

    testCallbackClass.runAlgorithm();

    auto f = mult;
    cout << f(2,3) << endl;
}

/*****************************************************/

class TestLambdaClass{
public:
    float x = 5;
    TestLambdaClass() {}
    void specialLamda(){
        auto f = [this] (float m)
        {
            return m*x;
        };
        cout << f(5) << endl;
    }
};

template<typename T>
class TestLambdaTemplateClass{
public:
    TestLambdaTemplateClass(){}
    void specialLambda(T t){
        auto f = [this] (T t)
        {
            return t*t;
        };
        cout << f(t) << endl;
    }
};

template<class InputIterator>
void for_each2(InputIterator begin, InputIterator end, std::function<void(int)> f) {
    for(begin; begin !=end; ++begin){
        f(*begin);
    }
}

template <class... Params>
void fextend(Params... params) {
    std::array<int, sizeof...(params)> list = {params...};
    cout << list.size() << endl;
}

void lamda(){
    TestLambdaClass testLambda;
    testLambda.specialLamda();

    TestLambdaTemplateClass<int> testLambdaTemplate;
    testLambdaTemplate.specialLambda(4);

    std::vector<int> intVec = {1,2,3,4};
    intVec.erase(std::remove_if(intVec.begin(),intVec.end(),[](int i) -> bool {
        if(i==2) return true;
        else return false;
    }),intVec.end());
    for(auto i : intVec) cout << i << endl;

    std::string s = std::accumulate(std::next(intVec.begin()), intVec.end(),std::to_string(intVec[0]),[](std::string a, int b) {
        return a + '-' + std::to_string(b);
    });
    cout << s << endl;

    std::function<int(int)> fact = [&fact] (int n) {
        return (n==0) ? 1 : n * fact(n-1);
    };
    cout << fact(7) << endl;

    std::for_each(intVec.begin(), intVec.end(), [] (int x) {std::cout << x << ' ';});
    std::string myString = "abcd";
    for_each2(myString.begin(), myString.end(), [] (char x) {std::cout << x << ' ';});

    fextend(5);

    auto tup = std::make_tuple(1 , 2.0, "hello");
    std::vector<std::tuple<int,float,std::string>> m; m.push_back(tup);
    cout << std::get<0>(tup) << endl;

    // & means pass by ref outside, = is pass by val
    int y = 0;
    int counter = 0;
    std::for_each(intVec.begin(), intVec.end(), [&y, &counter] (int x)  {y += x; counter++;});
    cout << y << endl;
}

/*****************************************************/

class PassPointerClass{
public:
    std::shared_ptr<int> p1_;
    PassPointerClass(){
        cout << "constructor" << endl;
    }
    ~PassPointerClass(){
        cout << "destructor" << endl;
    }
    void passPointer(std::shared_ptr<int> p1){
        p1_ = p1;
        *p1_ = 6;
    }
};

class Node
{
    int value;
public:
    std::shared_ptr<Node> leftPtr;
    std::shared_ptr<Node> rightPtr;
    std::weak_ptr<Node> parentPtr; // change from shared to weak
    Node(int val) : value(val)     {
        std::cout<<"Contructor"<<std::endl;
    }
    ~Node()     {
        std::cout<<"Destructor"<<std::endl;
    }
};

void passUnique(std::unique_ptr<int>& ptr){
    cout << "unq: " << *ptr << endl;
}

void smartpointer(){

    std::shared_ptr<int> p1(new int());
    *p1 = 5;
    cout << p1.use_count() << endl;
    std::shared_ptr<PassPointerClass> passPointer(new PassPointerClass());
    passPointer->passPointer(p1);
    cout << p1.use_count() << endl;
    passPointer.reset();
    cout << p1.use_count() << endl;
    cout << *p1 << endl;
    p1 = nullptr;
    cout << p1.use_count() << endl;
    p1 = std::make_shared<int>(5);
    cout << p1.use_count() << endl;
    cout << p1.use_count() << endl;
    cout << *p1 << endl;
    int* p1xx = p1.get();
    cout << *p1xx << endl;

    std::shared_ptr<PassPointerClass> p3(new PassPointerClass[2], [](PassPointerClass* passPointerClassArray){
        cout << "special destructor" << endl;
        delete[] passPointerClassArray;
    });
    p3.reset();

    std::shared_ptr<int> p1x = std::make_shared<int>(5);
    std::shared_ptr<int> p2x(p1x);
    if(p1x == p2x){
        cout << "pointers equal" << endl;
        *p1x = 6;
        cout << *p2x << endl;
    }

    std::shared_ptr<Node> ptrNode = std::make_shared<Node>(4);
    ptrNode->leftPtr = std::make_shared<Node>(2);
    ptrNode->leftPtr->parentPtr = ptrNode;
    ptrNode->rightPtr = std::make_shared<Node>(5);
    ptrNode->rightPtr->parentPtr = ptrNode;
    std::cout<<"ptr reference count = "<<ptrNode.use_count()<<std::endl;
    std::cout<<"ptr->leftPtr reference count = "<<ptrNode->leftPtr.use_count()<<std::endl;
    std::cout<<"ptr->rightPtr reference count = "<<ptrNode->rightPtr.use_count()<<std::endl;

    std::shared_ptr<int> ptr = std::make_shared<int>(4);
    std::weak_ptr<int> weakPtr(ptr);
    std::shared_ptr<int> ptr_2 =  weakPtr.lock();
    if(ptr_2)
        std::cout<<(*ptr_2)<<std::endl;
    std::cout<<"Reference Count :: "<<ptr_2.use_count()<<std::endl;
    if(weakPtr.expired() == false)
        std::cout<<"Not expired yet"<<std::endl;

    // cant link unqiue pointer, only movable always guranteed to die
    std::unique_ptr<int> unqPointer(new int(12));
    passUnique(unqPointer);
    std::unique_ptr<int> newPtr(std::move(unqPointer));
    cout << *newPtr << endl;
    //cout << *unqPointer << endl;
}

int main (int argc, char ** argv)
{
    struct x{
        int m;
    };

    //iterators();

    //functor();

    //lamda();

    smartpointer();

    cout << "---" << endl;
    return (0);
}
