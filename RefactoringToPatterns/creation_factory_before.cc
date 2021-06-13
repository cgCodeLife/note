
#include <iostream>
#include <string>

class Parser {
public:
    Parser() : should_remote_escape_characters_(false),
        should_decode_(false) {}

    void SetStringNodeDecoding(bool should_decode) {
        should_decode_= should_decode;
    }

    void SetRemoveEscapeCharacters(bool should_remove_escape_chars) {
        should_remote_escape_characters_ = should_remove_escape_chars;
    }

    bool ShouldDecodeStringNodes() {
        return should_decode_;
    }

    bool ShouldRemoveEscapeCharacters() {
        return should_remote_escape_characters_;
    }

private:
    bool should_remote_escape_characters_;
    bool should_decode_;
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
    static Node* CreateStringNode(bool should_decode) {
        if (should_decode) {
            return new DecodeStringNode();
        }
        return new StringNode();
    }

    std::string ToString() {
        return "StringNode";
    }
};

class StringParser {
public:
    StringParser(const Parser& parser) : parser_(parser) {}

    Node* Find() {
        return StringNode::CreateStringNode(parser_.ShouldDecodeStringNodes());
    }

    void SetStringNodeDecoding(bool should_decode) {
        parser_.SetStringNodeDecoding(should_decode);
    }

private:
    Parser parser_;
};

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
