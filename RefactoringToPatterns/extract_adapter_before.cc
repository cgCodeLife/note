
#include <cstdlib>

class A {
public:
    void   Funcv1() {}
    void   Funcv2() {}
    void   Funcv3() {}
    void   Funcv4() {}

    void Run(const int);
};

void A::Run(const int version) {
    if (version == 1) {
        Funcv1();
    } else if (version == 2) {
        Funcv2();
    } else if (version == 3) {
        Funcv3();
    } else if (version == 4) {
        Funcv4();
    } else {
        abort();
    }
}

int main() {
    A a;
    a.Run(1);
}