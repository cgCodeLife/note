
#include <iostream>
#include <string>

class B {
public:
    B() {}

    B(const std::string& name) : name_(name) {}

    std::string ToString() {
        return "B";
    }

private:
    std::string name_;
};

class C {
public:
    C() {}

    C(const std::string& name) : name_(name) {}
    
    std::string ToString() {
        return "C";
    }
private:
    std::string name_;
};

class D {
public:
    D() {}

    D(const std::string& name) : name_(name) {}

    std::string ToString() {
        return "D";
    }
private:
    std::string name_;
};

class A {
public:
    A() {}

    A(const B& b, const C& c, const D& d) : b_(b), c_(c), d_(d) {}

    std::string ToString() {
        return b_.ToString() + "," + c_.ToString() + "," + d_.ToString();
    }
private:
    B b_;
    C c_;
    D d_;
};


class Builder {
public:
    Builder SetB() {
        b_ = B("B");
        return *this;
    }

    Builder SetC() {
        c_ = C("C");
        return *this;
    }

    Builder SetD() {
        d_ = D("D");
        return *this;
    }

    void Build(A* a) {
        *a = A(b_, c_, d_);
    }

private:
    B b_;
    C c_;
    D d_;
};

int main() {
    Builder builder;
    A a;
    builder.SetB().SetC().SetD().Build(&a);
    std::cout << a.ToString() << std::endl;
}
