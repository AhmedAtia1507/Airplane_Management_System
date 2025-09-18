#include "../include/PaymentService.hpp"
#include "../../Repositories/include/PaymentRepository.hpp"
#include "../../Model/include/PaymentStrategyFactory.hpp"


/**
 * @brief Creates a new payment record and stores it in the repository.
 * 
 * This function creates a payment using the specified parameters by:
 * 1. Creating an appropriate payment strategy based on the payment method
 * 2. Instantiating a PaymentModel with the provided details
 * 3. Adding the payment to the repository
 * 4. Returning the stored payment if successful
 * 
 * @param passengerId The unique identifier of the passenger making the payment
 * @param amount The payment amount (must be positive)
 * @param method The payment method string (e.g., "credit_card", "paypal", etc.)
 * @param paymentDetails JSON object containing method-specific payment details
 * 
 * @return std::optional<std::shared_ptr<PaymentModel>> Returns a shared pointer to the 
 *         created payment if successful, or std::nullopt if:
 *         - Payment strategy creation fails
 *         - Payment model creation fails
 *         - Repository storage fails
 *         - Any exception occurs during the process
 * 
 * @throws No exceptions are thrown; all exceptions are caught and result in std::nullopt
 * 
 * @note This function uses the PaymentStrategyFactory to create appropriate payment strategies
 *       and the PaymentRepository singleton to persist the payment data.
 */
std::optional<std::shared_ptr<PaymentModel>> PaymentService::createPayment(const std::string& passengerId, float amount, 
            const std::string& method, const JSON& paymentDetails) {

    // Get the payment strategy based on the method
    try {
        auto paymentStrategy = PaymentStrategyFactory::createPaymentStrategy(method, paymentDetails);
        if (!paymentStrategy) {
            return std::nullopt;
        }
        auto payment = std::make_shared<PaymentModel>(passengerId, amount, paymentStrategy);
        if (!payment) {
            return std::nullopt;
        }
        
        if (!PaymentRepository::getInstance() -> addPayment(*payment)) {
            return std::nullopt;
        }
        return PaymentRepository::getInstance() -> findPaymentById(payment->getPaymentId());
    }
    catch (const std::exception& e) {
        // Handle invalid payment method or strategy creation failure
        return std::nullopt;
    }
    return std::nullopt;
}
/**
 * @brief Processes a payment with the given payment ID.
 *
 * This method attempts to find a payment by its ID using the PaymentRepository.
 * If the payment is found, it processes the payment and returns the result as a string.
 * If the payment is not found, it returns "Payment not found".
 *
 * @param paymentId The unique identifier of the payment to process.
 * @return std::string The result of the payment processing or an error message if not found.
 */
std::string PaymentService::processPayment(const std::string& paymentId) {
    auto paymentOpt = PaymentRepository::getInstance() -> findPaymentById(paymentId);
    if (!paymentOpt.has_value()) {
        return "Payment not found";
    }
    return paymentOpt.value() -> processPayment();
}
/**
 * @brief Refunds a payment with the specified payment ID.
 *
 * This method attempts to find a payment by its ID and, if found, processes a refund for that payment.
 * If the payment is not found, it returns an appropriate message.
 *
 * @param paymentId The unique identifier of the payment to be refunded.
 * @return A string indicating the result of the refund operation, such as success or error message.
 */
std::string PaymentService::refundPayment(const std::string& paymentId) {
    auto paymentOpt = PaymentRepository::getInstance() -> findPaymentById(paymentId);
    if (!paymentOpt.has_value()) {
        return "Payment not found";
    }
    return paymentOpt.value() -> refundPayment();
}