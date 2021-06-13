
#include <vector>

class AttributeDescriptor {
public:
    AttributeDescriptor() {}

    virtual ~AttributeDescriptor() {}
};

class BooleanDescriptor : public AttributeDescriptor {
public:
    BooleanDescriptor() {}
};

class DefaultDescriptor : public AttributeDescriptor {
public:
    DefaultDescriptor() {}
};

class ReferenceDescriptor : public AttributeDescriptor {
public:
    ReferenceDescriptor() {}
};

class Factory {
public:
    Factory() {}

    AttributeDescriptor* ForDefault() {
        return new DefaultDescriptor();
    }

    AttributeDescriptor* ForBoolean() {
        return new BooleanDescriptor();
    }

    AttributeDescriptor* ForReference() {
        return new ReferenceDescriptor();
    }
};


int main() {
    std::vector<AttributeDescriptor*> result;
    Factory factory;
    auto* default_type = factory.ForDefault();
    result.push_back(default_type);
};
