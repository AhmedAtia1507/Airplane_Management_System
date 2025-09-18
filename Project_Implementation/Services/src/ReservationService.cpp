#include "../include/ReservationService.hpp"
#include "../../Repositories/include/ReservationRepository.hpp"
#include "../../Model/include/ReservationModelBuilder.hpp"
#include "../../Repositories/include/UserRepository.hpp"
#include "../include/PaymentService.hpp"
#include "../../Model/include/Passenger.hpp"
#include "../../Repositories/include/FlightRepository.hpp"

/**
 * @brief Calculates the price of a seat based on seat number and loyalty points.
 *
 * The price is determined by the seat's row and column:
 * - First class: rows 1-5 ($200 base price)
 * - Business class: rows 6-15 ($150 base price)
 * - Economy class: rows 16+ ($100 base price)
 * - Window seats (columns 'A' or 'F') add $20 premium
 * - Aisle seats (columns 'C' or 'D') add $10 premium
 *
 * Loyalty points provide a discount: 1 point = $1 discount, up to a maximum of 30% off the base price.
 *
 * @param seatNumber The seat identifier (e.g., "12C").
 * @param loyaltyPoints The number of loyalty points to apply as a discount.
 * @return The final seat price after applying any premiums and loyalty discount.
 */
float ReservationService::getSeatPrice(const std::string& seatNumber, float loyaltyPoints) {
    float basePrice = 100.0f; // Default base price

    if (!seatNumber.empty()) {
        int row = std::stoi(seatNumber.substr(0, seatNumber.size() - 1));
        char col = seatNumber.back();

        // First class: rows 1-5
        if (row >= 1 && row <= 5) basePrice = 200.0f;
        // Business class: rows 6-15
        else if (row >= 6 && row <= 15) basePrice = 150.0f;
        // Economy: rows 16+
        else basePrice = 100.0f;

        // Window seat premium (columns A or F)
        if (col == 'A' || col == 'F') basePrice += 20.0f;
        // Aisle seat premium (columns C or D)
        else if (col == 'C' || col == 'D') basePrice += 10.0f;
    }

    // Loyalty points discount: 1 point = $1 discount, max 30% off
    float maxDiscount = basePrice * 0.3f;
    float discount = std::min(loyaltyPoints, maxDiscount);

    return basePrice - discount;
}
/**
 * @brief Retrieves all reservations from the repository.
 *
 * This method returns a vector containing shared pointers to all
 * ReservationModel instances currently stored in the system.
 *
 * @return std::vector<std::shared_ptr<ReservationModel>> 
 *         A list of all reservations.
 */
std::vector<std::shared_ptr<ReservationModel>> ReservationService::getAllReservations() {
    return ReservationRepository::getInstance() -> getAllReservations();
}
/**
 * @brief Retrieves a reservation by its unique identifier.
 *
 * This method queries the ReservationRepository for a reservation matching the provided ID.
 * If a reservation is found, it returns an optional containing a shared pointer to the ReservationModel.
 * If no reservation is found, it returns an empty optional.
 *
 * @param reservationId The unique identifier of the reservation to retrieve.
 * @return std::optional<std::shared_ptr<ReservationModel>> An optional containing the reservation if found, or empty if not found.
 */
std::optional<std::shared_ptr<ReservationModel>> ReservationService::getReservationById(const std::string& reservationId) {
    return ReservationRepository::getInstance() -> findReservationById(reservationId);
}
/**
 * @brief Retrieves all reservations associated with a specific user ID.
 *
 * This function queries the reservation repository to find and return
 * a list of reservations for the passenger identified by the given user ID.
 *
 * @param userId The unique identifier of the user (passenger) whose reservations are to be retrieved.
 * @return std::vector<std::shared_ptr<ReservationModel>> A vector containing shared pointers to ReservationModel objects for the specified user.
 */
std::vector<std::shared_ptr<ReservationModel>> ReservationService::getReservationByUserId(const std::string& userId) {
    return ReservationRepository::getInstance() -> findReservationsByPassenger(userId);
}

/**
 * @brief Adds a reservation for a passenger on a specified flight and seat.
 *
 * This method performs the following steps:
 * - Verifies the passenger exists and has the correct user role.
 * - Calculates the seat price, applying loyalty points for discounts if available.
 * - Updates the passenger's loyalty points (capped at 100).
 * - Processes the payment using the provided payment method and details.
 * - Builds and stores the reservation if payment is successful.
 *
 * @param flightId The unique identifier of the flight.
 * @param seatNumber The seat number to be reserved.
 * @param passengerId The unique identifier of the passenger.
 * @param paymentMethod The payment method to be used.
 * @param paymentDetails Additional payment details in JSON format.
 * @return std::optional<std::shared_ptr<ReservationModel>> An optional containing a shared pointer to the created reservation model,
 *         or std::nullopt if the reservation could not be completed.
 */
std::optional<std::shared_ptr<ReservationModel>> ReservationService::addReservation(
    const std::string& flightId,
    const std::string& seatNumber,
    const std::string& passengerId,
    const std::string& paymentMethod,
    const JSON& paymentDetails
) {
    auto userRepository = UserRepository::getInstance();
    auto userOpt = userRepository -> findUserById(passengerId);
    if (!userOpt.has_value() || (userOpt.value() -> getRole() != UserModel::UserType::Passenger)) {
        return std::nullopt;
    }
    auto passenger = std::dynamic_pointer_cast<Passenger>(userOpt.value());
    if (!passenger) {
        return std::nullopt;
    }
    auto loyaltyPoints = passenger -> getLoyaltyPoints();

    // check if seat is already booked for the flight
    auto flightOpt = FlightRepository::getInstance() -> findFlightById(flightId);
    if (!flightOpt.has_value()) {
        return std::nullopt;
    }
    auto flight = flightOpt.value();
    if (flight -> getSeatStatus(seatNumber)) {
        return std::nullopt; // Seat already booked
    }

    float seatPrice = getSeatPrice(seatNumber, loyaltyPoints);
    if (loyaltyPoints > 0.0f) {
        // Deduct 10% of the seat price after discount from loyalty points (post-discount deduction)
        float deduction = std::min(loyaltyPoints, seatPrice * 0.1f); // Cap deduction to available points
        loyaltyPoints -= deduction;
    }
    else {
        loyaltyPoints += seatPrice * 0.1f; // Add 10% of seat price to loyalty points
        loyaltyPoints = (loyaltyPoints > 100) ? 100 : loyaltyPoints; // Cap loyalty points at 100
    }
    auto paymentOpt = PaymentService::createPayment(passengerId, seatPrice, paymentMethod, paymentDetails);
    if (!paymentOpt.has_value()) {
        return std::nullopt;
    }
    auto reservation = ReservationModelBuilder()
    .setFlightId(flightId)
    .setPassengerId(passengerId)
    .setSeatNumber(seatNumber)
    .setPaymentId(paymentOpt.value() -> getPaymentId())
    .build();
    
    if (ReservationRepository::getInstance() -> addReservation(*reservation)) {
        flight -> setSeatStatus(seatNumber, true); // Mark seat as booked
        passenger -> setLoyaltyPoints(loyaltyPoints);
        return ReservationRepository::getInstance() -> findReservationById(reservation -> getReservationId());
    }

    return std::nullopt;
}

/**
 * @brief Updates an existing reservation with new details.
 * 
 * This method handles the complete update process for a reservation, including
 * seat management when the flight or seat number changes. It ensures data
 * consistency by validating the new flight exists and the new seat is available
 * before making any changes.
 * 
 * @param reservation The ReservationModel object containing the updated reservation details
 * 
 * @return true if the reservation was successfully updated, false otherwise
 * 
 * @details The update process includes:
 * - Retrieves the existing reservation to compare changes
 * - Validates that the new flight exists
 * - Checks if the new seat is available (if seat/flight changed)
 * - Updates the reservation in the repository
 * - Manages seat bookings by unbooking the old seat and booking the new seat
 * 
 * @note If the flight ID or seat number has changed, the method will:
 * - Unbook the previous seat on the old flight
 * - Book the new seat on the new flight
 * 
 * @warning Returns false if:
 * - The original reservation doesn't exist
 * - The new flight doesn't exist
 * - The new seat is already booked (when seat/flight changed)
 * - The repository update operation fails
 */
bool ReservationService::updateReservation(const ReservationModel& reservation) {
    // Retrieve the old reservation to check for seat/flight changes
    auto oldReservationOpt = ReservationRepository::getInstance() -> findReservationById(reservation.getReservationId());
    std::string oldSeatNumber;
    std::string oldFlightId;
    bool seatChanged = false;

    if (oldReservationOpt.has_value()) {
        auto oldReservation = oldReservationOpt.value();
        oldSeatNumber = oldReservation->getSeatNumber();
        oldFlightId = oldReservation->getFlightId();
        // If flight or seat has changed, unbook the old seat
        if (oldReservation->getFlightId() != reservation.getFlightId() ||
            oldReservation->getSeatNumber() != reservation.getSeatNumber()) {
            seatChanged = true;
        }
    }
    else {
        return false;
    }

    auto newFlightOpt = FlightRepository::getInstance() -> findFlightById(reservation.getFlightId());
    if (!newFlightOpt.has_value()) {
        return false; // New flight does not exist
    }

    auto newFlight = newFlightOpt.value();
    if (seatChanged && newFlight->getSeatStatus(reservation.getSeatNumber())) {
        return false; // New seat already booked
    }

    if (ReservationRepository::getInstance() -> updateReservation(reservation)) {
        if (seatChanged) {
            // Unbook the old seat
            auto oldFlightOpt = FlightRepository::getInstance() -> findFlightById(oldFlightId);
            if (oldFlightOpt.has_value()) {
                auto oldFlight = oldFlightOpt.value();
                oldFlight -> setSeatStatus(oldSeatNumber, false);
            }
            // Book the new seat
            newFlight -> setSeatStatus(reservation.getSeatNumber(), true);
        }
        return true;
    }
    return false;
}
/**
 * @brief Deletes a reservation with the specified reservation ID.
 *
 * This function attempts to delete a reservation from the repository
 * using the provided reservation ID. It delegates the deletion operation
 * to the ReservationRepository singleton instance.
 *
 * @param reservationId The unique identifier of the reservation to be deleted.
 * @return true if the reservation was successfully deleted; false otherwise.
 */
bool ReservationService::deleteReservation(const std::string& reservationId) {
    // Retrieve the reservation to be deleted
    auto reservationOpt = ReservationRepository::getInstance() -> findReservationById(reservationId);
    if (!reservationOpt.has_value()) {
        return false;
    }
    auto reservation = reservationOpt.value();
    // Unbook the seat associated with the reservation
    auto flightOpt = FlightRepository::getInstance() -> findFlightById(reservation->getFlightId());
    if (flightOpt.has_value()) {
        auto flight = flightOpt.value();
        flight -> setSeatStatus(reservation->getSeatNumber(), false);
    }
    // Delete the reservation
    return ReservationRepository::getInstance() -> deleteReservation(reservationId);
}