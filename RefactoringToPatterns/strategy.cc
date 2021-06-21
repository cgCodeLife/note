
#include <memory>
#include <stdint.h>
#include <vector>

struct Payment {
    Payment(double amount, uint64_t date) :
        amount_(amount), date_(date) {}

    double amount_ = 0.0;
    uint64_t date_ = 1U;
};

class CapitalStrategy;

class Loan {
public:
    static std::unique_ptr<Loan> NewRevolver();

    static std::unique_ptr<Loan> NewAdvisedLine();

    explicit Loan(CapitalStrategy* capital_strategy);

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

    void InsertPayment(const Payment& payment) {
        payments_.push_back(payment);
    }

    double GetUnusedPercentage() {
        return 0.0;
    }

    double GetOutstandingRiskAmount() {
        return outstanding_;
    }

    double GetUnusedRiskAmount() {
        return commitment_ - outstanding_;
    }

    double GetOutstanding() {
        return outstanding_;
    }

    double* GetExpiry() {
        return expiry_;
    }

    double* GetMaturity() {
        return maturity_;
    }

    double GetCommitment() {
        return commitment_;
    }

    std::vector<Payment>& GetPayments() {
        return payments_;
    }

    double Capital();

    double Duration();

private:
    double outstanding_ = 0.0;
    double* expiry_ = nullptr;
    double* maturity_ = nullptr;
    double commitment_ = 0.0;
    std::vector<Payment> payments_;

    std::unique_ptr<CapitalStrategy> capital_strategy_;
};

class CapitalStrategy {
public:
    double Capital(Loan* loan) {
        if (loan->GetExpiry() == nullptr && loan->GetMaturity() != nullptr) {
            return loan->GetCommitment() * loan->Duration() * riskFactor();
        }
        if (loan->GetExpiry() != nullptr && loan->GetMaturity() == nullptr) {
            if (loan->GetUnusedPercentage() != 1.0) {
                return loan->GetCommitment() * loan->GetUnusedPercentage() * loan->Duration() * riskFactor();
            } else {
                return (loan->GetOutstandingRiskAmount() * loan->Duration() * riskFactor())
                    + (loan->GetUnusedRiskAmount() * loan->Duration() * unusedRiskFactor());
            }
        }
        return 0.0;
    }

    double Duration(Loan* loan) {
        if (loan->GetExpiry() == nullptr && loan->GetMaturity() != nullptr) {
            return weightedAverageDuration(loan);
        } else if (loan->GetExpiry() != nullptr && loan->GetMaturity() == nullptr) {
            return *loan->GetExpiry();
        }
        return 0.0;
    }

    double weightedAverageDuration(Loan* loan) {
        double duration = 0.0;
        double weightedAverage = 0.0;
        double sumOfPayments = 0.0;
        for (auto& payment : loan->GetPayments()) {
            sumOfPayments += payment.amount_;
            weightedAverage += payment.date_ * payment.amount_;
        }
        if (loan->GetCommitment() != 0.0) {
            duration = weightedAverage / sumOfPayments;
        }
        return duration;
    }

    double riskFactor() {
        return risk_factor_;
    }

    double unusedRiskFactor() {
        return risk_rating_;
    }

    void SetRiskFactor(double risk_factor) {
        risk_factor_ = risk_factor;
    }

    void SetRiskRating(double risk_rating) {
        risk_rating_ = risk_rating;
    }

private:
    double risk_factor_ = 0.0;
    double risk_rating_ = 0.0;
};

class CapitalStrategyTermLoan : public CapitalStrategy {
};

class CapitalStrategyRevolver : public CapitalStrategy {
};

class CapitalStrategyAdvisedLine : public CapitalStrategy {
};

std::unique_ptr<Loan> Loan::NewRevolver() {
    std::unique_ptr<Loan> res;
    res.reset(new Loan(new CapitalStrategyRevolver()));
    return res;
}

std::unique_ptr<Loan> Loan::NewAdvisedLine() {
    std::unique_ptr<Loan> res;
    res.reset(new Loan(new CapitalStrategyAdvisedLine()));
    return res;
}

Loan::Loan(CapitalStrategy* capital_strategy) {
    capital_strategy_.reset(capital_strategy);
    capital_strategy_->SetRiskFactor(3.4);
    capital_strategy_->SetRiskRating(5.6);
}

double Loan::Capital() {
    return capital_strategy_->Capital(this);
}

double Loan::Duration() {
    return capital_strategy_->Duration(this);
}



int main() {
    auto loan = Loan::NewRevolver();

    loan->SetOutstanding(2.0);
    double dummy = 10.0;
    loan->SetExpiry(&dummy);
    loan->SetMaturity(nullptr);
    loan->SetCommitment(3.2);
    Payment payment = Payment{3.6, 360};
    loan->InsertPayment(payment);

    loan->Capital();
}
