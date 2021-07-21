
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

class Spec {
public:
    virtual bool IsSatisfy(const Product& product) = 0;
};

class ColorSpec : public Spec {
public:
    ColorSpec(Product::TypeColor color) : color_(color) {}

    bool IsSatisfy(const Product& product) override {
        return color_ == product.GetColor();
    }

private:
    Product::TypeColor color_;
};

class PriceSpec : public Spec {
public:
    PriceSpec(float price) : price_(price) {}

    bool IsSatisfy(const Product& product) override {
        return price_ == product.GetPrice();
    }

private:
    float price_;
};

class SizeSpec : public Spec {
public:
    SizeSpec(Product::TypeSize size) : size_(size) {}

    bool IsSatisfy(const Product& product) override {
        return size_ == product.GetSize();
    }

private:
    Product::TypeSize size_;
};

class BelowPriceSpec : public Spec {
public:
    BelowPriceSpec(float price) : max_price_(price) {}

    bool IsSatisfy(const Product& product) override {
        return max_price_ > product.GetPrice();
    }

private:
    float max_price_;
};

class NotSpec : public Spec {
public:
  NotSpec(std::vector<Spec*> spec_list) {
      spec_list_ = spec_list;
  }

  bool IsSatisfy(const Product& product) override {
    auto all_not_satisfy = true;
      for (auto& spec : spec_list_) {
          if (spec->IsSatisfy(product)) {
              all_not_satisfy = false;
              break;
          }
      }
      return all_not_satisfy;
  }

private:
    std::vector<Spec*> spec_list_;  // NOT OWN
};

class AndSpec : public Spec {
public:
  AndSpec(std::vector<Spec*> spec_list) {
      spec_list_ = spec_list;
  }

    bool IsSatisfy(const Product& product) override {
        auto all_satisfy = true;
        for (auto& spec : spec_list_) {
            if (!spec->IsSatisfy(product)) {
                all_satisfy = false;
                break;
            }
        }
        return all_satisfy;
    }

private:
    std::vector<Spec*> spec_list_;  // NOT OWN
};

class ProductFinder {
public:
    ProductFinder(ProductRepository* repository) {
        repository_.reset(repository);
    }

    std::vector<Product> IsSatisfy(Spec* spec) {
        std::vector<Product> found_products;
        auto products = repository_->GetProducts();
        for (auto it : products) {
            auto product = it.second;
            if (spec->IsSatisfy(product)) {
                found_products.push_back(product);
            }
        }
        return found_products;
    }

    std::vector<Product> ByColor(Product::TypeColor color) {
        ColorSpec spec(color);
        return IsSatisfy(&spec);
    }

    std::vector<Product> ByPrice(float price) {
        PriceSpec spec(price);
        return IsSatisfy(&spec);
    }

    std::vector<Product> ByColorSizeAndBelowPrice(Product::TypeColor color,
                                                  Product::TypeSize size,
                                                  float price) {
        ColorSpec color_spec(color);
        BelowPriceSpec below_price_spec(price);
        SizeSpec size_spec(size);
        AndSpec spec({&color_spec, &below_price_spec, &size_spec});
        return IsSatisfy(&spec);
    }

    std::vector<Product> BelowPriceAvoidingColor(float price, Product::TypeColor color) {
        BelowPriceSpec below_price_spec(price);
        auto color_spec = ColorSpec(color);
        NotSpec not_spec({&color_spec});
        AndSpec spec({&below_price_spec, &not_spec});
        return IsSatisfy(&spec);
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
