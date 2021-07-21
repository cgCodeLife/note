
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <string>

class Product {
public:
    enum TypeColor {
        TYPE_COLOR_PINK,
        TYPE_COLOR_RED,
        TYPE_COLOR_YELLOW,
        TYPE_COLOR_WHITE,
        TYPE_COLOR_NUM
    };

    enum TypeSize {
        TYPE_SIZE_SMALL,
        TYPE_SIZE_MEDIUM,
        TYPE_SIZE_LARGE,
        TYPE_SIZE_NUM
    };

    Product(const std::string& name, TypeColor color, float price, TypeSize size) :
        name_(name), color_(color), price_(price), size_(size) {}

    Product(const Product& product) {
        name_ = product.GetName();
        color_ = product.GetColor();
        price_ = product.GetPrice();
        size_ = product.GetSize();
    }

    Product& operator=(const Product& product) {
        name_ = product.GetName();
        color_ = product.GetColor();
        price_ = product.GetPrice();
        size_ = product.GetSize();
        return *this;
    }

    std::string GetName() const {
        return name_;
    }

    TypeColor GetColor() const {
        return color_;
    }

    TypeSize GetSize() const {
        return size_;
    }

    float GetPrice() const {
        return price_;
    }

    std::string ToString() const {
        std::ostringstream oss;
        oss << "name=" << name_
            << ", color=" << color_
            << ", price=" << price_
            << ", size=" << size_;
        return oss.str();
    }

private:
    std::string name_;
    TypeColor color_;
    float price_;
    TypeSize size_;
};

class ProductRepository {
public:
    void Add(const Product& product) {
        if (products_.find(product.GetName()) != products_.end()) {
            return;
        }
        products_.insert({product.GetName(), product});
        // products_[product.GetName()] = product;  // build error: need use Product() constructor
    }

    std::unordered_map<std::string, Product> GetProducts() const {
        return products_;
    }
private:
    std::unordered_map<std::string, Product> products_;
};

class ProductFinder {
public:
    ProductFinder(ProductRepository* repository) {
        repository_.reset(repository);
    }

    std::vector<Product> ByColor(Product::TypeColor color) {
        std::vector<Product> found_products;
        auto products = repository_->GetProducts();
        for (auto it : products) {
            auto product = it.second;
            if (product.GetColor() == color) {
                found_products.push_back(product);
            }
        }
        return found_products;
    }

    std::vector<Product> ByPrice(float price) {
        std::vector<Product> found_products;
        auto products = repository_->GetProducts();
        for (auto it : products) {
            auto product = it.second;
            if (product.GetPrice() == price) {
                found_products.push_back(product);
            }
        }
        return found_products;
    }

    std::vector<Product> ByColorSizeAndBelowPrice(Product::TypeColor color,
                                                  Product::TypeSize size,
                                                  float price) {
        std::vector<Product> found_products;
        auto products = repository_->GetProducts();
        for (auto it : products) {
            auto product = it.second;
            if (product.GetColor() == color &&
                product.GetSize() == size &&
                product.GetPrice() < price) {
                found_products.push_back(product);
            }
        }
        return found_products;
    }

    std::vector<Product> BelowPriceAvoidingColor(float price, Product::TypeColor color) {
        std::vector<Product> found_products;
        auto products = repository_->GetProducts();
        for (auto it : products) {
            auto product = it.second;
            if (product.GetPrice() < price &&
                product.GetColor() != color) {
                found_products.push_back(product);
            }
        }
        return found_products;
    }

private:
    std::unique_ptr<ProductRepository> repository_;
};

class Test {
public:
    Test() {
        finder_.reset(new ProductFinder(createProductRepository()));
    }

    ProductRepository* createProductRepository() {
        auto fire_trunk = Product("Fire Trunk", Product::TYPE_COLOR_RED, 8.95,
                                      Product::TYPE_SIZE_SMALL);
        auto barbie_classic = Product("Baribe Classic", Product::TYPE_COLOR_YELLOW, 15.95,
                                          Product::TYPE_SIZE_MEDIUM);
        auto frisbee = Product("Frisbee", Product::TYPE_COLOR_PINK, 9.99,
                                   Product::TYPE_SIZE_LARGE);
        auto baseball = Product("Baseball", Product::TYPE_COLOR_WHITE, 8.95,
                                    Product::TYPE_SIZE_LARGE);
        auto toy_convertible = Product("Toy Porsche Convertible", Product::TYPE_COLOR_RED, 230.00,
                                           Product::TYPE_SIZE_SMALL);

        ProductRepository* repository = new ProductRepository();
        repository->Add(fire_trunk);
        repository->Add(barbie_classic);
        repository->Add(frisbee);
        repository->Add(baseball);
        repository->Add(toy_convertible);
        return repository;
    }

    void testFindByColor() {
        auto found_products = finder_->ByColor(Product::TYPE_COLOR_WHITE);
        std::cout << "testFindByColor with white\n";
        for (auto product : found_products) {
            std::cout << product.ToString() << std::endl;
        }
    }

    void testFindByPrice() {
        auto found_products = finder_->ByPrice(9.99);
        std::cout << "testFindByPrice equal=" << 9.99 << std::endl;
        for (auto product : found_products) {
            std::cout << product.ToString() << std::endl;
        }
    }

    void  testFindByColorSizeAndBelowPrice() {
        auto found_products = finder_->ByColorSizeAndBelowPrice(Product::TYPE_COLOR_RED,
                                                                Product::TYPE_SIZE_SMALL,
                                                                10.00);
        for (auto product : found_products) {
            std::cout << "testFindByColorSizeAndBelowPrice."
                    << " product=" << product.ToString()
                    << std::endl;
        }
    }

    void testFindBelowPriceAvoidingAColor() {
        auto found_products = finder_->BelowPriceAvoidingColor(9.00, Product::TYPE_COLOR_WHITE);
        for (auto product : found_products) {
            std::cout << "testFindBelowPriceAvoidingAColor."
                    << " product=" << product.ToString()
                    << std::endl;
        }
    }

    void Run() {
        testFindByColor();
        testFindByPrice();
        testFindByColorSizeAndBelowPrice();
        testFindBelowPriceAvoidingAColor();
    }

private:
    std::unique_ptr<ProductFinder> finder_;
};

int main() {
    Test test;
    test.Run();
}
