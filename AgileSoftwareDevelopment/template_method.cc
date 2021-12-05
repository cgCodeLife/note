#include <iostream>
#include <string>
#include <vector>

class Task {
public:
    void Run() {
        while (!done()) {
            execute();
        }
    }

    void Do() {
        Run();
    }

protected:
   virtual bool done() {return true;}

    virtual void execute() {}
};

class A : public Task {
public:
    bool done() override {
        return left_ < 0;
    }

    void execute() override {
        --left_;
        if (left_ % 1000 == 0) {
            std::cout << "left=" << left_ << std::endl;
        }
    }

private:
    int64_t left_ = 100000;
};

int main() {
    A* a_ptr = new A();
    std::unique_ptr<Task> task_ptr(a_ptr);
    task_ptr->Do();
}
