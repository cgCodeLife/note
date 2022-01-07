
#include <iostream>
#include <string>
#include <vector>

class Entry {
public:
    virtual ~Entry() {}

    virtual std::string GetName() = 0;

    virtual uint32_t GetSize() = 0;

    virtual Entry* Add(Entry* entry) = 0;

    virtual void Print(const std::string& prefix) = 0;
};

class File : public Entry {
public:
    File(const std::string name, uint32_t size) : name_(name), size_(size) {}

    std::string GetName() override {return name_;}

    uint32_t GetSize() override {return size_;}

    void Print(const std::string& prefix) override {std::cout << prefix + "/" + name_ << std::endl;}

    Entry* Add(Entry* entry) override {return nullptr;}

private:
    std::string name_;
    uint32_t size_;
};

class Directory : public Entry {
public:
    Directory(const std::string name) : name_(name) {}

    std::string GetName() override {return name_;}

    uint32_t GetSize() override {
        uint32_t size = 0U;
        for (auto entry : directory_) {
            size += entry->GetSize();
        }
        return size;
    }

    Entry* Add(Entry* entry) override {
        directory_.emplace_back(entry);
        return this;
    }

    void Print(const std::string& prefix) override {
        std::cout << prefix << "/" << name_ << std::endl;
        for (auto entry : directory_) {
            entry->Print(prefix + "/" + name_);
        }
    }

private:
    std::string name_;
    std::vector<Entry*> directory_;
};

int main() {
    Directory* root_dir = new Directory("root");
    Directory* bin_dir = new Directory("bin");
    Directory* tmp_dir = new Directory("tmp");
    Directory* usr_dir = new Directory("usr");
    root_dir->Add(bin_dir);
    root_dir->Add(tmp_dir);
    root_dir->Add(usr_dir);
    bin_dir->Add(new File("vi", 1000));
    bin_dir->Add(new File("gcc", 2000000));
    root_dir->Print(".");
}
