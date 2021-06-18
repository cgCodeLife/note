
#include <iostream>
#include <string>
#include <vetor>

class HitStayResponse {
public:
    std::string ToString() {
        return "HitStayResponse";
    }
};

class TestHitStayResponse : public HitStayResponse {
public:
    std::string ToString() {
        return "TestHitStayResponse";
    }
};

class TestAlwaysHitResponse : public HitStayResponse {
public:
    std::string ToString() {
        return "TestAlwaysHitResponse";
    }
};

class Blackjack {
    public:
};
