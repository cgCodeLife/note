
#include <iostream>

class A {
public:
    A() : num_(0) {}

    virtual void Func() {}
    
protected:
    int num_ = 0;
};

class B : public A {
public:
    void Func() override {
        Step1();
        Step2();
        Step3();
        std::cout << num_ << std::endl;
    }

    void Step1() {
        num_ += 1;
    }

    void Step2() {
        num_ += 2;
    }

    void Step3() {
        num_ += 3;
    }
};

class C : public A {
public:
    void Func() override {
        Step1();
        Step2();
        Step3();
        Step4();
        std::cout << num_ << std::endl;
    }

    void Step1() {
        num_ += 100;
    }

    void Step2() {
        num_ += 200;
    }

    void Step3() {
        num_ += 300;
    }

    void Step4() {
        num_ += 400;
    }
};

class D : public A {
public:
    void Func() override {
        Step1();
        Step2();
        Other();
        Step3();
        std::cout << num_ << std::endl;
    }
    
    void Step1() {
        num_ += 1000;
    }

    void Step2() {
        num_ += 2000;
    }

    void Step3() {
        num_ += 3000;
    }

    void Other() {
        num_ += 100000;
    }
};

int main() {
    B b;
    b.Func();
    C c;
    c.Func();
    D d;
    d.Func();
}
