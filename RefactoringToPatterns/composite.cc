
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

enum TypeColor : uint8_t {
    YELLOW,
    PINK,
    WHITE,
    RED,
    BLUE,
    BLACK,
};

enum TypeSize : uint8_t {
    LARGE,
    MEDIUM,
    SMALL,
};

class Product {
public:
    Product(const std::string& logo, const TypeColor& color,
            const float price, const TypeSize& size) :
        logo_(logo),
        color_(color),
        price_(price),
        size_(size) {}

    TypeColor Color() const {
        return color_;
    }
    
    TypeSize Size() const {
        return size_;
    }

    float Price() const {
        return price_;
    }

    std::string ToString() {
        std::stringstream ss;
        ss << "logo=" << logo_
            << ", color=" << color_
            << ", price=" << price_
            << ", size=" << size_;
        return ss.str();
    }

private:
    std::string logo_;
    TypeColor color_;
    float price_;
    TypeSize size_;
};

class Spec {
public:
    virtual ~Spec() {}

    virtual bool IsSatisfiedBy(const Product&) const = 0;

    virtual std::string ToString() = 0;
};

class ColorSpec : public Spec {
public:
    ColorSpec(const TypeColor& color) : color_(color) {}

    bool IsSatisfiedBy(const Product& product) const override {
       return product.Color() == color_;
    }

    std::string ToString() {
        return "ColorSpec";
    }

private:
    TypeColor color_;
};

class SizeSpec : public Spec {
public:
    SizeSpec(const TypeSize size) : size_(size) {}
    
    bool IsSatisfiedBy(const Product& product) const override {
        return product.Size() == size_;
    }

    std::string ToString() override {
        return "SizeSpec";
    }

private:
    TypeSize size_;
};

class BelowPriceSpec : public Spec {
public:
    BelowPriceSpec(const float max_price) : max_price_(max_price) {}

    bool IsSatisfiedBy(const Product& product) const {
        return max_price_ > product.Price();
    }
    std::string ToString() override {
        return "BelowPriceSpec";
    }
private:
    float max_price_;
};

class CompositeSpec : public Spec {
public:
    void Add(std::unique_ptr<Spec> spec) {
        specs_.emplace_back(spec.release());
    }

    bool IsSatisfiedBy(const Product& product) const override {
        bool satisfied_all = true;
        for (auto& spec : specs_) {
            satisfied_all &= spec->IsSatisfiedBy(product);
        }
        return satisfied_all;
    }

    std::string ToString() override {
        std::string res = "CompositeSpec info=";
        for (auto& spec : specs_) {
            res += "," + spec->ToString();
        }
        res += "\n\n\n";
    }

private:
    std::vector<std::unique_ptr<Spec>> specs_;
};


class ProductRepository {
public:   
    void Add(const Product& product) {
        products_.emplace_back(product);
    }

    std::vector<Product> SelectBy(const Spec* spec) const {
        std::vector<Product> found;
        for (auto& product : products_) {
            if (spec->IsSatisfiedBy(product)) {
                found.emplace_back(product);
            }
        }
        return found;
    }

private:
    std::vector<Product> products_;
};

class Test {
public:
    void SetUp() {
        auto fireTrunk = Product("Fire Trunk", TypeColor::RED, 8.95, TypeSize::SMALL);
        auto barbieClassic = Product("Barbie Classic", TypeColor::YELLOW, 15.95, TypeSize::SMALL);
        auto frisbee = Product("Frisbee", TypeColor::PINK, 9.99, TypeSize::LARGE);
        auto baseball = Product("Baseball", TypeColor::WHITE, 8.95, TypeSize::LARGE);
        auto toyConvertible = Product("Toy Porsche Convertible", TypeColor::RED, 230.00, TypeSize::MEDIUM);

        repository_.Add(fireTrunk);
        repository_.Add(barbieClassic);
        repository_.Add(frisbee);
        repository_.Add(baseball);
        repository_.Add(toyConvertible);
    }

    void FindByColor() {
        std::unique_ptr<Spec> color_spec;
        color_spec.reset(new ColorSpec(TypeColor::RED));

        CompositeSpec composite_spec;
        composite_spec.Add(std::move(color_spec));

        auto products = repository_.SelectBy(&composite_spec); 

        for (auto& product : products) {
            std::cout << product.ToString() << std::endl;
        }
    }

    void FindByColorSizeAndBelowPrice() {
        std::unique_ptr<Spec> color_spec;
        color_spec.reset(new ColorSpec(TypeColor::RED));

        std::unique_ptr<Spec> size_spec;
        size_spec.reset(new SizeSpec(TypeSize::SMALL));

        std::unique_ptr<Spec> below_price_spec;
        below_price_spec.reset(new BelowPriceSpec(10.00));

        CompositeSpec composite_spec;
        composite_spec.Add(std::move(color_spec));
        composite_spec.Add(std::move(size_spec));
        composite_spec.Add(std::move(below_price_spec));

        auto products = repository_.SelectBy(&composite_spec);

        for (auto& product : products) {
            std::cout << product.ToString() << std::endl;
        }
    }

    void Run() {
        std::cout << "FindByColor...." << std::endl; 
        FindByColor();
        std::cout << "FindByColorSizeAndBelowPrice......" << std::endl;
        FindByColorSizeAndBelowPrice();
    }

private:
    ProductRepository repository_;
};

int main() {
    Test t;
    t.SetUp();
    t.Run();
}
