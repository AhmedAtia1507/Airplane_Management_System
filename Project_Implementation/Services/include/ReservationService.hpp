#pragma once

#include <string>
#include <optional>
#include <memory>
#include "../../Model/include/ReservationModel.hpp"
#include "../../Model/include/PaymentModel.hpp"
#include "../../Third_Party/json.hpp"
#include <vector>

using JSON = nlohmann::json;



/**
 * @brief Service class for managing flight reservations in the airplane management system.
 * 
 * The ReservationService class provides static methods for performing CRUD operations
 * on flight reservations. It handles reservation creation, retrieval, updates, and
 * deletions, along with seat pricing calculations based on loyalty points.
 * 
 * This class follows a static service pattern and cannot be instantiated.
 * All operations are performed through static methods that interact with the
 * underlying reservation data storage.
 * 
 * @note This class is designed as a utility class and cannot be instantiated.
 *       All constructors are deleted to enforce static-only usage.
 */
class ReservationService {
        static float getSeatPrice(const std::string& seatNumber, float loyaltyPoints);
    public:
        ReservationService() = delete;

        static std::vector<std::shared_ptr<ReservationModel>> getAllReservations();
        static std::optional<std::shared_ptr<ReservationModel>> getReservationById(const std::string& reservationId);
        static std::vector<std::shared_ptr<ReservationModel>> getReservationByUserId(const std::string& userId);

        static std::optional<std::shared_ptr<ReservationModel>> addReservation(
            const std::string& flightId,
            const std::string& seatNumber,
            const std::string& passengerId,
            const std::string& paymentMethod,
            const JSON& paymentDetails
        );
        static bool updateReservation(const ReservationModel& reservation);
        static bool deleteReservation(const std::string& reservationId);
};