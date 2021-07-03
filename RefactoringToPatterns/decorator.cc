
#include <iostream>
#include <string>

class VisualComponent {
public:
    VisualComponent();

    virtual void Draw();

    virtual void Resize();
};

class Decorator : public VisualComponent {
public:
    Decorator(VisualComponent*);

    virtual void Draw() override;

    virtual  void Resize() override;

private:
    VisualComponent* component_;
};

void Decorator::Draw() {
    component_->Draw();
}

void Decorator::Resize() {
    component_->Resize();
}

class BorderDecorator : public Decorator {
public:
    BorderDecorator(VisualComponent*, int borderWidth);

    virtual void Draw();

private:
    void DrawBorder(int);

private:
    int width_;
};

void BorderDecorator::Draw() {
    Decorator::Draw();
    DrawBorder(width_);
}

void BorderDecorator::DrawBorder(int width) {
    std::cout << "witdh:" << width << std::endl;
}

class TextView : public VisualComponent {
public:
    virtual void Draw() override;

    virtual  void Resize() override;
};

void TextView::Draw() {
    std::cout << "TextView Draw" << std::endl;
}

void TextView::Resize() {}

int main() {
    auto view = new BorderDecorator(new TextView(), 1);
    view->Draw();
    delete view;
}