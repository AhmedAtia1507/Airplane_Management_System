#include "../include/PaymentStrategyFactory.hpp"
#include <stdexcept>

/**
 * @brief Creates a payment strategy instance based on the specified type and details.
 *
 * This factory method constructs and returns a shared pointer to a PaymentStrategy
 * implementation according to the provided payment type and associated details.
 *
 * Supported payment types:
 * - "paypal": Requires 'email' field in details.
 * - "credit": Requires 'cardNumber', 'expirationDate', and 'cvv' fields in details.
 * - "cash": Does not require any details.
 *
 * @param type The type of payment strategy to create ("paypal", "credit", "cash").
 * @param details A JSON object containing the necessary fields for the selected payment type.
 * @return std::shared_ptr<PaymentStrategy> A shared pointer to the created payment strategy.
 *
 * @throws std::invalid_argument If required fields are missing, empty, or if the type is unknown.
 */
std::shared_ptr<PaymentStrategy> PaymentStrategyFactory::createPaymentStrategy(const std::string& type, const JSON& details) {
    if (type == "paypal") {
        if (!details.contains("email")) {
            throw std::invalid_argument("PayPal payment details require an 'email' field.");
        }
        auto email = details.at("email").get<std::string>();
        if (email.empty()) {
            throw std::invalid_argument("PayPal payment email cannot be empty.");
        }
        return std::make_shared<PaypalPayment>(email);
    } else if (type == "credit") {
        if (!details.contains("cardNumber") || !details.contains("expirationDate") || !details.contains("cvv")) {
            throw std::invalid_argument("Credit Card payment details require 'cardNumber', 'expirationDate', and 'cvv' fields.");
        }
        auto cardNumber = details.at("cardNumber").get<std::string>();
        auto expirationDate = details.at("expirationDate").get<std::string>();
        auto cvv = details.at("cvv").get<std::string>();

        if (cardNumber.empty() || expirationDate.empty() || cvv.empty()) {
            throw std::invalid_argument("Credit Card payment details cannot be empty.");
        }
        return std::make_shared<CreditPayment>(
            cardNumber,
            expirationDate,
            cvv
        );
    } else if (type == "cash") {
        // Cash payment does not require details
        return std::make_shared<CashPayment>();
    } else {
        throw std::invalid_argument("Unknown payment strategy type: " + type);
    }
}