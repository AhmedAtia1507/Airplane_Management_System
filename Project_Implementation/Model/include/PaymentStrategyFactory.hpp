#pragma once

#include <memory>
#include "PaymentStrategy.hpp"
#include "../../Third_Party/json.hpp"
#include "PaypalPayment.hpp"
#include "CreditPayment.hpp"
#include "CashPayment.hpp"

using JSON = nlohmann::json;

/**
 * @class PaymentStrategyFactory
 * @brief Factory class for creating payment strategy objects.
 *
 * This class provides a static method to instantiate different types of payment strategies
 * based on the provided type string and optional details in JSON format.
 */
class PaymentStrategyFactory {
    public:
        PaymentStrategyFactory() = delete;
        static std::shared_ptr<PaymentStrategy> createPaymentStrategy(const std::string& type, const JSON& details = {});
};