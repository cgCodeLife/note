
#include <iostream>

class OutputBuilder {

};

class XMLBuilder : public OutputBuilder {
public:
    void Do() {}
};

class DOMBuilder : public OutputBuilder {
public:
    void Do() {}
};

class Test {

};

class UseXMLBuilder : public Test {
public:
    void testAddAboveTest() {
        auto* builder = new XMLBuilder();
        builder->Do();
        delete builder;
    }
};

class UseDOMBuilder : public Test {
public:
    void testAddAboveTest() {
        auto* builder = new DOMBuilder();
        builder->Do();
        delete builder;
    }
};

int main() {}
