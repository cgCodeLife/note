
#include <vector>

class AttributeDescriptor {
public:
};

class BooleanDescriptor : public AttributeDescriptor {

};

class DefaultDescriptor : public AttributeDescriptor {

};

class ReferenceDescriptor : public AttributeDescriptor {

};

int main() {
    std::vector<AttributeDescriptor*> result;
    result.push_back(new DefaultDescriptor());
    result.push_back(new ReferenceDescriptor());
    result.push_back(new BooleanDescriptor());
}
