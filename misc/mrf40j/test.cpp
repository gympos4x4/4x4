#include <iostream>
using namespace std;

struct Hello {
    int a, b;
    struct {
        int c, d;
    } m;
};

int main()
{
    Hello h{1, 2, {3, 4}};

    cout << h.a << h.b << h.m.c << h.m.d << endl;

    return 0;
}