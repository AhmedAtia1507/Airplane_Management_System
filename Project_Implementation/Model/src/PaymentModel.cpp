#include "../include/PaymentModel.hpp"
#include "../../Repositories/include/PaymentRepository.hpp"
#include "../../Repositories/include/UserRepository.hpp"
#include "../../Utils/include/IDGenerator.hpp"
#include "../include/PaymentStrategyFactory.hpp"
#include <stdexcept>

/**
 * @brief Constructs a PaymentModel object with the specified details.
 *
 * This constructor initializes a payment record for a passenger, validating the input parameters,
 * generating a unique payment ID, and setting the payment date. If the provided payment date is invalid,
 * the current date and time are used.
 *
 * @param passengerId The unique identifier of the passenger making the payment.
 * @param amount The amount to be paid. Must be greater than zero.
 * @param strategy The payment strategy to be used (e.g., credit card, cash).
 * @param status The status of the payment (e.g., pending, completed).
 * @param paymentDate The date and time of the payment. If invalid, the current date and time is used.
 *
 * @throws std::invalid_argument If any of the following conditions are met:
 *         - passengerId is empty
 *         - amount is less than or equal to zero
 *         - strategy is null
 *         - passengerId does not exist in the user repository
 */
PaymentModel::PaymentModel(const std::string& passengerId, double amount, 
        const std::shared_ptr<PaymentStrategy>& strategy, const PaymentStatus& status, const DateTime& paymentDate) {
    if (passengerId.empty() || amount <= 0 || !strategy) {
        throw std::invalid_argument("Invalid payment details provided.");
    }
    if (!UserRepository::getInstance() -> findUserById(passengerId).has_value()) {
        throw std::invalid_argument("Passenger ID does not exist.");
    }

    auto paymentRepository = PaymentRepository::getInstance();
    std::string id = "PAY-" + IDGenerator::generateUniqueID();

    while ( paymentRepository -> findPaymentById(id).has_value() ) {
        id = "PAY-" + IDGenerator::generateUniqueID();
    }

    paymentId = id;
    this -> passengerId = passengerId;
    this -> amount = amount;
    this -> paymentStrategy = strategy;
    this -> status = status;
    if (paymentDate.isValid()) {
        this->paymentDate = paymentDate;
    } else {
        this->paymentDate = DateTime::now();
    }
}
        
/**
 * @brief Constructs a PaymentModel object from a JSON representation.
 *
 * This constructor initializes a PaymentModel instance using the provided JSON object.
 * It validates the presence of required fields, checks the format of the payment ID,
 * verifies the existence of the passenger ID, ensures the payment amount is positive,
 * creates the appropriate payment strategy, validates the payment date, and sets the payment status.
 *
 * @param json The JSON object containing payment information.
 *
 * @throws std::invalid_argument If any required field is missing, the payment ID format is invalid,
 *         the passenger ID does not exist, the amount is not greater than zero, the payment date is invalid,
 *         or the payment status is not recognized.
 */
PaymentModel::PaymentModel(const JSON& json) {
    std::vector<std::string> requiredTags = {"id", "passengerId", "amount", "method", "paymentDate", "status"};
    for ( const auto& tag : requiredTags ) {
        if (!json.contains(tag)) {
            throw std::invalid_argument("Invalid JSON for PaymentModel: missing tag '" + tag + "'.");
        }
    }
    
    paymentId = json.at("id").get<std::string>();
    if (paymentId.substr(0, 4) != "PAY-") {
        throw std::invalid_argument("Invalid ID for PaymentModel");
    }
    passengerId = json.at("passengerId").get<std::string>();
    if (!UserRepository::getInstance() -> findUserById(passengerId).has_value()) {
        throw std::invalid_argument("Passenger ID does not exist.");
    }
    amount = json.at("amount").get<double>();
    if (amount <= 0) {
        throw std::invalid_argument("Amount must be greater than zero.");
    }

    std::string strategyType = json.at("method").get<std::string>();
    paymentStrategy = PaymentStrategyFactory::createPaymentStrategy(strategyType, json.at("details"));

    paymentDate = DateTime(json.at("paymentDate").get<std::string>());
    if (!paymentDate.isValid()) {
        throw std::invalid_argument("Invalid payment date provided.");
    }
    std::string statusStr = json.at("status").get<std::string>();
    if (statusStr == "COMPLETED") {
        status = PaymentStatus::COMPLETED;
    } else if (statusStr == "REFUNDED") {
        status = PaymentStatus::REFUNDED;
    } else {
        throw std::invalid_argument("Invalid payment status provided.");
    }
}

/**
 * @brief Processes the payment using the assigned payment strategy.
 *
 * Sets the payment status to COMPLETED and delegates the payment processing
 * to the current payment strategy with the specified amount.
 *
 * @return A string representing the result of the payment processing.
 */
std::string PaymentModel::processPayment() {
    this -> status = PaymentStatus::COMPLETED;
    return paymentStrategy -> processPayment(amount);
}
/**
 * @brief Refunds the current payment.
 *
 * Sets the payment status to REFUNDED and delegates the refund operation
 * to the associated payment strategy, passing the payment amount.
 *
 * @return A string indicating the result of the refund operation.
 */
std::string PaymentModel::refundPayment() {
    this -> status = PaymentStatus::REFUNDED;
    return paymentStrategy -> refundPayment(amount);
}


/**
 * @brief Serializes the PaymentModel object to a JSON representation.
 *
 * This method assigns a new value to the provided JSON reference, containing the payment's details,
 * including its ID, passenger ID, amount, payment method, payment date, and status.
 * If a payment strategy is set, its type and details are included; otherwise,
 * the method is set to "unknown".
 *
 * @param json Reference to a JSON object that will be assigned the payment data.
 */
void PaymentModel::to_json(JSON& json) const {
    json = JSON {
        {"id", paymentId},
        {"passengerId", passengerId},
        {"amount", amount},
        {"method", paymentStrategy ? paymentStrategy -> getType() : "UNKNOWN"},
        {"paymentDate", paymentDate.toString()},
        {"details", paymentStrategy ? paymentStrategy -> getDetails() : JSON{}}
    };

    switch (status) {
        case PaymentStatus::COMPLETED:
            json["status"] = "COMPLETED"; break;
        case PaymentStatus::REFUNDED:
            json["status"] = "REFUNDED"; break;
        default:
            json["status"] = "UNKNOWN"; break;
    }
}