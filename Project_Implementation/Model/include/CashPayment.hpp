#pragma once
#include "PaymentStrategy.hpp"

/**
 * @class CashPayment
 * @brief Implements the PaymentStrategy interface for cash payments.
 *
 * This class provides concrete implementations for processing and refunding payments made with cash.
 * It also provides methods to retrieve the payment type and details in JSON format.
 */
class CashPayment : public PaymentStrategy {
    public:
        std::string processPayment(double amount) override;
        std::string refundPayment(double amount) override;
        std::string getType() const override;
        JSON getDetails() const override;

        ~CashPayment() = default;
};