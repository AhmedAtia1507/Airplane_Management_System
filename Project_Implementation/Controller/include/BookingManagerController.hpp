#pragma once

#include <optional>
#include <memory>
#include "../../Model/include/ReservationModel.hpp"
#include "../../Model/include/FlightModel.hpp"
#include "../../Model/include/UserModel.hpp"
#include <string>
#include <vector>

/**
 * @class BookingManagerController
 * @brief Controller class for managing booking operations and flight reservations
 * 
 * This class provides static methods for booking managers to perform various operations
 * including flight management, passenger management, reservation handling, and payment processing.
 * All public methods require authentication through a booking manager ID.
 */

/**
 * @brief Authenticates a booking manager using their ID
 * @param bookingManagerId The unique identifier of the booking manager
 * @return True if authentication is successful, false otherwise
 */

/**
 * @brief Retrieves all available flights for an authenticated booking manager
 * @param bookingManagerId The unique identifier of the booking manager
 * @return Vector of shared pointers to FlightModel objects representing all flights
 */

/**
 * @brief Retrieves flights filtered by route and departure date
 * @param bookingManagerId The unique identifier of the booking manager
 * @param origin The departure location/airport code
 * @param destination The arrival location/airport code
 * @param departureDate The date of departure
 * @return Vector of shared pointers to FlightModel objects matching the criteria
 */

/**
 * @brief Retrieves all passengers in the system
 * @param bookingManagerId The unique identifier of the booking manager
 * @return Vector of shared pointers to UserModel objects representing all passengers
 */

/**
 * @brief Retrieves all reservations in the system
 * @param bookingManagerId The unique identifier of the booking manager
 * @return Vector of shared pointers to ReservationModel objects representing all reservations
 */

/**
 * @brief Retrieves detailed information about a specific passenger
 * @param bookingManagerId The unique identifier of the booking manager
 * @param passengerId The unique identifier of the passenger
 * @return Optional shared pointer to UserModel object, empty if passenger not found
 */

/**
 * @brief Retrieves detailed information about a specific reservation
 * @param bookingManagerId The unique identifier of the booking manager
 * @param reservationId The unique identifier of the reservation
 * @return Optional shared pointer to ReservationModel object, empty if reservation not found
 */

/**
 * @brief Retrieves detailed information about a specific flight
 * @param bookingManagerId The unique identifier of the booking manager
 * @param flightId The unique identifier of the flight
 * @return Optional shared pointer to FlightModel object, empty if flight not found
 */

/**
 * @brief Creates a new reservation for a passenger on a specific flight
 * @param bookingManagerId The unique identifier of the booking manager
 * @param passengerId The unique identifier of the passenger
 * @param flightId The unique identifier of the flight
 * @param seatNumber The assigned seat number for the reservation
 * @param paymentType The type of payment method used
 * @param paymentDetails JSON object containing payment-specific details
 * @return Optional shared pointer to the created ReservationModel object, empty if creation failed
 */

/**
 * @brief Updates an existing reservation with new information
 * @param bookingManagerId The unique identifier of the booking manager
 * @param reservation The ReservationModel object containing updated information
 * @return True if the update was successful, false otherwise
 */

/**
 * @brief Cancels an existing reservation
 * @param bookingManagerId The unique identifier of the booking manager
 * @param reservationId The unique identifier of the reservation to cancel
 * @return True if cancellation was successful, false otherwise
 */

/**
 * @brief Processes a payment transaction
 * @param bookingManagerId The unique identifier of the booking manager
 * @param paymentId The unique identifier of the payment to process
 * @return String indicating the payment processing result or status
 */

/**
 * @brief Processes a refund for a payment transaction
 * @param bookingManagerId The unique identifier of the booking manager
 * @param paymentId The unique identifier of the payment to refund
 * @return String indicating the refund processing result or status
 */
class BookingManagerController {
        static bool authenticateBookingManager(const std::string& bookingManagerId);
    public:
        static std::vector<std::shared_ptr<FlightModel>> getAllFlights(const std::string& bookingManagerId);
        static std::vector<std::shared_ptr<FlightModel>> getFlightsByRouteAndDate(
            const std::string& bookingManagerId, 
            const std::string& origin, 
            const std::string& destination, 
            const DateTime& departureDate
        );
        static std::vector<std::shared_ptr<UserModel>> getAllPassengers(const std::string& bookingManagerId);
        static std::vector<std::shared_ptr<ReservationModel>> getAllReservations(const std::string& bookingManagerId);

        static std::optional<std::shared_ptr<UserModel>> getPassengerDetails(const std::string& bookingManagerId, const std::string& passengerId);
        static std::optional<std::shared_ptr<ReservationModel>> getReservationDetails(const std::string& bookingManagerId, const std::string& reservationId);
        static std::optional<std::shared_ptr<FlightModel>> getFlightDetails(const std::string& bookingManagerId, const std::string& flightId);
        
        static std::optional<std::shared_ptr<ReservationModel>> createReservation (
            const std::string& bookingManagerId,
            const std::string& passengerId,
            const std::string& flightId,
            const std::string& seatNumber,
            const std::string& paymentType,
            const JSON& paymentDetails
        );
        static bool updateReservation(const std::string& bookingManagerId, const ReservationModel& reservation);
        static bool cancelReservation(const std::string& bookingManagerId, const std::string& reservationId);
        static std::string processPayment(const std::string& bookingManagerId, const std::string& paymentId);
        static std::string refundPayment(const std::string& bookingManagerId, const std::string& paymentId);
};