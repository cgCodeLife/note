#include <iostream>
#include <memory>
#include <vector>

class ISortHandle {
public:
    virtual ~ISortHandle() {}

    virtual void Swap(int index) = 0;

    virtual bool OutOfOrder(int index) = 0;

    virtual int Length() = 0;
};

class BubbleSorter {
public:
    BubbleSorter(ISortHandle* sort_handle) {
        sort_handle_.reset(sort_handle);
    }

    int Sort() {
        length_ = sort_handle_->Length();
        if (length_ <= 1) {
            return operations_;
        }
        for (int next_to_last = length_ - 2; next_to_last >= 0; --next_to_last) {
            for (int index = 0; index <= next_to_last; ++index) {
                if(sort_handle_->OutOfOrder(index)) {
                    sort_handle_->Swap(index);
                    ++operations_;
                }
            }
        }
        return operations_;
    }

private:
    int operations_ = 0;
    int length_ = 0;
    std::unique_ptr<ISortHandle> sort_handle_;
};

class QuickBubbleSorter {
public:
    QuickBubbleSorter(ISortHandle* sort_handle) {
        sort_handle_.reset(sort_handle);
    }

    int Sort() {
        length_ = sort_handle_->Length();
        if (length_ <= 1) {
            return operations_;
        }
        bool this_pass_in_order = false;
        for (int next_to_last = length_ - 2; next_to_last >= 0 && !this_pass_in_order; --next_to_last) {
            this_pass_in_order = true;
            for (int index = 0; index <= next_to_last; ++index) {
                if (sort_handle_->OutOfOrder(index)) {
                    sort_handle_->Swap(index);
                    this_pass_in_order = false;
                }
                ++operations_;
            }
        }
        return operations_;
    }

private:
    int operations_ = 0;
    int length_ = 0;
    std::unique_ptr<ISortHandle> sort_handle_;
};

class IntSortHandle : public ISortHandle {
public:
    void Swap(int index) override {
        int temp = (*data_)[index];
        (*data_)[index] = (*data_)[index+1];
        (*data_)[index+1] = temp;
    }

    void SetData(std::vector<int>* data_ptr) {
        data_.reset(data_ptr);
    }

    int Length() override {
        return data_->size();
    }

    bool OutOfOrder(int index) override {
        return (*data_)[index] > (*data_)[index+1];
    }

private:
    std::unique_ptr<std::vector<int>> data_;
};

IntSortHandle* GenerateIntSortHandle() {
    IntSortHandle* int_sort_handle = new IntSortHandle();
    return int_sort_handle;
}

ISortHandle* GenerateSortHandle(int type = 0) {
    ISortHandle* sort_handle = nullptr;
    switch (type) {
        case 0:
            sort_handle = GenerateIntSortHandle();
            break;
        default:
            std::cout << "Unknow SortHandle type=" << type << std::endl;
            break;
    }
    return sort_handle;
}

int main() {
    {
        std::cout << "Bubble Sort test\n";
        auto* int_sort_handle = GenerateSortHandle(0);
        std::vector<int>* data = new std::vector<int>{89, 13, 231, 547};
        dynamic_cast<IntSortHandle*>(int_sort_handle)->SetData(data);

        std::unique_ptr<BubbleSorter> bubble_sorter(new BubbleSorter(int_sort_handle));
        bubble_sorter->Sort();
        for (auto& it : *data) {
            std::cout << it << " ";
        }
        std::cout << std::endl;
    }
    {
        std::cout << "Quick Sort test\n";
        auto* int_sort_handle = GenerateSortHandle(0);
        std::vector<int>* data = new std::vector<int>{89, 13, 231, 547};
        dynamic_cast<IntSortHandle*>(int_sort_handle)->SetData(data);

        std::unique_ptr<QuickBubbleSorter> quick_bubble_sorter(new QuickBubbleSorter(int_sort_handle));
        quick_bubble_sorter->Sort();
        for (auto& it : *data) {
            std::cout << it << " ";
        }
        std::cout << std::endl;
    }
}
