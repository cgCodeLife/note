#include <iostream>
#include <memory>
#include <string>
#include <vector>

////////////////////不使用模式的方式//////////////////////////////
class BubbleSorter {
public:
      int sort(std::vector<int>* data) {
          operations = 0;
          if (data->empty()) {
              return operations;
          }
          
          for (int next_to_last = data->size() - 2; next_to_last >= 0; --next_to_last) {
              for (int index = 0; index <= next_to_last; ++index) {
                  comapreAndSwamp(data, index);
              }
          }
          return operations;
      }

      void Test() {
          std::vector<int> data = {89,13,231,547};
          sort(&data);
          for (auto& it : data) {
              std::cout << it << " ";
          }
          std::cout << std::endl;
      }

private:
      void swap(std::vector<int>* data_ptr, int index) {
          auto& data = *data_ptr;
          int temp = data[index];
          data[index] = data[index+1];
          data[index+1] = temp;
      }
      void comapreAndSwamp(std::vector<int>* data_ptr, int index) {
          auto& data = *data_ptr;
          if (data[index] > data[index+1]) {
              swap(data_ptr, index);
              ++operations;
          }
      }

public:
      int operations = 0;
};

///////////////////////////////template_method模式///////////////////////////////////
class IBubbleSorter {

protected:
    int doSort() {
        operations = 0;
        if (length <= 1) {
            return operations;
        }
        for (int next_to_last = length - 2; next_to_last >= 0; --next_to_last) {
            for (int index = 0; index <= next_to_last; ++index) {
              if (outOfOrder(index)) {
                swap(index);
                ++operations;
               }
            }
        }
        return operations;
    }

    virtual void swap(int index) = 0;

    virtual bool outOfOrder(int index) = 0;

private:
    int operations = 0;

protected:
    int length = 0;
};

class IntBubbleSorter : public IBubbleSorter {
public:
    int sort(std::vector<int>* data_ptr) {
        data_.reset(data_ptr);
        length = data_->size();
        return doSort();
    }

    void Test() {
        std::vector<int>* data = new std::vector<int>{89,13,231,547};
        sort(data);
        for (auto& it : *data_) {
            std::cout << it << " ";
        }
        std::cout << std::endl;
    }

protected:
    void swap(int index) {
        int temp = (*data_)[index];
        (*data_)[index] = (*data_)[index+1];
        (*data_)[index+1] = temp;
    }

    bool outOfOrder(int index) {
        return (*data_)[index] > (*data_)[index+1];
    }

private:
    std::unique_ptr<std::vector<int>> data_;
};

int main() {
    BubbleSorter b;
    std::cout << "normal test \n";
    b.Test();
    std::unique_ptr<IntBubbleSorter> ib(new IntBubbleSorter());
    std::cout << "template test\n";
    ib->Test();
}
