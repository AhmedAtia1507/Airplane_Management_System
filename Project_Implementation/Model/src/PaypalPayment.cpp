#include "../include/PaypalPayment.hpp"
#include <stdexcept>


/**
 * @brief Constructs a PaypalPayment object with the specified PayPal email.
 *
 * Initializes the PaypalPayment instance using the provided email address.
 * Throws std::invalid_argument if the email is empty.
 *
 * @param email The PayPal email address to associate with the payment.
 * @throws std::invalid_argument If the provided email is empty.
 */
PaypalPayment::PaypalPayment(std::string email) : paypalEmail(std::move(email)) {
    if (paypalEmail.empty()) {
        throw std::invalid_argument("PayPal payment email cannot be empty.");
    }
    auto atPos = paypalEmail.find('@');
    if (atPos == std::string::npos) {
        throw std::invalid_argument("Invalid PayPal email format. Missing '@' symbol.");
    }
    auto domainString = paypalEmail.substr(atPos + 1);
    if (domainString.compare("paypal.com") != 0) {
        throw std::invalid_argument("Invalid PayPal email format. Domain part must be 'paypal.com'.");
    }
}

/**
 * @brief Processes a payment using PayPal.
 *
 * This function takes an amount and returns a confirmation message indicating
 * that the PayPal payment has been processed successfully.
 *
 * @param amount The amount to be processed via PayPal.
 * @return std::string Confirmation message of the processed payment.
 */
std::string PaypalPayment::processPayment(double amount) {
    return "PayPal payment of " + std::to_string(amount) + " using " + paypalEmail + " processed successfully.";
}
/**
 * @brief Refunds a PayPal payment for the specified amount.
 *
 * This method processes a refund for a PayPal payment and returns a confirmation message.
 *
 * @param amount The amount to refund.
 * @return A string message indicating the refund was successful.
 */
std::string PaypalPayment::refundPayment(double amount) {
    return "PayPal payment of " + std::to_string(amount) + " using " + paypalEmail + " refunded successfully.";
}

/**
 * @brief Retrieves the PayPal payment details as a JSON object.
 *
 * This method returns a JSON object containing the PayPal email address
 * associated with the payment.
 *
 * @return JSON object with the PayPal payment details.
 */
JSON PaypalPayment::getDetails() const {
    return JSON {
        {"email", paypalEmail}
    };
}