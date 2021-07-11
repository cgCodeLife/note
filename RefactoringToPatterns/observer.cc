

#include <iostream>
#include <memory>
#include <vector>

class Runner {
public:
    virtual void Execute(const std::string& message) = 0;

    virtual std::string ToString() = 0;
};

class Observer {
public:
    void Register(Runner* runner) {
        runners_.push_back(std::unique_ptr<Runner>(runner));
    }

    void Execute() {
        for (auto& runner : runners_) {
            runner->Execute(runner->ToString());
        }
    }

private:
    std::vector<std::unique_ptr<Runner>> runners_;
};

class Worker {
public:
    Worker(Observer* observer) {
        observer_.reset(observer);
    }

    void Execute() {
        observer_->Execute();
    }

private:
    std::unique_ptr<Observer> observer_;
};

class ARunner : public Runner {
public:
    void Execute(const std::string& message) {
        std::cout << message << std::endl;
    }

    std::string ToString() {
        return "ARunner";
    }
};

class BRunner : public Runner {
 public:
     void Execute(const std::string& message) {
         std::cout << message << std::endl;
     }

     std::string ToString() {
         return "BRunner";
     }
};

int main() {
    auto observer = new Observer();
    observer->Register(new ARunner());
    observer->Register(new BRunner());
    auto worker = std::unique_ptr<Worker>(new Worker(observer));
    worker->Execute();
}
