#include <iostream>

class Implementor {
public:
    virtual ~Implementor() {}

    virtual void Operation() = 0;
};

class ConcreteImplementorA : public Implementor {
public:
    void Operation() override {
        std::cout << "ConcreteImplementorA" << std::endl;
    }
};

class ConcreteImplementorB : public Implementor {
public:
    void Operation() override {
        std::cout << "ConcreteImplementorB" << std::endl;
    }
};

class Abstraction {
public:
    virtual ~Abstraction() {}

    void SetImplementor(Implementor* implementor) {
        implementor_ = implementor;
    }

    virtual void Operation() {
        implementor_->Operation();
    }

protected:
    Implementor* implementor_;
};

class RefinedAbstraction : public Abstraction {
public:
    void Operation() override {
        implementor_->Operation();
    }
};

int main() {
    Abstraction* r = new RefinedAbstraction();
    ConcreteImplementorA* ca = new ConcreteImplementorA();
    ConcreteImplementorB* cb = new ConcreteImplementorB();
    r->SetImplementor(ca);
    r->Operation();
    r->SetImplementor(cb);
    r->Operation();

    delete ca;
    delete cb;
    delete r;
    return 0;
}
