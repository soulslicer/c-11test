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

int main (int argc, char ** argv)
{
    struct x{
        int m;
    };

    //iterators();

    //functor();

    lamda();

    return (0);
}
