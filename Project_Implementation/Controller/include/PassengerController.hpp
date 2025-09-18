#pragma once

#include <memory>
#include <optional>
#include <vector>
#include "../../Model/include/UserModel.hpp"
#include "../../Model/include/FlightModel.hpp"
#include "../../Model/include/ReservationModel.hpp"

/**
 * @brief Controller class for managing passenger operations in the airline management system.
 * 
 * The PassengerController class provides static methods for handling passenger-related
 * operations such as flight searching, booking, payment processing, and reservation management.
 * All operations require passenger authentication through the passengerId parameter.
 * 
 * This class follows a static-only design pattern and cannot be instantiated.
 * 
 * @note All public methods perform passenger authentication before executing operations.
 * @note This class is part of the MVC architecture serving as the controller layer.
 */
class PassengerController {
    static bool authenticatePassenger(const std::string& passengerId);
public:
    PassengerController() = delete;
    static std::vector<std::shared_ptr<FlightModel>> getAllFlights(const std::string& passengerId);
    static std::optional<std::shared_ptr<FlightModel>> getFlightDetails(
        const std::string& passengerId, 
        const std::string& flightId
    );
    static std::vector<std::shared_ptr<FlightModel>> getFlightsByRouteAndDate(
        const std::string& passengerId, 
        const std::string& origin, 
        const std::string& destination, 
        const DateTime& departureDate
    );
    static std::optional<std::shared_ptr<ReservationModel>> bookFlight(
        const std::string& passengerId, 
        const std::string& flightId,
        const std::string& seatNumber,
        const std::string& paymentType,
        const JSON& paymentDetails
    );
    static std::string processPayment(const std::string& passengerId, const std::string& paymentId);
    static std::vector<std::shared_ptr<ReservationModel>> getPassengerReservations(const std::string& passengerId);
};