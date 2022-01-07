
#include <iostream>
#include <string>
#include <vector>

class Entry {
public:
    virtual ~Entry() {}

    std::string GetName() = 0;

    uint32_t GetSize() = 0;

    Entry* Add(Entry* entry) = 0;

    void Print() = 0;
};

class File : public Entry {

};

class Directory : public Entry {

};
