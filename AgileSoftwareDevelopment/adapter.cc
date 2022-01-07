#include <iostream>

class Target {
public:
    virtual ~Target() {}
    virtual void Request() = 0;
};

class Adaptee {
public:
    void SpecificRequest() {std::cout << "Adaptee" << std::endl;}
};

class Adapter : public Target {
public:
    Adapter() {adaptee_ = new Adaptee();}

    ~Adapter() {delete adaptee_;}

    void Request() {adaptee_->SpecificRequest();}

private:
    Adaptee* adaptee_;
};

int main() {
    Target* target = new Adapter();
    target->Request();

    delete target;
    return 0;
}
