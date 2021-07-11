
#include <memory>
#include <vector>

class Abstract {
public:
   virtual void Func() = 0;
};

class Adapter {
public:
    Adapter(Abstract* data) {
        data_.reset(data);
    }

    void Func() {
        data_->Func();
    }

private:
    std::unique_ptr<Abstract> data_;
};

class B : public Abstract {
public:
    void Func() override {
        BFunc1();
        BFunc2();
    }

    void BFunc1() {}

    void BFunc2() {}
};

class C : public Abstract {
public:
    void Func() override {
        CFunc1();
        CFunc2();
    }

    void CFunc1() {}

    void CFunc2() {}
};

class A {
public:
    A() {
        adapters_.push_back(std::unique_ptr<Adapter>(new Adapter(new B())));
        adapters_.push_back(std::unique_ptr<Adapter>(new Adapter(new C())));
        for (auto& adapter : adapters_) {
            adapter->Func();
        }
    }

private:
    std::vector<std::unique_ptr<Adapter>> adapters_;
};

int main() {
    A();
}
