#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Action {
    Action(const std::string& name) : name_(name) {}

    bool operator==(const Action& r) {
        return name_ == r.name_;
    }

    std::string name_;
};

struct Args {
    uint32_t num_ = 1;
};

std::string APPLE = std::string("apple");
std::string BANANA = "banana";
std::string BOOK = "book";
std::string TEA = "tea";

class  Product {
public:
    Product(const std::string& name) : name_(name) {}
    virtual ~Product() {}

    std::string ToString() const {
        return name_;
    }

    std::string name_;
};

class Apple : public Product {
public:
    Apple() : Product("apple") {}
};

class Banana : public Product {
public:
    Banana() : Product("banana") {}
};

class Book : public Product {
public:
    Book() : Product("book")  {}  
};

class Tea : public Product {
public:
    Tea() : Product("tea") {}
};

struct Result {
 std::vector<std::shared_ptr<Product>> products_;
};

class Shopping {
public:
    Result GoShopping(Action action, Args args);
};

Result Shopping::GoShopping(Action action, Args args) {
    Result result;
    if (action.name_ == APPLE) {
        for (int i = 0; i < args.num_; ++i) {
            std::shared_ptr<Product> apple;
            apple.reset(new Apple());
            result.products_.push_back(apple);
        }
    } else if (action.name_ == BANANA) {
        for (int i = 0; i < args.num_; ++i) {
            std::shared_ptr<Product> banana;
            banana.reset(new Banana());
            result.products_.push_back(banana);
        }
    } else if (action.name_ == BOOK) {
        for (int i = 0; i < args.num_; ++i) {
            std::shared_ptr<Product> book;
            book.reset(new Book());
            result.products_.push_back(book);
        }
    } else if (action.name_ == TEA) {
        for (int i = 0; i < args.num_; ++i) {
            std::shared_ptr<Product> tea;
            tea.reset(new Tea());
            result.products_.push_back(tea);
        }
    }
    return result;
}

int main() {
    Shopping shopping;
    auto res = shopping.GoShopping(Action(APPLE), Args());
    for (auto& product : res.products_) {
        std::cout << product->ToString() << std::endl;
    }
}
