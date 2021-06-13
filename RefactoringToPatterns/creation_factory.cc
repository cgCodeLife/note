
#include <iostream>
#include <string>

class Node;

class NodeFactory {
public:
    NodeFactory() {}

    ~NodeFactory() {}

    Node* CreateStringNode();

    void SetStringNodeDecoding(bool should_decode) {
        should_decode_= should_decode;
    }

    bool ShouldDecodeStringNodes() {
        return should_decode_;
    }

private:
    bool should_decode_;
};

class Parser {
public:
    explicit Parser() : should_remote_escape_characters_(false) {
        node_factory_ = new NodeFactory();
    }

    ~Parser() {
        delete node_factory_;
    }

    void SetRemoveEscapeCharacters(bool should_remove_escape_chars) {
        should_remote_escape_characters_ = should_remove_escape_chars;
    }

    bool ShouldRemoveEscapeCharacters() {
        return should_remote_escape_characters_;
    }

    NodeFactory* GetNodeFactory() {
        return node_factory_;
    }

private:
    bool should_remote_escape_characters_;
    NodeFactory* node_factory_;
};

class Node {
public:
    virtual ~Node() {}

    virtual std::string ToString() = 0;
};

class DecodeStringNode : public Node {
public:
    std::string ToString() {
        return "DecodeStringNode";
    }
};

class StringNode : public Node {
public:
    std::string ToString() {
        return "StringNode";
    }
};

class StringParser {
public:
    StringParser(const Parser& parser) : parser_(parser) {}

    Node* Find() {
        auto node_factory = parser_.GetNodeFactory();
        return node_factory->CreateStringNode();
    }

    void SetStringNodeDecoding(bool should_decode) {
        auto node_factory = parser_.GetNodeFactory();
        node_factory->SetStringNodeDecoding(should_decode);
    }

private:
    Parser parser_;
};

Node* NodeFactory::CreateStringNode() {
    if (should_decode_) {
        return new DecodeStringNode();
    }
    return new StringNode();
}

int main() {
    Parser parser;
    StringParser string_parser(parser);
    auto node1 = string_parser.Find();
    std::cout << node1->ToString() << std::endl;
    string_parser.SetStringNodeDecoding(true);
    auto node2 = string_parser.Find();
    std::cout << node2->ToString() << std::endl;
    delete node1;
    delete node2;
}
