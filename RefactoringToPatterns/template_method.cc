
#include <iostream>
#include <memory>

class A {
public:
    A() : num_(0) {}

    virtual void Func() {
        Step1();
        Step2();
    }

    virtual void Func(int) {
        A::Func();
        Step3();
    }

    virtual void Func(int, int) {
        A::Func(0);
        Step4();
    }

    virtual void Step1() {
        num_ += 1;
    }

    virtual void Step2() {
        num_ += 2;
    }

    virtual void Step3() {
        num_ += 3;
    }

    virtual void Step4() {
        num_ += 4;
    }
    
protected:
    int num_ = 0;
};

class B : public A {
public:
    void Func() override {
        A::Func(0);
        std::cout << num_ << std::endl;
    }
};

class C : public A {
public:
    void Func() override {
        A::Func(0, 0);
        std::cout << num_ << std::endl;
    }

    void Step1() override {
        num_ += 100;
    }

    void Step2() override {
        num_ += 200;
    }

    void Step3() override {
        num_ += 300;
    }

    void Step4() override {
        num_ += 400;
    }
};

class D : public A {
public:
    void Func() override {
        A::Func();
        Other();
        Step3();
        std::cout << num_ << std::endl;
    }
    
    void Step1() override {
        num_ += 1000;
    }

    void Step2() override {
        num_ += 2000;
    }

    void Step3() override {
        num_ += 3000;
    }

    void Other() {
        num_ += 100000;
    }
};

int main() {
    std::unique_ptr<A> b;
    b.reset(new B());
    b->Func();


    std::unique_ptr<A> c;
    c.reset(new C());
    c->Func();

    std::unique_ptr<A> d;
    d.reset(new D());
    d->Func();
}
