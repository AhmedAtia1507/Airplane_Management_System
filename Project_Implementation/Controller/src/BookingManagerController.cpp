#include "../include/BookingManagerController.hpp"
#include "../../Services/include/FlightService.hpp"
#include "../../Services/include/ReservationService.hpp"
#include "../../Services/include/PaymentService.hpp"
#include "../../Services/include/UserManagementService.hpp"
/**
 * @brief Authenticates whether the given user ID belongs to a Booking Manager.
 *
 * This function checks the role of the user associated with the provided bookingManagerId.
 * If the user's role is BookingManager, authentication succeeds.
 *
 * @param bookingManagerId The unique identifier of the user to authenticate.
 * @return true if the user is a Booking Manager, false otherwise.
 */
bool BookingManagerController::authenticateBookingManager(const std::string& bookingManagerId) {
    if (UserManagementService::getUserRole(bookingManagerId) == UserModel::UserType::BookingManager) {
        return true;
    }
    return false;
}
/**
 * @brief Retrieves all passengers from the system for a booking manager.
 * 
 * This method allows an authenticated booking manager to access a list of all 
 * passengers in the system. The method first validates the booking manager's 
 * authentication before proceeding with the data retrieval.
 * 
 * @param bookingManagerId The unique identifier of the booking manager requesting 
 *                        the passenger list. Used for authentication purposes.
 * 
 * @return std::vector<std::shared_ptr<UserModel>> A vector containing shared 
 *         pointers to all passenger user models in the system. Returns an empty 
 *         vector if authentication fails or no passengers are found.
 * 
 * @throws May throw exceptions from underlying authentication or user service calls.
 * 
 * @note Only authenticated booking managers can access this functionality.
 * @see authenticateBookingManager()
 * @see UserManagementService::getUsersByRole()
 */
std::vector<std::shared_ptr<ReservationModel>> BookingManagerController::getAllReservations(const std::string& bookingManagerId) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return {};
    }
    return ReservationService::getAllReservations();
}
/**
 * @brief Retrieves the details of a reservation for a given booking manager.
 *
 * This function authenticates the booking manager using the provided bookingManagerId.
 * If authentication is successful, it fetches the reservation details corresponding to the reservationId.
 * If authentication fails, it returns std::nullopt.
 *
 * @param bookingManagerId The unique identifier of the booking manager.
 * @param reservationId The unique identifier of the reservation.
 * @return std::optional<std::shared_ptr<ReservationModel>> An optional containing a shared pointer to the ReservationModel if found and authenticated;
 *         std::nullopt otherwise.
 */
std::optional<std::shared_ptr<ReservationModel>> BookingManagerController::getReservationDetails(const std::string& bookingManagerId, const std::string& reservationId) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return std::nullopt;
    }
    return ReservationService::getReservationById(reservationId);
} 
/**
 * @brief Creates a new reservation for a passenger on a specified flight.
 *
 * This method attempts to create a reservation using the provided booking manager ID, passenger ID, flight ID,
 * seat number, payment type, and payment details. If the reservation is successfully created,
 * it returns an optional containing a shared pointer to the ReservationModel. Otherwise, it returns
 * an empty optional.
 *
 * @param bookingManagerId The unique identifier of the booking manager creating the reservation.
 * @param passengerId The unique identifier of the passenger making the reservation.
 * @param flightId The unique identifier of the flight to reserve a seat on.
 * @param seatNumber The seat number to be reserved.
 * @param paymentType The type of payment method used (e.g., credit card, PayPal).
 * @param paymentDetails A JSON object containing payment-specific details. 
 *        // NOTE: The type 'JSON' should be defined elsewhere in your project, e.g., using nlohmann::json or similar.
 * @return std::optional<std::shared_ptr<ReservationModel>> An optional containing the created reservation model if successful, or empty if not.
 */
std::optional<std::shared_ptr<ReservationModel>> BookingManagerController::createReservation (
    const std::string& bookingManagerId,
    const std::string& passengerId,
    const std::string& flightId,
    const std::string& seatNumber,
    const std::string& paymentType,
    const JSON& paymentDetails
) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return std::nullopt;
    }
    return ReservationService::addReservation (
        flightId,
        seatNumber,
        passengerId,
        paymentType,
        paymentDetails
    );
}
/**
 * @brief Updates an existing reservation if the booking manager is authenticated.
 *
 * This method verifies the authentication of the booking manager using the provided
 * bookingManagerId. If authentication succeeds, it delegates the update operation
 * to the ReservationService. Returns true if the update is successful, false otherwise.
 *
 * @param bookingManagerId The unique identifier of the booking manager attempting the update.
 * @param reservation The ReservationModel object containing updated reservation details.
 * @return true if the reservation was successfully updated; false if authentication fails or the update is unsuccessful.
 */
bool BookingManagerController::updateReservation(const std::string& bookingManagerId, const ReservationModel& reservation) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return false;
    }
    return ReservationService::updateReservation(reservation);
}
/**
 * @brief Cancels a reservation for a booking manager.
 *
 * This method attempts to cancel a reservation identified by `reservationId` for the booking manager
 * specified by `bookingManagerId`. The booking manager must be authenticated before the cancellation
 * is processed. If authentication fails, the reservation will not be cancelled.
 *
 * @param bookingManagerId The unique identifier of the booking manager requesting the cancellation.
 * @param reservationId The unique identifier of the reservation to be cancelled.
 * @return true if the reservation was successfully cancelled; false otherwise.
 */
bool BookingManagerController::cancelReservation(const std::string& bookingManagerId, const std::string& reservationId) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return false;
    }
    return ReservationService::deleteReservation(reservationId);
}
/**
 * @brief Processes a payment for a booking manager.
 *
 * Authenticates the booking manager using the provided ID. If authentication
 * fails, returns "Unauthorized". Otherwise, delegates payment processing to
 * the PaymentService using the given payment ID.
 *
 * @param bookingManagerId The unique identifier of the booking manager.
 * @param paymentId The unique identifier of the payment to be processed.
 * @return std::string The result of the payment processing, or "Unauthorized" if authentication fails.
 */
std::string BookingManagerController::processPayment(const std::string& bookingManagerId, const std::string& paymentId) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return "Unauthorized";
    }
    return PaymentService::processPayment(paymentId);
}
/**
 * @brief Refunds a payment associated with a booking manager.
 *
 * This method authenticates the booking manager using the provided bookingManagerId.
 * If authentication fails, it returns "Unauthorized". Otherwise, it initiates a refund
 * for the payment identified by paymentId via the PaymentService.
 *
 * @param bookingManagerId The unique identifier of the booking manager requesting the refund.
 * @param paymentId The unique identifier of the payment to be refunded.
 * @return A string indicating the result of the refund operation ("Unauthorized" or the result from PaymentService).
 */
std::string BookingManagerController::refundPayment(const std::string& bookingManagerId, const std::string& paymentId) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return "Unauthorized";
    }
    return PaymentService::refundPayment(paymentId);
}
/**
 * @brief Retrieves all flights for a booking manager.
 *
 * This method authenticates the booking manager using the provided bookingManagerId.
 * If authentication is successful, it retrieves and returns a list of all available flights.
 * If authentication fails, it returns an empty vector.
 *
 * @param bookingManagerId The unique identifier of the booking manager requesting the flight list.
 * @return std::vector<std::shared_ptr<FlightModel>> A vector containing shared pointers to FlightModel objects if authenticated; 
 *         an empty vector otherwise.
 */
std::vector<std::shared_ptr<FlightModel>> BookingManagerController::getAllFlights(const std::string& bookingManagerId) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return {};
    }
    return FlightService::getAllFlights();
}
/**
 * @brief Retrieves flights matching the specified route and departure date for an authenticated booking manager.
 * 
 * This method first authenticates the booking manager using their ID, then retrieves all flights
 * that match the given origin, destination, and departure date criteria.
 * 
 * @param bookingManagerId The unique identifier of the booking manager requesting the flight information
 * @param origin The departure location/airport code for the flight search
 * @param destination The arrival location/airport code for the flight search
 * @param departureDate The specific date for which flights are being searched
 * 
 * @return A vector of shared pointers to FlightModel objects representing matching flights.
 *         Returns an empty vector if authentication fails or no flights match the criteria.
 * 
 * @note Authentication is required - the method returns an empty result if the booking manager
 *       cannot be authenticated.
 */
std::vector<std::shared_ptr<FlightModel>> BookingManagerController::getFlightsByRouteAndDate(
            const std::string& bookingManagerId, 
            const std::string& origin, 
            const std::string& destination, 
            const DateTime& departureDate
) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return {};
    }
    return FlightService::getFlightsByRouteAndDate(origin, destination, departureDate);
}
/**
 * @brief Retrieves all passengers from the system for a booking manager.
 * 
 * This method allows an authenticated booking manager to access a list of all 
 * passengers in the system. The method first validates the booking manager's 
 * authentication before proceeding with the data retrieval.
 * 
 * @param bookingManagerId The unique identifier of the booking manager requesting 
 *                        the passenger list. Used for authentication purposes.
 * 
 * @return std::vector<std::shared_ptr<UserModel>> A vector containing shared 
 *         pointers to all passenger user models in the system. Returns an empty 
 *         vector if authentication fails or no passengers are found.
 * 
 * @throws May throw exceptions from underlying authentication or user service calls.
 * 
 * @note Only authenticated booking managers can access this functionality.
 * @see authenticateBookingManager()
 * @see UserManagementService::getUsersByRole()
 */
std::vector<std::shared_ptr<UserModel>> BookingManagerController::getAllPassengers(const std::string& bookingManagerId) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return {};
    }
    return UserManagementService::getUsersByRole(UserModel::UserType::Passenger);
}

/**
 * @brief Retrieves passenger details for a specific passenger ID.
 * 
 * This method allows an authenticated booking manager to access passenger information
 * by their unique identifier. The method first validates the booking manager's 
 * authentication before proceeding with the passenger data retrieval.
 * 
 * @param bookingManagerId The unique identifier of the booking manager requesting the information
 * @param passengerId The unique identifier of the passenger whose details are being requested
 * 
 * @return std::optional<std::shared_ptr<UserModel>> Returns a shared pointer to the UserModel 
 *         containing passenger details if the booking manager is authenticated and the passenger 
 *         is found, otherwise returns std::nullopt
 * 
 * @note The method returns std::nullopt if:
 *       - The booking manager authentication fails
 *       - The passenger with the given ID is not found
 * 
 * @see authenticateBookingManager()
 * @see UserManagementService::getUserById()
 */
std::optional<std::shared_ptr<UserModel>> BookingManagerController::getPassengerDetails(const std::string& bookingManagerId, const std::string& passengerId) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return std::nullopt;
    }
    return UserManagementService::getUserById(passengerId);
}

/**
 * @brief Retrieves detailed information about a specific flight for an authenticated booking manager.
 * 
 * This method verifies the booking manager's authentication before allowing access to flight details.
 * If authentication fails, the request is denied and no flight information is returned.
 * 
 * @param bookingManagerId The unique identifier of the booking manager requesting flight details
 * @param flightId The unique identifier of the flight to retrieve details for
 * 
 * @return std::optional<std::shared_ptr<FlightModel>> containing the flight details if authentication
 *         succeeds and the flight exists, or std::nullopt if authentication fails or flight not found
 * 
 * @note This method requires valid booking manager authentication to access flight information
 * @see FlightService::getFlightById()
 * @see authenticateBookingManager()
 */
std::optional<std::shared_ptr<FlightModel>> BookingManagerController::getFlightDetails(const std::string& bookingManagerId, const std::string& flightId) {
    if (!authenticateBookingManager(bookingManagerId)) {
        return std::nullopt;
    }
    return FlightService::getFlightById(flightId);
}
