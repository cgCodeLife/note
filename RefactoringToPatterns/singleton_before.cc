
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class HitStayResponse {
public:
    virtual ~HitStayResponse() {}

    virtual std::string ToString() {
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

class Console {
public:
    static HitStayResponse* ObtainHitStayResponse() {
        return hit_stay_response_;
    }

    static void SetPlayerResponse(HitStayResponse* hit_stay_response) {
        hit_stay_response_ = hit_stay_response;
    }

private:
    static HitStayResponse* hit_stay_response_;
};

HitStayResponse* Console::hit_stay_response_ = nullptr;

class Blackjack {
public:
    void Play() {
        auto* hitStayResponse = Console::ObtainHitStayResponse();
        std::cout << hitStayResponse->ToString() << std::endl;
    }
};

int main() {
    {
        Blackjack black_jack;
        std::unique_ptr<HitStayResponse> hit_stay_response;
        hit_stay_response.reset(new TestHitStayResponse());
        Console::SetPlayerResponse(hit_stay_response.get());
        black_jack.Play();
    }

    {
        Blackjack black_jack;
        std::unique_ptr<HitStayResponse> hit_stay_response;
        hit_stay_response.reset(new TestAlwaysHitResponse());
        Console::SetPlayerResponse(hit_stay_response.get());
        black_jack.Play();
    }
} 
