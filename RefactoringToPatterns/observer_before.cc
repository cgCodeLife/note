

#include <iostream>
#include <memory>
#include <vector>

class Task {
public:
    virtual void Execute() = 0;
};

class Worker {
public:
    void AddTask(Task* task) {
        tasks_.push_back(std::unique_ptr<Task>(task));
    }

    void Execute() {
        for (auto& task : tasks_) {
            task->Execute();
        }
    }

private:
    std::vector<std::unique_ptr<Task>> tasks_;
};

class ARunner {
public:
    void Execute(const std::string& message) {
        std::cout << message << std::endl;
    }
};

class BRunner {
 public:
     void Execute(const std::string& message) {
         std::cout << message << std::endl;
     }
};

class ATask : public Task {
public:
    ATask() {
        runner_.reset(new ARunner());
    }

    void Execute() override {
        auto message = "ATask";
        runner_->Execute(message);
    }

private:
    std::unique_ptr<ARunner> runner_;
};

class BTask : public Task {
public:
    BTask() {
        runner_.reset(new BRunner());
    }

    void Execute() override {
        auto message = "BTask";
        runner_->Execute(message);
    }

private:
    std::unique_ptr<BRunner> runner_;
};

int main() {
    Worker worker;
    worker.AddTask(new ATask());
    worker.AddTask(new BTask());
    worker.Execute();
}
