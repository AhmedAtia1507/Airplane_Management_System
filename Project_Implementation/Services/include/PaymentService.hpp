#pragma once

#include <string>
#include <memory>
#include "../../Model/include/PaymentModel.hpp"
#include "../../Third_Party/json.hpp"

using JSON = nlohmann::json;


/**
 * @brief Service class for handling payment operations in the airline management system.
 * 
 * The PaymentService class provides static methods for managing payments including
 * creation, processing, and refunding. This class follows a static utility pattern
 * and cannot be instantiated.
 * 
 * @note This class is designed as a utility class with deleted default constructor
 *       to prevent instantiation.
 */

/**
 * @brief Creates a new payment record for a passenger.
 * 
 * Creates a payment entry in the system with the specified details including
 * passenger information, amount, payment method, and additional payment details.
 * 
 * @param passengerId The unique identifier of the passenger making the payment
 * @param amount The payment amount to be processed
 * @param method The payment method (e.g., "credit_card", "debit_card", "paypal")
 * @param paymentDetails Additional payment details in JSON format (card info, etc.)
 * 
 * @return std::optional<std::shared_ptr<PaymentModel>> Returns a shared pointer to the
 *         created PaymentModel if successful, std::nullopt if creation failed
 * 
 * @throws May throw exceptions related to invalid payment details or database errors
 */

/**
 * @brief Processes an existing payment by its ID.
 * 
 * Initiates the payment processing workflow for a previously created payment.
 * This involves communicating with payment gateways and updating payment status.
 * 
 * @param paymentId The unique identifier of the payment to process
 * 
 * @return std::string Returns the processing result status or confirmation message
 * 
 * @throws May throw exceptions for invalid payment ID or processing failures
 */

/**
 * @brief Refunds a processed payment by its ID.
 * 
 * Initiates a refund for a previously processed payment. This involves
 * reversing the payment transaction and updating the payment status.
 * 
 * @param paymentId The unique identifier of the payment to refund
 * 
 * @return std::string Returns the refund result status or confirmation message
 * 
 * @throws May throw exceptions for invalid payment ID, non-refundable payments,
 *         or refund processing failures
 */
class PaymentService {
    public:
        PaymentService() = delete;

        static std::optional<std::shared_ptr<PaymentModel>> createPayment(const std::string& passengerId, float amount, 
            const std::string& method, const JSON& paymentDetails);
        static std::string processPayment(const std::string& paymentId);
        static std::string refundPayment(const std::string& paymentId);
};