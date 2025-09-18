#include "../include/CreditPayment.hpp"
#include <stdexcept>

/**
 * @brief Constructs a CreditPayment object with the provided credit card details.
 *
 * Initializes the credit card number, expiration date, and CVV code.
 * Throws std::invalid_argument if any of the provided details are empty.
 *
 * @param number The credit card number as a string.
 * @param expiry The expiration date of the credit card as a string.
 * @param cvv_code The CVV code of the credit card as a string.
 *
 * @throws std::invalid_argument If any of the parameters are empty.
 */
CreditPayment::CreditPayment(std::string number, std::string expiry, std::string cvv_code)
    : creditCardNumber(std::move(number)), expirationDate(std::move(expiry)), cvv(std::move(cvv_code)) {
    if (creditCardNumber.empty() || expirationDate.empty() || cvv.empty()) {
        throw std::invalid_argument("Credit card details cannot be empty.");
    }
    if (creditCardNumber.size() != 16) {
        throw std::invalid_argument("Credit card number must be 16 digits.");
    }
    if (expirationDate.size() != 5 || expirationDate[2] != '/') {
        throw std::invalid_argument("Expiration date must be in MM/YY format.");
    }
    if (cvv.size() != 3) {
        throw std::invalid_argument("CVV must be 3 digits.");
    }
    for (char c : cvv) {
        if (!std::isdigit(c)) {
            throw std::invalid_argument("CVV must contain only digits.");
        }
    }
    for (char c : expirationDate) {
        if (!std::isdigit(c) && c != '/') {
            throw std::invalid_argument("Expiration date must contain only digits and '/'.");
        }
    }
    for (char c : creditCardNumber) {
        if (!std::isdigit(c)) {
            throw std::invalid_argument("Credit card number must contain only digits.");
        }
    }
}
std::string CreditPayment::maskCardNumber() const {
    std::string masked = "****-****-****-";
    masked += creditCardNumber.substr(creditCardNumber.length() - 4);
    return masked;
}
/**
 * @brief Processes a credit card payment for the specified amount.
 *
 * This method generates a confirmation message indicating that a credit card payment
 * has been processed successfully using the associated credit card number.
 *
 * @param amount The amount to be paid using the credit card.
 * @return A string message confirming the successful processing of the payment.
 */
std::string CreditPayment::processPayment(double amount) {
    return "Credit card payment of " + std::to_string(amount) + " using credit card number " + maskCardNumber() + " processed successfully.";
}
/**
 * @brief Refunds a payment made via credit card.
 *
 * This method processes the refund for a specified amount and returns a confirmation message.
 *
 * @param amount The amount to be refunded.
 * @return A string message confirming the successful refund using the credit card.
 */
std::string CreditPayment::refundPayment(double amount) {
    return "Credit card refund of " + std::to_string(amount) + " to credit card number " + maskCardNumber() + " processed successfully.";
}

/**
 * @brief Returns the type of payment as a string.
 *
 * @return A string representing the payment type, which is "credit" for this class.
 */
std::string CreditPayment::getType() const {
    return "credit";
}

/**
 * @brief Retrieves the credit payment details as a JSON object.
 *
 * This method returns a JSON object containing the credit card number,
 * expiration date, and CVV associated with the credit payment.
 *
 * @return JSON object with the following fields:
 *         - "cardNumber": The credit card number.
 *         - "expirationDate": The expiration date of the credit card.
 *         - "cvv": The card verification value (CVV).
 */
JSON CreditPayment::getDetails() const {
    return JSON {
        {"cardNumber", creditCardNumber},
        {"expirationDate", expirationDate},
        {"cvv", cvv}
    };
}