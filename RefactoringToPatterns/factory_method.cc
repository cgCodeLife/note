
#include <iostream>
#include <string>

class OutputBuilder {
public:
    virtual ~OutputBuilder() {}

    virtual void Do() = 0;
};

class XMLBuilder : public OutputBuilder {
public:
    void Do() override {
        std::cout << "XMLBuilder" << std::endl;
    }
};

class DOMBuilder : public OutputBuilder {
public:
    void Do() override {
        std::cout << "DOMBuilder" << std::endl;
    }
};

class Test {
public:
    virtual ~Test() {}

    void TestAddAboveTest() {
        auto* builder = CreateBuilder();
        builder->Do();
        delete builder;
    }

    virtual OutputBuilder* CreateBuilder() = 0;
};

class UseXMLBuilder : public Test {
public:
    OutputBuilder* CreateBuilder() override {
        return new XMLBuilder();
    }
};

class UseDOMBuilder : public Test {
public:
    OutputBuilder* CreateBuilder() override {
        return new DOMBuilder();
    }
};

int main() {
    UseXMLBuilder xml_builder;
    UseDOMBuilder dom_builder;

    xml_builder.TestAddAboveTest();
    dom_builder.TestAddAboveTest();
}
