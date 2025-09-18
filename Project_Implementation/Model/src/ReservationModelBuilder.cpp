#include "../include/ReservationModelBuilder.hpp"

/**
 * @brief Sets the flight ID for the reservation model.
 * 
 * This method assigns the provided flight ID to the reservation model builder.
 * 
 * @param flightId The unique identifier of the flight to associate with the reservation.
 * @return Reference to the current ReservationModelBuilder instance for method chaining.
 */
ReservationModelBuilder& ReservationModelBuilder::setFlightId(const std::string& flightId) {
    (this -> flightId) = flightId;
    return *this;
}
/**
 * @brief Sets the passenger ID for the reservation.
 * 
 * This method assigns the provided passenger ID to the reservation model being built.
 * 
 * @param passengerId The unique identifier of the passenger.
 * @return Reference to the current ReservationModelBuilder instance for method chaining.
 */
ReservationModelBuilder& ReservationModelBuilder::setPassengerId(const std::string& passengerId) {
    (this -> passengerId) = passengerId;
    return *this;
}
/**
 * @brief Sets the seat number for the reservation.
 * 
 * This method assigns the provided seat number to the reservation model being built.
 * 
 * @param seatNumber The seat number to be set.
 * @return Reference to the current ReservationModelBuilder instance for method chaining.
 */
ReservationModelBuilder& ReservationModelBuilder::setSeatNumber(const std::string& seatNumber) {
    (this -> seatNumber) = seatNumber;
    return *this;
}
/**
 * @brief Sets the reservation status for the ReservationModel being built.
 * 
 * This method assigns the provided status to the internal status member of the builder.
 * 
 * @param status The reservation status to set.
 * @return Reference to the current ReservationModelBuilder instance for method chaining.
 */
ReservationModelBuilder& ReservationModelBuilder::setStatus(const ReservationModel::ReservationStatus& status) {
    (this -> status) = status;
    return *this;
}
/**
 * @brief Sets the payment ID for the reservation.
 * 
 * This method assigns the provided payment ID to the reservation model being built.
 * 
 * @param paymentId The payment ID to associate with the reservation.
 * @return Reference to the current ReservationModelBuilder instance for method chaining.
 */
ReservationModelBuilder& ReservationModelBuilder::setPaymentId(const std::string& paymentId) {
    (this -> paymentId) = paymentId;
    return *this;
}

/**
 * @brief Builds a ReservationModel instance with the provided parameters.
 *
 * This method constructs a ReservationModel object using the parameters
 * set in the ReservationModelBuilder. It validates that all required fields
 * (flightId, passengerId, seatNumber, paymentId) are not empty before
 * creating the ReservationModel. If any required parameter is missing,
 * an std::invalid_argument exception is thrown.
 *
 * @return std::shared_ptr<ReservationModel> A shared pointer to the created ReservationModel instance.
 * @throws std::invalid_argument If any required reservation parameter is missing.
 */
std::shared_ptr<ReservationModel> ReservationModelBuilder::build() const {
    if (flightId.empty() || passengerId.empty() ||
        seatNumber.empty() || paymentId.empty()) {
        throw std::invalid_argument("Missing required reservation parameters.");
    }
    return std::make_shared<ReservationModel> (
        flightId, passengerId, seatNumber, status, paymentId
    );

}