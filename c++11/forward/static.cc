#include <utility>

void f(int&& a) {
}

template<typename T>
void fwd(T&& param) {
    f(std::forward<T>(param));
}

class A {
public:
    static const std::size_t val = 28;
};

const std::size_t A::val;

int main() {
    f(A::val);
    fwd(A::val);
}
