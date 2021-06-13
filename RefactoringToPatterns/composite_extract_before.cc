
#include <stdlib.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

class Person {
public:
    Person(const std::string& name) : name_(name) {}

    ~Person() {}

    virtual std::string ToString() const {
        return name_;
    }

    std::string GetName() const {
        return name_;
    }

private:
    std::string name_;
};

enum TypeErrorcode : uint8_t {
    TYPE_ERR_OK = 0,
    TYPE_ERR_ALREADY_SON,
    TYPE_ERR_NOT_ENOUGH_MONEY,
    TYPE_ERR_SON_MISMATCH,
};

struct Errorcode {
    Errorcode(const TypeErrorcode code, const std::string& message) :
        code_(code), message_(message) {}

    TypeErrorcode code_;
    std::string message_;

    std::string ToString() {
        std::stringstream ss;
        ss << "code=" << code_
            << ", mesage=" << message_;
        return ss.str();
    }
};

class Father : public Person {
public:
    Father(const std::string& name, const int work_hours = 100, const int salary = 1) :
        Person(name),
        son_name_(""),
        work_hours_every_week_(work_hours),
        salary_(salary) {}

    void Work() {
        for (int i = 0; i < work_hours_every_week_; i++) {
            deposit_ += salary_;
        }
    }

    Errorcode SetSon(const std::string& son_name) {
        if (son_name_ != "") {
            return Errorcode(TYPE_ERR_ALREADY_SON, "i have son and not you");
        }
        son_name_ = son_name;
        return Errorcode(TYPE_ERR_OK, "welcome my son, i'm you father");
    }

    Errorcode GetMoney(const std::string& son_name, uint32_t how_much, uint32_t* cost) {
        if (son_name != son_name_) {
            return Errorcode(TYPE_ERR_SON_MISMATCH, "you not my son");
        }
        if (how_much > deposit_) {
            return Errorcode(TYPE_ERR_NOT_ENOUGH_MONEY, "you father not have enough money");
        }
        *cost = how_much;
        deposit_ -= how_much;
        return Errorcode(TYPE_ERR_OK, "ok");
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Father Info=" << " name=" << GetName()
            << ", my son=" << son_name_
            << ", deposit=" << deposit_
            << ", salary=" << salary_;
        return ss.str();
    }

private:
    int work_hours_every_week_ = 100;
    int salary_ = 1;

    uint64_t deposit_ = 0UL;
    std::string son_name_ = "";
};

class Mother : public Person {
public:
    Mother(const std::string& name, const int work_hours = 100, const int salary = 1) :
        Person(name),
        son_name_(""),
        work_hours_every_week_(work_hours),
        salary_(salary) {}

    void Work() {
        for (int i = 0; i < work_hours_every_week_; i++) {
            deposit_ += salary_;
        }
    }

    Errorcode SetSon(const std::string& son_name) {
        if (son_name_ != "") {
            return Errorcode(TYPE_ERR_ALREADY_SON, "i have son and not you");
        }
        son_name_ = son_name;
        return Errorcode(TYPE_ERR_OK, "welcome my son, i'm you mother");
    }

    Errorcode GetMoney(const std::string& son_name, uint32_t how_much, uint32_t* cost) {
        if (son_name != son_name_) {
            return Errorcode(TYPE_ERR_SON_MISMATCH, "you not my son");
        }
        if (how_much > deposit_) {
            return Errorcode(TYPE_ERR_NOT_ENOUGH_MONEY, "you father not have enough money");
        }
        *cost = how_much;
        deposit_ -= how_much;
        return Errorcode(TYPE_ERR_OK, "ok");
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "Mother Info=" << " name=" << GetName()
            << ", my son=" << son_name_
            << ", deposit=" << deposit_
            << ", salary=" << salary_;
        return ss.str();
    }

private:
    int work_hours_every_week_ = 100;
    int salary_ = 1;

    uint64_t deposit_ = 0UL;
    std::string son_name_ = "";
};

class User : public Person {
public:
    User(const std::string& name, const Father& father, const Mother& mother) :
        Person(name),
        father_(father),
        mother_(mother) {
            auto err_code = father_.SetSon(GetName());
            if (err_code.code_ != TYPE_ERR_OK) {
                std::cerr << "father set son failed. father="
                    << father_.ToString()
                    << ", error_code=" << err_code.ToString();
                abort();
            }
            err_code = mother_.SetSon(GetName());
            if (err_code.code_ != TYPE_ERR_OK) {
                std::cerr << "mother set son failed. mother="
                    << mother_.ToString()
                    << ", error_code=" << err_code.ToString();
                abort();
            }
            mother_.SetSon(GetName());
        }

    uint32_t GetMoney() {
        uint32_t deposit_father = 0U;
       uint32_t deposit_mother = 0U;
        auto error_code = father_.GetMoney(GetName(), 1, &deposit_father);
        if (error_code.code_ != TYPE_ERR_OK) {
            std::cout << "GetMoney from my father failed."
                << error_code.ToString() << std::endl;
        }
        error_code = mother_.GetMoney(GetName(), 1, &deposit_mother);
        if (error_code.code_ != TYPE_ERR_OK) {
            std::cout << "GetMoney from my mother failed."
                << error_code.ToString() << std::endl;
        }
        return deposit_father + deposit_mother;
    }

    void Work() {
        father_.Work();
        father_.Work();
        father_.Work();
        father_.Work();
        father_.Work();
        father_.Work();
        father_.Work();
        father_.Work();
        mother_.Work();
    }

private:
    Father father_;
    Mother mother_;
};

int main() {
    Father father("Caoge");
    Mother mother("YaoLi");
    User user("CaoJiaBao", father, mother);
    user.Work();
    std::cout << user.GetName() << " get money=" << user.GetMoney() << std::endl;
}
