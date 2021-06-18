
#include <iostream>
#include <string>

class B {
public:
    B(const std::string& name) : name_(name) {}

    std::string ToString() {
        return "B";
    }

private:
    std::string name_;
};

class C {
public:
    C(const std::string& name) : name_(name) {}
    
    std::string ToString() {
        return "C";
    }
private:
    std::string name_;
};

class D {
public:
    D(const std::string& name) : name_(name) {}

    std::string ToString() {
        return "D";
    }
private:
    std::string name_;
};

class A {
public:
    A(const B& b, const C& c, const D& d) : b_(b), c_(c), d_(d) {}

    std::string ToString() {
        return b_.ToString() + "," + c_.ToString() + "," + d_.ToString();
    }
private:
    B b_;
    C c_;
    D d_;
};

int main() {
    B b("B");
    C c("C");
    D d("D");

    A a(b, c, d);

    std::cout << a.ToString() << std::endl;
}
