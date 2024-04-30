
#include <stdio.h>
#include <utility>

class A {

public:
    int val;

public:
    A() : val{} {
        printf("A()\n");
    }

    A(int val) : val{val} {
        printf("A(int); // val=%d\n", val);
    }

    A(const A& a) : val{a.val} {
        printf("A(const A&); // val=%d\n", val);
    }

    A(A&& a) : val{a.val} {
        printf("A(A&&); // val=%d\n", val);
    }

    A& operator=(const A& a) {
        val = a.val;
        printf("operator=(const A&); val=%d\n", val);
        return *this;
    }

    A& operator=(A&& a) {
        val = a.val;
        printf("operator=(A&&); val=%d\n", val);
        return *this;
    }

    ~A() {
        printf("~A(); // val=%d\n", val);
    }
};

void func1(A a) {
    printf("func1 a.val=%d\n", a.val);
}

void func2(const A& a) {
    printf("func2 a.val=%d\n", a.val);
}

A func3() {
    printf("func3\n");
    A rv = 2;
    return rv;
}

A obj(90);

int main()
{
    printf(" - begin - \n");

    A x, y, z(10);

    printf(" - 2 - \n");

    A t = x;

    printf(" - 3 - \n");

    func1(t);

    printf(" - 4 - \n");

    func1(std::move(z));

    printf(" - 5 - \n");

    func2(t);

    printf(" - 6 - \n");

    A xx(func3());

    printf("xx.val %d\n", xx.val);
    printf(" - 7 - \n");

    y = func3();

    printf("y.val %d\n", y.val);

    printf(" - end - \n");
}
