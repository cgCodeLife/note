
class B {
public:
    void BFunc1() {}

    void BFunc2() {}
};

class C {
public:
    void CFunc1() {}

    void CFunc2() {}
};

class A {
public:
    A() {
        b.BFunc1();
        b.BFunc2();
        c.CFunc1();
        c.CFunc2();
    }

private:
    B b;
    C c;
};

int main() {
    A();
}
