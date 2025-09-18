#pragma once

#include <string>
#include "../../Third_Party/json.hpp"
using JSON = nlohmann::json;

/**
 * @brief Abstract base class for payment strategies.
 *
 * Defines the interface for processing and refunding payments,
 * retrieving the payment type, and obtaining payment details.
 * Concrete payment strategy implementations should inherit from this class.
 */
class PaymentStrategy {
    public:
    virtual std::string processPayment(double amount) = 0;
    virtual std::string refundPayment(double amount) = 0;
    virtual std::string getType() const = 0;
    virtual JSON getDetails() const = 0;
    virtual ~PaymentStrategy() = default;
};