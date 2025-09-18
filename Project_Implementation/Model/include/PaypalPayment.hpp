#pragma once
#include "PaymentStrategy.hpp"

/**
 * @class PaypalPayment
 * @brief Implements the PaymentStrategy interface for PayPal payments.
 *
 * This class provides functionality to process and refund payments using PayPal.
 * It stores the PayPal email address associated with the payment and overrides
 * the necessary methods from the PaymentStrategy base class.
 *
 * @note The PayPal email must be provided for payment processing.
 */
class PaypalPayment : public PaymentStrategy {
    std::string paypalEmail;
    public:
        PaypalPayment() = default;
        PaypalPayment(std::string email);
        std::string processPayment(double amount) override;
        std::string refundPayment(double amount) override;
        std::string getType() const override { return "paypal"; }
        JSON getDetails() const override;

        ~PaypalPayment() = default;
};