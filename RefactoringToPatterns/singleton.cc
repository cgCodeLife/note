
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

class Blackjack {
public:
    Blackjack() : hit_stay_response_(nullptr) {}

    HitStayResponse* ObtainHitStayResponse() {
        return hit_stay_response_;
    }

    void SetPlayerResponse(HitStayResponse* hit_stay_response) {
        hit_stay_response_ = hit_stay_response;
    }

    void Play() {
        auto* hitStayResponse = Console::ObtainHitStayResponse();
        std::cout << hitStayResponse->ToString() << std::endl;
    }

private:
    HitStayResponse* hit_stay_response_;
};

int main() {
    {
        Blackjack black_jack;
        std::unique_ptr<HitStayResponse> hit_stay_response;
        hit_stay_response.reset(new TestHitStayResponse());
        black_jack.SetPlayerResponse(hit_stay_response.get());
        black_jack.Play();
    }

    {
        Blackjack black_jack;
        std::unique_ptr<HitStayResponse> hit_stay_response;
        hit_stay_response.reset(new TestAlwaysHitResponse());
        black_jack.SetPlayerResponse(hit_stay_response.get());
        black_jack.Play();
    }
} 
