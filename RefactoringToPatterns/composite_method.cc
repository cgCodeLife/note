
#include <utility>
#include <vector>

class List {
public:
    List() : size_(0), read_only_(false) {}

    void UpdateReadoly(bool read_only) {
        read_only_ = read_only;
    }

    void Add(int i) {
        if (read_only_) {
            return;
        }

        if (atCapacity()) {
            grow();
        }

        addElement(i);
    }

    bool atCapacity() {
        return size_ + 1 > elements_.size();
    }

    void grow() {
        int new_size = elements_.size() > 0 ? (elements_.size() * GROWTH_INCREMENT) : GROWTH_INCREMENT;
        std::vector<int> new_elements(new_size);
        new_elements.assign(elements_.begin(), elements_.end());
        elements_ = std::move(new_elements);
    }

    void addElement(int element) {
        elements_.push_back(element);
        ++size_;
    }

private:
    static constexpr int GROWTH_INCREMENT = 10;

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
