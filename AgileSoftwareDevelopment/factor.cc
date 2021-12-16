
#include <iostream>
#include <string>


class IProduct {
public:
    virtual std::string ToString() = 0;
};

class IFactor {
public:
    virtual IProduct* GetProduct(const std::string& name) = 0;
};

class Book : public IProduct {
public:
    Book(const std::string& name) : name_(name) {}

    ~Book() {}

    std::string ToString() override {
        return "Book:" + name_;
    }

private:
    std::string name_;
};

class BookFactor : public IFactor {
public:
    IProduct* GetProduct(const std::string& name) override {
        return new Book(name);
    }
};

class Computer : public IProduct {
public:
    Computer(const std::string& name) : name_(name) {}

    ~Computer() {}

    std::string ToString() override {
        return "Computer:" + name_;
    }

private:
    std::string name_;
};

class ComputerFactor : public IFactor {
public:
    IProduct* GetProduct(const std::string& name) override {
        return new Computer(name);
    }
};

int main() {
    {
        // book
        BookFactor* book_factor_ptr = new BookFactor();
        auto* book_ptr = book_factor_ptr->GetProduct("C++ Primary");
        std::cout << book_ptr->ToString() << std::endl;
    }
    {
        // factor
        ComputerFactor* computer_factor_ptr = new ComputerFactor();
        auto* computer_ptr = computer_factor_ptr->GetProduct("Mac OS Pro");
        std::cout << computer_ptr->ToString() << std::endl;
    }
}
