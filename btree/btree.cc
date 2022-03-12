
#include <iostream>
#include <stdint.h>
#include <vector>

template<class T>
class Btree {
private:
    struct Node {
        bool is_leaf_;
        uint8_t key_num_;
        std::vector<T> data_list_;  // The sake of simplicity key==value
        std::vector<Node*> child_list_;

        Node(bool is_leaf, uint8_t key_num, uint8_t key_max, uint8_t child_max) :
            is_leaf_(is_leaf), key_num_(key_num) {

            data_list_ = std::vector<T>(key_max);
            child_list_ = std::vector<Node*>(child_max, nullptr);
        }

        ~Node() {}
    };

public:
    Btree(uint8_t t) : t_(t), root_(nullptr) {
        key_min_ = t_ - 1;
        key_max_ = 2*t - 1;
        child_max_ = key_max_ + 1;
        child_min_ = key_min_ + 1;
    }

    ~Btree() {
        clear();
    }

    bool Insert(const T& key);
    bool Remove(const T& key);
    void Display() const;

private:
    Node* NewNode(bool is_leaf) {
        return new Node(is_leaf, 0, key_max_, child_max_);
    }
    void insertNoFull(Node* node, const T& key);
    void splitChild(Node* parent, Node* child, uint8_t index);
    bool contain(const T& key);
    bool search(Node* root, const T& key) const;
    void clear();
    void recursiveClear(Node* root);
    void deleteNode(Node*& node);
    void recursiveRemove(Node* root, const T& key);
    void mergeChild(Node* root, uint8_t index);
    T getPredecessor(Node* root);
    T getSuccessor(Node* root);
    void display(Node* root, int count) const;

private:
    uint8_t t_;
    uint8_t key_min_;
    uint8_t key_max_;
    uint8_t child_max_;
    uint8_t child_min_;
    Node* root_;

};

template<typename T>
bool Btree<T>::Insert(const T& key) {
    if (contain(key)) {
        return false;
    }
    if (!root_) {
        root_ = new Node(true/*is_leaf*/, 0 /*key_num*/, key_max_, child_max_);
    }
    if (root_->key_num_ >= key_max_) {
        Node* new_node = NewNode(false);
        new_node->child_list_[0] = root_;
        splitChild(new_node/*parent*/, root_ /*ready split child*/, 0/*split index*/);
        root_ = new_node;
    }
    insertNoFull(root_, key);
    return true;
}

template<typename T>
bool Btree<T>::Remove(const T& key) {
    if (!search(root_, key)) {
        return false;
    }
    if (root_->key_num_ == 1) {
        if (root_->is_leaf_) {
            clear();
            return true;
        }
        Node* left_child = root_->child_list_[0];
        Node* right_child = root_->child_list_[1];
        if (left_child->key_num_ == key_min_ && right_child->key_num_ == key_min_) {
            mergeChild(root_, 0);
            deleteNode(root_);
            root_ = left_child;
        }
    }
    recursiveRemove(root_, key);
    return true;
}

template<typename T>
void Btree<T>::insertNoFull(Node* node, const T& key) {
    uint8_t i = node->key_num_;
    if (node->is_leaf_) {
        while (i > 0 && key < node->data_list_[i - 1]) {
            node->data_list_[i] = node->data_list_[i - 1];
            --i;
        }
        node->data_list_[i] = key;
        ++node->key_num_;
        return;    
    }
    while (i > 0 && key < node->data_list_[i-1]) {
        --i;
    }
    Node* child = node->child_list_[i];
    if (child->key_num_ >= key_max_) {
        splitChild(node, child, i);
        if (key > node->data_list_[i]) {
            child = node->child_list_[i+1];
        }
    }
    insertNoFull(child, key);
}

template<typename T>
void Btree<T>::splitChild(Node* parent, Node* child, uint8_t index) {
    Node* right_node = NewNode(child->is_leaf_);
    right_node->key_num_ = key_min_;
    uint8_t i;
    for (i = 0; i < key_min_; ++i) {
        right_node->data_list_[i] = child->data_list_[i + child_min_];
    }
    if (!child->is_leaf_) {
        for (uint8_t i = 0; i < child_min_; ++i) {
            right_node->child_list_[i] = child->child_list_[i + child_min_];
        }
    }
    child->key_num_ = key_min_;

    for (uint8_t i = parent->key_num_; i > index; --i) {
        parent->child_list_[i + 1] = parent->child_list_[i];
        parent->data_list_[i] = parent->data_list_[i - 1];
    }
    ++parent->key_num_;
    parent->child_list_[index + 1] = right_node;
    parent->data_list_[index] = child->data_list_[key_min_];
}

template<typename T>
bool Btree<T>::contain(const T& key) {
    return search(root_, key);
}

template<typename T>
bool Btree<T>::search(Node* root, const T& key) const {
    if (!root) {
        return false;
    }

    uint8_t i = 0;
    while (i < root->key_num_ && key > root->data_list_[i]) {
        ++i;
    }
    if (i < root->key_num_ && key == root->data_list_[i]) {
        return true;
    }
    if (root->is_leaf_) {
        return false;
    }
    return search(root->child_list_[i], key);
}

template<typename T>
void Btree<T>::clear() {
    recursiveClear(root_);
    root_ = nullptr;
}

template<typename T>
void Btree<T>::recursiveClear(Node* root) {
    if (!root) {
        return;
    }
    if (!root->is_leaf_) {
        for (uint8_t i = 0; i <= root->key_num_; ++i) {
            recursiveClear(root->child_list_[i]);
        }
    }
    deleteNode(root);
}

template<typename T>
void Btree<T>::deleteNode(Node*& node) {
    if (node) {
        delete node;
        node = nullptr;
    }
}

template<typename T>
void Btree<T>::recursiveRemove(Node* root, const T& key) {
    uint8_t i = 0;
    while (i < root->key_num_ && key > root->data_list_[i]) {
        ++i;
    }
    if (i < root->key_num_ && key == root->data_list_[i]) {
        if (root->is_leaf_) {
            --root->key_num_;
            for (; i < root->key_num_; ++i) {
                root->data_list_[i] = root->data_list_[i + 1];
            }
            return;
        }
        Node* left_child = root->child_list_[i];
        Node* right_child = root->child_list_[i+1];
        if (left_child->key_num_ == child_min_) {
            T prev_key = getPredecessor(left_child);
            recursiveRemove(left_child, prev_key);
            root->data_list_[i] = prev_key;
            return;
        } else if (right_child->key_num_ == child_min_) {
            T next_key = getSuccessor(right_child);
            recursiveRemove(right_child, next_key);
            root->data_list_[i] = next_key;
            return;
        } else {
            mergeChild(root, i);
            recursiveRemove(left_child, key);
        }
    } else {
        Node* child = root->child_list_[i];
        if (child->key_num_ == key_min_) {
            Node* left = i > 0 ? root->child_list_[i] : nullptr;
            Node* right = i < root->key_num_ ? root->child_list_[i+1] : nullptr;
            uint8_t j = 0;
            if (left && left->key_num_ >= child_min_) {
                for (j = child->key_num_; j > 0; --j) {
                    child->data_list_[j] = child->data_list_[j-1];
                }
                child->data_list_[0] = root->data_list_[i-1];
                if (!left->is_leaf_) {
                    for (j = child->key_num_ + 1; j > 0; --j) {
                        child->child_list_[j] = child->child_list_[j-1];
                    }
                    child->child_list_[0] = left->child_list_[left->key_num_];
                }
                ++child->key_num_;
                root->data_list_[i] = left->data_list_[left->key_num_-1];
                --left->key_num_;
            } else if (right && right->key_num_ >= child_min_) {
                child->data_list_[child->key_num_] = root->data_list_[i];
                ++child->key_num_;
                root->data_list_[i] = right->data_list_[0];
                --right->key_num_;
                for (j = 0; j < right->key_num_; ++j) {
                    right->data_list_[j] = right->data_list_[j+1];
                }
                if (!right->is_leaf_) {
                    child->child_list_[child->key_num_] = right->child_list_[0];
                    for (j = 0; j <= right->key_num_; ++j) {
                        right->child_list_[j] = right->child_list_[j+1];
                    }
                }
            } else if (left) {
                mergeChild(root, i-1);
                child = left;
            } else if (right) {
                mergeChild(root, i);
            }
        }
        recursiveRemove(child, key);
    }
}

template<typename T>
void Btree<T>::mergeChild(Node* root, uint8_t index) {
    Node* left = root->child_list_[index];
    Node* right = root->child_list_[index+1];
    left->key_num_ = key_max_;
    left->data_list_[key_min_] = root->data_list_[index];
    uint8_t i = 0;
    for (i = 0; i < key_min_; ++i) {
        left->data_list_[i + key_min_ + 1] = right->data_list_[i];
    }
    if (!left->is_leaf_) {
        for (i = 0; i < child_min_; ++i) {
            left->child_list_[i+child_min_] = right->child_list_[i];
        }
    }
    --root->key_num_;
    for (i = index; i < root->key_num_; ++i) {
        root->data_list_[i] = root->data_list_[i+1];
        root->child_list_[i+1] = root->child_list_[i+2];
    }
    deleteNode(right);
}

template<typename T>
T Btree<T>::getPredecessor(Node* root) {
    while (!root->is_leaf_) {
        root = root->child_list_[root->key_num_];
    }
    return root->data_list_[root->key_num_-1];
}

template<typename T>
T Btree<T>::getSuccessor(Node* root) {
    while (!root->is_leaf_) {
        root = root->child_list_[0];
    }
    return root->data_list_[0];
}

template<typename T>
void Btree<T>::Display() const {
    display(root_, key_max_ * 2);
}

template<typename T>
void Btree<T>::display(Node* root, int count) const {
    if (!root) {
        return;
    }
    uint8_t i = 0;
    int j = 0;
    for (i = 0; i < root->key_num_; ++i) {
        if (!root->is_leaf_) {
            display(root->child_list_[i], count - 2);
        }
        for (j = count; j >= 0; --j) {
            std::cout << "-";
        }
        std::cout << root->data_list_[i] << std::endl;
    }
    if (!root->is_leaf_) {
        display(root->child_list_[i], count - 2);
    }
}

int main() {
    Btree<int> btree(3);
    for (int i = 0; i < 1000; ++i) {
        btree.Display();
        btree.Insert(i);
        std::cout << "=================" << std::endl;
    }
    for (int i = 0; i < 1000; ++i) {
        btree.Display();
        btree.Remove(i);
        std::cout << "=================" << std::endl;
    }
}
