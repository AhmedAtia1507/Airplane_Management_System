#include "../include/CashPayment.hpp"

/**
 * @brief Processes a cash payment for the specified amount.
 *
 * This method handles the logic for processing a cash payment and returns
 * a confirmation message indicating the payment was successful.
 *
 * @param amount The amount to be paid in cash.
 * @return A string message confirming the successful processing of the cash payment.
 */
std::string CashPayment::processPayment(double amount) {
    return "Cash payment of " + std::to_string(amount) + " processed successfully.";
}
/**
 * @brief Processes a refund for a cash payment.
 *
 * This method generates a confirmation message indicating that a specified amount
 * has been refunded successfully via cash payment.
 *
 * @param amount The amount to be refunded.
 * @return A string message confirming the successful refund of the specified amount.
 */
std::string CashPayment::refundPayment(double amount) {
    return "Cash payment of " + std::to_string(amount) + " refunded successfully.";
}
/**
 * @brief Returns the type of payment as a string.
 * 
 * @return std::string The string "cash" representing the payment type.
 */
std::string CashPayment::getType() const { 
    return "cash"; 
}
/**
 * @brief Retrieves the details of the cash payment.
 *
 * This method returns a JSON object containing the details of the cash payment.
 * For cash payments, there are typically no specific details to provide, so an empty JSON object is returned.
 *
 * @return JSON An empty JSON object representing the cash payment details.
 */
JSON CashPayment::getDetails() const {
    return JSON(); // Assuming no specific details for cash payment
}