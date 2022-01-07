#include <iostream>
#include <list>

class Element;
class ConcreteElementA;
class ConcreteElementB;

class Visitor {
public:
    virtual void VisitConcreteElementA(ConcreteElementA* ca) = 0;
    virtual void VisitConcreteElementB(ConcreteElementB* cb) = 0;
};

class ConcreteVisitorA : public Visitor {
public:
    void VisitConcreteElementA(ConcreteElementA* ca) {
        std::cout << "ConcreteVisitorA::VisitConcreteElementA" << std::endl;
    }

    void VisitConcreteElementB(ConcreteElementB* cb) {
        std::cout << "ConcreteVisitorA::VisitConcreteElementB" << std::endl;
    }
};

class ConcreteVisitorB : public Visitor {
public:
    void VisitConcreteElementA(ConcreteElementA* ca) {
        std::cout << "ConcreteVisitorB::VisitConcreteElementA" << std::endl;
    }

    void VisitConcreteElementB(ConcreteElementB* cb) {
        std::cout << "ConcreteVisitorB::VisitConcreteElementB" << std::endl;
    }
};

class Element {
public:
    virtual void Accept(Visitor* visitor) = 0;
};

class ConcreteElementA : public Element {
public:
    void Accept(Visitor* visitor) {
        std::cout << "ConcreteElementA: ";
        visitor->VisitConcreteElementA(this);
    }

    void OperationA() {
        std::cout << "OperationA" << std::endl;
    }
};

class ConcreteElementB : public Element {
public:
    void Accept(Visitor* visitor) {
        std::cout << "ConcreteElementB: ";
        visitor->VisitConcreteElementB(this);
    }

    void OperationB() {
        std::cout << "OperationB" << std::endl;
    }
};

class ObjectStructure {
public:
    void Attach(Element* element) {
        elements_.push_back(element);
    }

    void Detach(Element* element) {
        elements_.push_back(element);
    }

    void Accept(Visitor* visitor) {
        for (auto e = elements_.begin(); e != elements_.end(); ++e) {
            (*e)->Accept(visitor);
        }
    }

private:
    std::list<Element*> elements_;
};

int main() {
    ObjectStructure* o = new ObjectStructure();

    ConcreteElementA* ca = new ConcreteElementA();
    ConcreteElementB* cb = new ConcreteElementB();

    o->Attach(ca);
    o->Attach(cb);

    ConcreteVisitorA* cva = new ConcreteVisitorA();
    ConcreteVisitorB* cvb = new ConcreteVisitorB();

    o->Accept(cva);
    o->Accept(cvb);

    delete o;
    delete ca;
    delete cb;
    delete cva;
    delete cvb;
}
