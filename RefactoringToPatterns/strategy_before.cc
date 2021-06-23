
#include <stdint.h>
#include <vector>

struct Payment {
    Payment(double amount, uint64_t date) :
        amount_(amount), date_(date) {}

    double amount_ = 0.0;
    uint64_t date_ = 1U;
};

class Loan {
public:
    void SetOutstanding(double outstanding) {
        outstanding_ = outstanding;
    }

    void SetExpiry(double* expiry) {
        expiry_ = expiry;
    }

    void SetMaturity(double* maturity) {
        maturity_ = maturity;
    }

    void SetCommitment(double commitment) {
        commitment_ = commitment;
    }

    void SetRiskFactor(double risk_factor) {
        risk_factor_ = risk_factor;
    }

    void SetRiskRating(double risk_rating) {
        risk_rating_ = risk_rating;
    }

    void InsertPayment(const Payment& payment) {
        payments_.push_back(payment);
    }

    double capital() {
        if (expiry_ == nullptr && maturity_ != nullptr) {
            return commitment_ * duration() * riskFactor();
        }
        if (expiry_ != nullptr && maturity_ == nullptr) {
            if (getUnusedPercentage() != 1.0) {
                return commitment_ * getUnusedPercentage() * duration() * riskFactor();
            } else {
                return (outstandingRiskAmount() * duration() * riskFactor())
                    + (unusedRiskAmount() * duration() * unusedRiskFactor());
            }
        }
        return 0.0;
    }

    double getUnusedPercentage() {
        return 0.0;
    }

    double outstandingRiskAmount() {
        return outstanding_;
    }

    double unusedRiskAmount() {
        return commitment_ - outstanding_;
    }

    double unusedRiskFactor() {
        return risk_factor_;
    }

    double duration() {
        if (expiry_ == nullptr && maturity_ != nullptr) {
            return weightedAverageDuration();
        } else if (expiry_ != nullptr && maturity_ == nullptr) {
            return *expiry_;
        }
        return 0.0;
    }

    double weightedAverageDuration() {
        double duration = 0.0;
        double weightedAverage = 0.0;
        double sumOfPayments = 0.0;
        for (auto& payment : payments_) {
            sumOfPayments += payment.amount_;
            weightedAverage += payment.date_ * payment.amount_;
        }
        if (commitment_ != 0.0) {
            duration = weightedAverage / sumOfPayments;
        }
        return duration;
    }

    double riskFactor() {
        return risk_factor_;
    }

    double ununsedRiskFactor() {
        return risk_rating_;
    }

private:
    double outstanding_ = 0.0;
    double* expiry_ = nullptr;
    double* maturity_ = nullptr;
    double commitment_ = 0.0;
    double risk_factor_ = 0.0;
    double risk_rating_ = 0.0;
    std::vector<Payment> payments_;
};

int main() {
    Loan loan;
    loan.SetOutstanding(2.0);

    double dummy = 10.0;
    loan.SetExpiry(&dummy);

    loan.SetMaturity(nullptr);

    loan.SetCommitment(3.2);

    loan.SetRiskFactor(4.3);

    loan.SetRiskRating(5.6);

    Payment payment = Payment{3.6, 360};
    loan.InsertPayment(payment);

    loan.capital();
}
