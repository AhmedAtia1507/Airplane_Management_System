#pragma once
#include "PaymentStrategy.hpp"

/**
 * @class CreditPayment
 * @brief Implements the PaymentStrategy interface for credit card payments.
 *
 * This class handles payment processing, refunds, and provides details for credit card transactions.
 * It stores credit card information such as card number, expiration date, and CVV.
 *
 * @note Sensitive information is stored as strings; ensure secure handling.
 *
 * @constructor CreditPayment() - Default constructor.
 * @constructor CreditPayment(std::string number, std::string expiry, std::string cvv_code) - Initializes with card details.
 *
 * @fn std::string processPayment(double amount) override
 *      Processes a payment of the specified amount using credit card details.
 *      @param amount The amount to be paid.
 *      @return A string indicating the result of the payment process.
 *
 * @fn std::string refundPayment(double amount) override
 *      Processes a refund of the specified amount to the credit card.
 *      @param amount The amount to be refunded.
 *      @return A string indicating the result of the refund process.
 *
 * @fn std::string getType() const override
 *      Returns the type of payment strategy ("Credit").
 *      @return A string representing the payment type.
 *
 * @fn JSON getDetails() const override
 *      Retrieves the credit card details in JSON format.
 *      @return A JSON object containing card information.
 */
class CreditPayment : public PaymentStrategy {
    std::string creditCardNumber;
    std::string expirationDate;
    std::string cvv;

    std::string maskCardNumber() const;
    public:
        CreditPayment() = default;
        CreditPayment(std::string number, std::string expiry, std::string cvv_code);
        std::string processPayment(double amount) override;
        std::string refundPayment(double amount) override;
        std::string getType() const override;
        JSON getDetails() const override;

        ~CreditPayment() = default;
};