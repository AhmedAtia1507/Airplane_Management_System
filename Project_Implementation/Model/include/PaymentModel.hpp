#pragma once

#include <string>
#include <memory>
#include "PaymentStrategy.hpp"
#include "../../Third_Party/json.hpp"
#include "../../Utils/include/DateTime.hpp"

using JSON = nlohmann::json;

/**
 * @class PaymentModel
 * @brief Represents a payment transaction within the system.
 *
 * The PaymentModel class encapsulates details about a payment, including its unique identifier,
 * associated passenger, amount, payment method, status, and date. It supports processing and refunding
 * payments using a strategy pattern for extensible payment methods.
 *
 * @enum PaymentStatus
 *      Indicates the current status of the payment (COMPLETED, PENDING, REFUNDED).
 *
 * @var paymentId
 *      Unique identifier for the payment.
 * @var passengerId
 *      Identifier of the passenger associated with the payment.
 * @var amount
 *      Amount of the payment.
 * @var paymentStrategy
 *      Strategy used to process the payment.
 * @var paymentDate
 *      Date and time when the payment was made.
 * @var status
 *      Current status of the payment.
 *
 * @var status
 *      Status of the payment (COMPLETED, PENDING, REFUNDED).
 *
 * @constructor PaymentModel()
 *      Default constructor.
 * @constructor PaymentModel(const std::string&, double, const std::shared_ptr<PaymentStrategy>&, const PaymentStatus&, const DateTime&)
 *      Constructs a PaymentModel with specified passenger ID, amount, payment strategy, status, and date.
 * @constructor PaymentModel(const JSON&)
 *      Constructs a PaymentModel from a JSON object.
 *
 * @method getPaymentId
 *      Returns the payment's unique identifier.
 * @method getPassengerId
 *      Returns the passenger's identifier.
 * @method getAmount
 *      Returns the payment amount.
 * @method getPaymentStrategy
 *      Returns the payment strategy.
 * @method getPaymentDate
 *      Returns the payment date.
 *
 * @method setPaymentId
 *      Sets the payment's unique identifier.
 * @method setPassengerId
 *      Sets the passenger's identifier.
 * @method setAmount
 *      Sets the payment amount.
 * @method setPaymentStrategy
 *      Sets the payment strategy.
 * @method setPaymentDate
 *      Sets the payment date.
 *
 * @method processPayment
 *      Processes the payment using the assigned strategy.
 * @method refundPayment
 *      Refunds the payment.
 *
 * @method to_json
 *      Serializes the PaymentModel to a JSON object.
 * 
 * @destructor ~PaymentModel()
 *      Default destructor.
 */
class PaymentModel {
    public:
        enum class PaymentStatus {
            COMPLETED,
            PENDING,
            REFUNDED
        };
    private:
        std::string paymentId;
        std::string passengerId;
        double amount;
        std::shared_ptr<PaymentStrategy> paymentStrategy;
        DateTime paymentDate;
        PaymentStatus status;
    public:
        PaymentModel() = default;
        PaymentModel(const std::string& passengerId, double amount, 
            const std::shared_ptr<PaymentStrategy>& strategy, const PaymentStatus& status = PaymentStatus::PENDING, const DateTime& paymentDate = DateTime::now());
        PaymentModel(const JSON& json);

        std::string getPaymentId() const                                            { return paymentId; }
        std::string getPassengerId() const                                          { return passengerId; }
        double getAmount() const                                                    { return amount; }
        std::shared_ptr<PaymentStrategy> getPaymentStrategy() const                 { return paymentStrategy; }
        DateTime getPaymentDate() const                                             { return paymentDate; }

        void setPaymentId(const std::string& id)                                    { paymentId = id; }
        void setPassengerId(const std::string& id)                                  { passengerId = id; }
        void setAmount(double amt)                                                  { amount = amt; }
        void setPaymentStrategy(const std::shared_ptr<PaymentStrategy>& strategy)   { paymentStrategy = strategy; }
        void setPaymentDate(const DateTime& date)                                   { paymentDate = date; }

        std::string processPayment();
        std::string refundPayment();
    
        void to_json(JSON& json) const;

        ~PaymentModel() = default;
};