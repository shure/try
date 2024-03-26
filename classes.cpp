
#include <stdio.h>
#include <utility>

class A {

public:
    int val;

public:
    A() : val{} { printf("A()\n"); }
    A(int val) : val{val} { printf("A(int)\n"); }
    A(const A& a) : val{a.val} { printf("A(const A&)\n"); }
    A(A&& a) : val{a.val} { printf("A(A&&)\n"); }
    A& operator=(const A& a) { val = a.val; printf("operator=(const A&)\n"); return *this; }
    A& operator=(A&& a) { val = a.val; printf("operator=(A&&)\n"); return *this; }
};

void func1(A a) {
    printf("func1\n");
}

A func2() {
    printf("func2\n");
    A rv = 2;
    return rv;
}

int main()
{
    A x, y, z(10);
    A t = x;
    func1(t);
    func1(std::move(z));
    A xx(func2());
    printf("xx.val %d\n", xx.val);
    y = func2();
    printf("y.val %d\n", y.val);
}
