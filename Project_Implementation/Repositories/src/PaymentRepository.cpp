#include "../include/PaymentRepository.hpp"
#include "../../Utils/include/JSONManager.hpp"
#include "../../Utils/include/DatabasePathResolver.hpp"
/**
 * @brief Path to the payment database JSON file.
 *
 * This constant defines the relative file path to the payment database,
 * which is stored in JSON format and used for payment-related operations.
 */
const std::string PAYMENT_DATABASE_PATH = DatabasePathResolver::getDatabasePath() + "payments.json";

/**
 * @brief Constructs a PaymentRepository object and initializes the payments data.
 *
 * This constructor parses the payment data from the JSON file specified by
 * PAYMENT_DATABASE_PATH and populates the payments container.
 */
PaymentRepository::PaymentRepository() {
    JSONManager::parseJSON(payments, PAYMENT_DATABASE_PATH);
}

/**
 * @brief Returns a shared pointer to the singleton instance of PaymentRepository.
 *
 * This method ensures that only one instance of PaymentRepository exists throughout
 * the application's lifetime. The instance is created on first call and shared among
 * all callers using std::shared_ptr.
 *
 * @return std::shared_ptr<PaymentRepository> Singleton instance of PaymentRepository.
 */
std::shared_ptr<PaymentRepository> PaymentRepository::getInstance() {
    static std::shared_ptr<PaymentRepository> Instance(new PaymentRepository());
    return Instance;
}

/**
 * @brief Finds a payment by its unique identifier.
 *
 * This method ensures that only one instance of PaymentRepository exists throughout
 * the application's lifetime. The instance is created on first call and shared among
 * all callers using std::shared_ptr.
 *
 * @param paymentId The unique identifier of the payment to find.
 * @return std::optional<std::shared_ptr<PaymentModel>> Shared pointer to the PaymentModel if found, std::nullopt otherwise.
 */
std::optional<std::shared_ptr<PaymentModel>> PaymentRepository::findPaymentById(const std::string& paymentId) {
    if (payments.find(paymentId) == payments.end()) {
        return std::nullopt;
    }

    return payments.at(paymentId);
}

/**
 * @brief Adds a new payment to the repository.
 *
 * This method checks if a payment with the same ID already exists in the repository.
 * If it does, the method returns false and does not add the payment.
 * Otherwise, it adds the new payment and returns true.
 *
 * @param newPayment The PaymentModel object representing the payment to be added.
 * @return true if the payment was successfully added; false if a payment with the same ID already exists.
 */
bool PaymentRepository::addPayment(const PaymentModel& newPayment) {
    if (payments.find(newPayment.getPaymentId()) != payments.end()) {
        return false;
    }
    payments[newPayment.getPaymentId()] = std::make_shared<PaymentModel>(newPayment);
    return true;
}
/**
 * @brief Updates an existing payment in the repository.
 *
 * This function searches for a payment with the specified payment ID.
 * If the payment exists, it updates the payment information with the provided PaymentModel.
 * If the payment does not exist, the function returns false.
 *
 * @param payment The PaymentModel object containing updated payment information.
 * @return true if the payment was successfully updated; false if the payment does not exist.
 */
bool PaymentRepository::updatePayment(const PaymentModel& payment) {
    if (payments.find(payment.getPaymentId()) == payments.end()) {
        return false;
    }
    payments[payment.getPaymentId()] = std::make_shared<PaymentModel>(payment);
    return true;
}
/**
 * @brief Deletes a payment from the repository by its ID.
 *
 * This function attempts to remove the payment with the specified paymentId from the payments collection.
 * If the payment does not exist, the function returns false and no deletion occurs.
 * If the payment exists, it is erased from the collection and the function returns true.
 *
 * @param paymentId The unique identifier of the payment to be deleted.
 * @return true if the payment was successfully deleted; false if the payment does not exist.
 */
bool PaymentRepository::deletePayment(const std::string& paymentId) {
    if (payments.find(paymentId) != payments.end()) {
        return false;
    }
    payments.erase(paymentId);
    return true;
}

/**
 * @brief Destructor for the PaymentRepository class.
 *
 * Saves the current state of the payments collection to a JSON file
 * specified by PAYMENT_DATABASE_PATH using the JSONManager utility.
 * Ensures that all payment data is persisted before the repository
 * object is destroyed.
 */
PaymentRepository::~PaymentRepository() {
    JSONManager::saveToJSON(payments, PAYMENT_DATABASE_PATH);
    payments.clear();
}