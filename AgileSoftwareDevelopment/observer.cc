#include <iostream>
#include <list>
#include <string>

class Observer {
public:
    virtual void Update() = 0;
};

class Subject {
public:
    void Attach(Observer* observer) {observers_.push_back(observer);}
    void Detach(Observer* observer) {observers_.remove(observer);}
    void Notify() {
        for (auto observer = observers_.begin(); observer != observers_.end(); ++observer) {
            (*observer)->Update();
        }
    }

private:
    std::list<Observer*> observers_;
};

class ConcreteSubject : public Subject {
public:
    std::string GetState() {return subject_state_;}

    void SetState(const std::string state) {subject_state_ = state;}

private:
    std::string subject_state_;
};

class ConcreteObserver : public Observer {
public:
    ConcreteObserver(ConcreteSubject* s, const std::string n) :
        name_(n),
        subject_(s),
        observer_state_("") {}

    void Update() override {
        observer_state_ = subject_->GetState();
        std::cout << "observer:" << name_ << ", its new state is:" << observer_state_ << std::endl;
    }

private:
    std::string name_;
    ConcreteSubject* subject_;
    std::string observer_state_;
};

int main() {
    ConcreteSubject* s = new ConcreteSubject();
    s->SetState("ABC");
    ConcreteObserver* ox = new ConcreteObserver(s, "X");
    ConcreteObserver* oy = new ConcreteObserver(s, "Y");
    ConcreteObserver* oz = new ConcreteObserver(s, "Z");
    s->Attach(ox);
    s->Attach(oy);
    s->Attach(oz);
    s->Notify();
    s->SetState("XYZ");
    s->Notify();
    delete ox;
    delete oy;
    delete oz;
}
