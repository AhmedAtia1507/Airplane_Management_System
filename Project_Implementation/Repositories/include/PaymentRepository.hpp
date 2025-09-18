#pragma once

#include "../../Model/include/PaymentModel.hpp"
#include <unordered_map>
#include <memory>

/**
 * @class PaymentRepository
 * @brief Singleton repository for managing PaymentModel objects.
 *
 * This class provides thread-unsafe CRUD operations for PaymentModel instances,
 * stored in an unordered map keyed by payment ID. It enforces singleton access,
 * disallows copy and move semantics, and exposes methods to add, update, find,
 * and delete payments.
 *
 * Usage:
 *   auto repo = PaymentRepository::getInstance();
 *
 * Methods:
 *   - findPaymentById: Retrieve a payment by its ID.
 *   - addPayment: Add a new payment.
 *   - updatePayment: Update an existing payment.
 *   - deletePayment: Remove a payment by its ID.
 */
class PaymentRepository {
    std::unordered_map<std::string, std::shared_ptr<PaymentModel>> payments;

    PaymentRepository();
    PaymentRepository(const PaymentRepository&) = delete;
    PaymentRepository& operator=(const PaymentRepository&) = delete;
    PaymentRepository(PaymentRepository&&) = delete;
    PaymentRepository& operator=(PaymentRepository&&) = delete;


    public:
        static std::shared_ptr<PaymentRepository> getInstance();
        std::optional<std::shared_ptr<PaymentModel>> findPaymentById(const std::string& paymentId);
        bool addPayment(const PaymentModel& newPayment);
        bool updatePayment(const PaymentModel& payment);
        bool deletePayment(const std::string& paymentId);

        ~PaymentRepository();
};
