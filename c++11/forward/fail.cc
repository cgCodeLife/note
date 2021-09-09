#include <iostream>
#include <vector>

void f(std::vector<int>&& v) {
    std::cout << "f, " << &v << std::endl;
}

template<typename T>
void fwd(T&& params) {
    std::cout << "fwd, " << &params << std::endl;
    f(std::forward<T>(params));
}

void fwd2(std::vector<int>&& params) {
    std::cout << "fwd2, " << &params << std::endl;
    f(std::forward<std::vector<int>>(params));
}

int main() {
    f({1,2,3});
    // fwd({1,2,3});  build error
    auto data = {1,2,3};
    fwd(data);
    fwd2(data);
}
