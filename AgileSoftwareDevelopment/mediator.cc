
#include <iostream>
#include <string>

class Girl;
class Boy;

class Mediator {
public:
    static Mediator* GetMediator();

    Mediator();

    ~Mediator();

    void Register(const Boy& boy);

    void Register(const Girl& girl);

    bool FindPartner(const Boy& boy);

    bool FindPartner(const Girl& girl);

private:
    static Mediator* instance;
    Boy* boy_;
    Girl* girl_;
};

Mediator* instance = nullptr;


class Boy {
public:
    Boy(const std::string& name) : mediator_(GetMediator()), name_(name) {}

    ~Boy() {}

    Boy(const Boy& other) {
        name_ = other.GetName();
        mediator_ = other.GetMediator();
    }

    bool operator==(const Boy& boy) {
        return name_ == boy.GetName();
    }

    std::string GetName() const {
        return name_;
    }

    Mediator* GetMediator() const {
        return mediator_;
    }

    bool FindPartner(const Girl& girl) {
        return mediator_->FindPartner(girl);
    }

private:
    Mediator* mediator_;
    std::string name_;
};

class Girl {
public:
    Girl(const std::string& name) : mediator_(GetMediator()), name_(name) {}

    ~Girl() {}

    Girl(const Girl& other) {
        name_ = other.GetName();
        mediator_ = other.GetMediator();
    }

    bool operator==(const Girl& girl) {
        return name_ == girl.GetName();
    }

    std::string GetName() const {
        return name_;
    }

    Mediator* GetMediator() const {
        return mediator_;
    }

    bool FindPartner(const Boy& boy) {
        return mediator_->FindPartner(boy);
    }

private:
    Mediator* mediator_;
    std::string name_;
};

void Mediator::Register(const Boy& boy) {
    boy_ = new Boy("");
    *boy_ = boy;
}

void Mediator::Register(const Girl& girl) {
    girl_ = new Girl("");
    *girl_ = girl;
}

bool Mediator::FindPartner(const Boy& boy) {
    return *boy_ == boy;
}

bool Mediator::FindPartner(const Girl& girl) {
    return *girl_ == girl;
}

Mediator::Mediator() {
    boy_ = new Boy("");
    girl_ = new Girl("");
}

Mediator::~Mediator() {
    delete boy_;
    delete girl_;
    boy_ = nullptr;
    girl_ = nullptr;
}

Mediator* GetMediator() {
    if (instance == nullptr) {
        instance = new Mediator();
    }
    return instance;
}

int main() {
    Boy boy("caoge");
    Girl girl("yaoli");
    auto* mediator = GetMediator();
    mediator->Register(boy);
    mediator->Register(girl);
    std::cout << "boy:" << boy.GetName() << " find girl:" << girl.GetName()
        << " result:" << mediator->FindPartner(girl) << std::endl;

    std::cout << "girl:" << girl.GetName() << " find boy:" << boy.GetName()
        << " result:" << mediator->FindPartner(boy) << std::endl;
}
