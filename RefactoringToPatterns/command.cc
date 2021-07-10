#include <cassert>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Action {
    Action(const std::string& name) : name_(name) {}

    bool operator==(const Action& r) const {
        return name_ == r.name_;
    }

    bool operator!=(const Action& r) const {
        return !(name_== r.name_);
    }

    std::string name_;
};

struct ActionHash {
    std::size_t operator()(const Action& action) const {
        return std::hash<std::string>()(action.name_);
    }
};

struct Args {
    uint32_t num_ = 1;
};

std::string APPLE = "apple";
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

class IHandler {
public:
    virtual Result GoShopping(Args args) = 0;
};

class HandlerApple : public IHandler {
public:
    Result GoShopping(Args args) override {
        Result result;
       for (int i = 0; i < args.num_; ++i) {
            std::shared_ptr<Product> apple;
            apple.reset(new Apple());
            result.products_.push_back(apple);
        }
       return result;
    }
};

class HandlerBanana : public IHandler {
public:
    Result GoShopping(Args args) override {
        Result result;
       for (int i = 0; i < args.num_; ++i) {
            std::shared_ptr<Product> banana;
            banana.reset(new Banana());
            result.products_.push_back(banana);
        }
       return result;
    }
};

class HandlerBook : public IHandler {
public:
    Result GoShopping(Args args) override {
        Result result;
       for (int i = 0; i < args.num_; ++i) {
            std::shared_ptr<Product> book;
            book.reset(new Book());
            result.products_.push_back(book);
        }
       return result;
    }
};

class HandlerTea : public IHandler {
public:
    Result GoShopping(Args args) override {
        Result result;
       for (int i = 0; i < args.num_; ++i) {
            std::shared_ptr<Product> tea;
            tea.reset(new Tea());
            result.products_.push_back(tea);
        }
       return result;
    }
};

class Shopping {
public:
    Shopping() {
        std::vector<Action> actions = {APPLE, BANANA, BOOK, TEA};
        for (auto& action : actions) {
            RegisterHandler(action);
        }
    }

    Result GoShopping(Action action, Args args);
    
    void RegisterHandler(Action action) {
        std::shared_ptr<IHandler> handler = nullptr;
        if (action.name_ == APPLE) {
            handler = std::shared_ptr<IHandler>(new HandlerApple());
        } else if (action.name_ == BANANA) {
            handler = std::shared_ptr<IHandler>(new HandlerBanana());
        } else if (action.name_ == BOOK) {
            handler = std::shared_ptr<IHandler>(new HandlerBook());
        } else if (action.name_ == TEA) {
            handler = std::shared_ptr<IHandler>(new HandlerTea());
        } else {
            std::abort();
        }
        handlers_[action] = handler;
    }

private:
    std::shared_ptr<IHandler> GetHandler(Action action) {
        auto handler = handlers_.find(action);
        if (handler == handlers_.end()) {
            return nullptr;
        }
        return handler->second;
    }

private:
    std::unordered_map<Action, std::shared_ptr<IHandler>, ActionHash> handlers_;
};

Result Shopping::GoShopping(Action action, Args args) {
    auto handler = GetHandler(action);
    assert(handler != nullptr);
    return handler->GoShopping(args);
}

int main() {
    Shopping shopping;
    auto res = shopping.GoShopping(Action(APPLE), Args());
    for (auto& product : res.products_) {
        std::cout << product->ToString() << std::endl;
    }
}
