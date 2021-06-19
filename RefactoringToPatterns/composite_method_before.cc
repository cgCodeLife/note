
#include <utility>
#include <vector>

class List {
public:
    List() : size_(0), read_only_(false) {}

    void UpdateReadoly(bool read_only) {
        read_only_ = read_only;
    }

    void Add(int i) {
        if (!read_only_) {
            int new_size = size_ + 1;
            if (new_size > elements_.size()) {
                std::vector<int> new_elements(new_size*10);
                new_elements.assign(elements_.begin(), elements_.end());
                elements_ = std::move(new_elements);
            }
            elements_.push_back(i);
            size_++;
        }
    }

private:
    std::vector<int> elements_;
    int size_;
    bool read_only_;
};

int main() {
    List list;
    for (int i = 0; i < 10000; ++i) {
        list.Add(i);
    }
}
