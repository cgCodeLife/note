
#include <iostream>

class A {
public:
    virtual void Func() {
        std::cout << "Base Func" << std::endl;
    }

    void Run() {
        Func();
    }
};

class Version1 : public A {
public:
    void Func() override {
        std::cout << "v1 Func" << std::endl;
    }
};

class Version2 : public A {
public:
    void Func() override {
        std::cout << "v2 Func" << std::endl;
    }
};

class Version3 : public A {
public:
    void Func() override {
        std::cout << "v3 Func" << std::endl;
    }
};

class Version4 : public A {
public:
    void Func() override {
        std::cout << "v4 Func" << std::endl;
    }
};

int main() {
    A* a = new Version1();
    a->Run();
}