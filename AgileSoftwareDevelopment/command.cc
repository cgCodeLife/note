#include <iostream>
#include <memory>
#include <string>
#include <vector>

class ICommand {
public:
    ICommand() {}
    
    virtual ~ICommand() {}

    virtual void Do() = 0;
};

class Command : public ICommand {
public:
    void Save(const std::string& data) {data_ = data;}

    void Do() override {
        std::cout << data_ << std::endl;
    }

    std::string& Data() {return data_;}

private:
    std::string data_;
};

class Printer {
public:
    void AddCommand(ICommand* command) {
        std::unique_ptr<ICommand> command_ptr(command);
        commands_.emplace_back(std::move(command_ptr));
    }

    void Run() {
        for (auto& command : commands_) {
            command->Do();
        }
    }

private:
    std::vector<std::unique_ptr<ICommand>> commands_;
};

int main() {
    Printer p;
    for (int i = 0; i < 1000; ++i) {
        auto* command = new Command();
        command->Save(std::to_string(i));
        p.AddCommand(command);
    }
    p.Run();
}

