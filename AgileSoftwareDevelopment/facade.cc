
class SubSystemA {
public:
    void Start() {}
};

class SubSystemB {
public:
    void Start() {}
};

class SubSystemC {
public:
    void Start() {}
};

class Facade {
public:
    void Start() {
        system_a_.Start();
        system_b_.Start();
        system_c_.Start();
    }

private:
    SubSystemA system_a_;
    SubSystemB system_b_;
    SubSystemC system_c_;
};

int main() {
    Facade facade;
    facade.Start();
}
