#include "../include/PassengerController.hpp"
#include "../../Services/include/FlightService.hpp"
#include "../../Services/include/ReservationService.hpp"
#include "../../Services/include/UserManagementService.hpp"
#include "../../Services/include/PaymentService.hpp"

/**
 * @brief Authenticates a passenger by verifying their user role
 * 
 * This method checks if the provided passenger ID corresponds to a user
 * with the Passenger role in the system. It uses the UserManagementService
 * to retrieve the user's role and validates it against the expected Passenger type.
 * 
 * @param passengerId The unique identifier of the passenger to authenticate
 * @return true if the user exists and has Passenger role, false otherwise
 */
bool PassengerController::authenticatePassenger(const std::string& passengerId) {
    if (UserManagementService::getUserRole(passengerId) == UserModel::UserType::Passenger) {
        return true;
    }
    return false;
}
/**
 * @brief Retrieves all available flights for an authenticated passenger.
 * 
 * This method first authenticates the passenger using their ID. If authentication
 * is successful, it returns all flights available in the system. If authentication
 * fails, an empty vector is returned.
 * 
 * @param passengerId The unique identifier of the passenger requesting flight information
 * @return std::vector<std::shared_ptr<FlightModel>> A vector containing all available flights
 *         if authentication succeeds, or an empty vector if authentication fails
 * 
 * @note This method requires a valid passenger ID for authentication
 * @see authenticatePassenger()
 * @see FlightService::getAllFlights()
 */
std::vector<std::shared_ptr<FlightModel>> PassengerController::getAllFlights(const std::string& passengerId) {
    if (!authenticatePassenger(passengerId)) {
        return {};
    }
    return FlightService::getAllFlights();
}

/**
 * @brief Retrieves flights matching the specified route and departure date for an authenticated passenger.
 * 
 * This method first authenticates the passenger using their ID, then searches for available flights
 * between the specified origin and destination on the given departure date.
 * 
 * @param passengerId The unique identifier of the passenger requesting flight information
 * @param origin The departure airport/location code
 * @param destination The arrival airport/location code  
 * @param departureDate The desired departure date for the flight search
 * 
 * @return std::vector<std::shared_ptr<FlightModel>> A vector of shared pointers to FlightModel objects
 *         representing flights that match the search criteria. Returns an empty vector if passenger
 *         authentication fails or no flights are found.
 * 
 * @note Authentication is required before flight search is performed. If authentication fails,
 *       an empty vector is returned without performing the search.
 */
std::vector<std::shared_ptr<FlightModel>> PassengerController::getFlightsByRouteAndDate(
    const std::string& passengerId,
    const std::string& origin,
    const std::string& destination,
    const DateTime& departureDate
) {
    if (!authenticatePassenger(passengerId)) {
        return {};
    }
    return FlightService::getFlightsByRouteAndDate(origin, destination, departureDate);
}

/**
 * @brief Books a flight for an authenticated passenger.
 * 
 * This method attempts to book a flight for a passenger by first authenticating
 * the passenger and then creating a reservation through the ReservationService.
 * 
 * @param passengerId The unique identifier of the passenger attempting to book the flight
 * @param flightId The unique identifier of the flight to be booked
 * @param seatNumber The specific seat number to be reserved on the flight
 * @param paymentType The type of payment method being used (e.g., "credit_card", "paypal")
 * @param paymentDetails JSON object containing payment-specific details and information
 * 
 * @return std::optional<std::shared_ptr<ReservationModel>> Returns a shared pointer to the 
 *         created ReservationModel if booking is successful, or std::nullopt if the 
 *         passenger authentication fails or reservation creation fails
 * 
 * @note The passenger must be authenticated before the booking process can proceed
 * @see authenticatePassenger()
 * @see ReservationService::addReservation()
 */
std::optional<std::shared_ptr<ReservationModel>> PassengerController::bookFlight(
    const std::string& passengerId, 
    const std::string& flightId,
    const std::string& seatNumber,
    const std::string& paymentType,
    const JSON& paymentDetails
) {
    if (!authenticatePassenger(passengerId)) {
        return std::nullopt;
    }
    return ReservationService::addReservation (
        passengerId,
        flightId,
        seatNumber,
        paymentType,
        paymentDetails
    );
}
/**
 * @brief Retrieves all reservations associated with a specific passenger.
 * 
 * This method first authenticates the passenger using their ID, and if authentication
 * is successful, fetches and returns all reservations made by that passenger.
 * 
 * @param passengerId The unique identifier of the passenger whose reservations are to be retrieved
 * @return std::vector<std::shared_ptr<ReservationModel>> A vector containing shared pointers to 
 *         the passenger's reservation models. Returns an empty vector if authentication fails
 *         or if the passenger has no reservations.
 * 
 * @note This method requires valid passenger authentication before retrieving reservations
 * @see authenticatePassenger()
 * @see ReservationService::getReservationByUserId()
 */
std::vector<std::shared_ptr<ReservationModel>> PassengerController::getPassengerReservations(const std::string& passengerId) {
    if (!authenticatePassenger(passengerId)) {
        return {};
    }
    return ReservationService::getReservationByUserId(passengerId);
}
/**
 * @brief Retrieves flight details for an authenticated passenger
 * 
 * This method authenticates the passenger using their ID and, if successful,
 * retrieves the flight details for the specified flight ID.
 * 
 * @param passengerId The unique identifier of the passenger requesting flight details
 * @param flightId The unique identifier of the flight to retrieve details for
 * 
 * @return std::optional<std::shared_ptr<FlightModel>> Returns a shared pointer to the 
 *         FlightModel if the passenger is authenticated and the flight exists, 
 *         otherwise returns std::nullopt
 * 
 * @note The method returns std::nullopt if passenger authentication fails
 * @see authenticatePassenger()
 * @see FlightService::getFlightById()
 */
std::optional<std::shared_ptr<FlightModel>> PassengerController::getFlightDetails(
    const std::string& passengerId, 
    const std::string& flightId
) {
    if (!authenticatePassenger(passengerId)) {
        return std::nullopt;
    }
    return FlightService::getFlightById(flightId);
}
/**
 * @brief Processes a payment for a passenger after authentication.
 * 
 * This method authenticates the passenger using their ID and then processes
 * the specified payment through the PaymentService. The operation ensures
 * that only authenticated passengers can process payments.
 * 
 * @param passengerId The unique identifier of the passenger requesting payment processing
 * @param paymentId The unique identifier of the payment to be processed
 * 
 * @return std::string The result or confirmation of the payment processing operation
 * 
 * @throws std::runtime_error If the passenger authentication fails (invalid passenger ID)
 * 
 * @note This method requires valid passenger authentication before proceeding with payment
 */
std::string PassengerController::processPayment(const std::string& passengerId, const std::string& paymentId) {
    if (!authenticatePassenger(passengerId)) {
        throw std::runtime_error("Unauthorized access: Invalid passenger ID.");
    }
    return PaymentService::processPayment(paymentId);
}