#include <iostream>
#include <thread>

using namespace std;

int foo(int a, string b)
{
    while(1)
        cout<<a<<"\t"<<b<<endl;
}

int main()
{
    thread t(foo, 1, "sfjb");
    while(1)
        cout<<"fbdfb\n";
    t.join();
}