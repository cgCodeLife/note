#include <iostream>

class Subject {
public:
    virtual ~Subject() {}

    virtual void Request() = 0;
};

class RealSubject : public Subject {
public:
    void Request() {std::cout << "RealSubject" << std::endl;}
};

class Proxy : public Subject {
public:
    void Request() {
        if (real_subject_ == nullptr) {
            real_subject_ = new RealSubject();
        }
        real_subject_->Request();
    }

    ~Proxy() {
        if (real_subject_) {
            delete real_subject_;
            real_subject_ = nullptr;
        }
    }

private:
    RealSubject* real_subject_;
};

int main() {
    Proxy* p = new Proxy();
    p->Request();

    delete p;
    return 0;
}
