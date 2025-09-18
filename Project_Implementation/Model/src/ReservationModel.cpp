#include "../include/ReservationModel.hpp"
#include "../../Utils/include/IDGenerator.hpp"
#include "../../Repositories/include/UserRepository.hpp"
#include "../../Repositories/include/ReservationRepository.hpp"
#include "../../Repositories/include/FlightRepository.hpp"
#include "../../Repositories/include/PaymentRepository.hpp"
#include <vector>
#include <stdexcept>

/**
 * @brief Constructs a ReservationModel object with the provided details.
 *
 * This constructor initializes a reservation for a passenger on a specific flight,
 * with a designated seat number, reservation status, and associated payment ID.
 * It performs validation to ensure that the passenger ID, flight ID, and payment ID exist
 * in their respective repositories. If any of these IDs are invalid, an exception is thrown.
 * A unique reservation ID is generated and assigned to the reservation.
 *
 * @param flightId The unique identifier of the flight.
 * @param passengerId The unique identifier of the passenger.
 * @param seatNumber The seat number assigned to the reservation.
 * @param status The status of the reservation.
 * @param paymentId The unique identifier of the payment associated with the reservation.
 *
 * @throws std::invalid_argument If the passenger ID, flight ID, or payment ID does not exist.
 */
ReservationModel::ReservationModel(const std::string& flightId, const std::string& passengerId, 
    const std::string& seatNumber, const ReservationStatus& status, const std::string& paymentId) {
    auto userRepository = UserRepository::getInstance();
    if ( !userRepository -> findUserById(passengerId).has_value() ) {
        throw std::invalid_argument("Passenger ID does not exist.");
    }

    auto flightOpt = FlightRepository::getInstance() -> findFlightById(flightId);
    if ( !flightOpt.has_value() ) {
        throw std::invalid_argument("Flight ID does not exist.");
    }

    auto reservationRepository = ReservationRepository::getInstance();
    auto paymentRepository = PaymentRepository::getInstance();
    
    if ( !paymentRepository -> findPaymentById( paymentId ).has_value() ) {
        throw std::invalid_argument("Payment ID " + paymentId + " does not exist.");
    }

    std::string reservationId = "RES-" + IDGenerator::generateUniqueID();
    while ( reservationRepository -> findReservationById(reservationId).has_value() ) {
        reservationId = "RES-" + IDGenerator::generateUniqueID();
    }
    this -> reservationId = reservationId;
    this -> flightId = flightId;
    this -> passengerId = passengerId;
    this -> seatNumber = seatNumber;
    this -> status = status;
    this -> paymentId = paymentId;
}
/**
 * @brief Constructs a ReservationModel object from a JSON representation.
 *
 * This constructor validates the presence of required fields in the input JSON,
 * checks the format and existence of referenced IDs (reservation, flight, passenger, payment),
 * and sets the reservation status and seat assignment accordingly.
 *
 * @param json The JSON object containing reservation data.
 *
 * @throws std::invalid_argument If any required field is missing, if IDs are invalid or do not exist,
 *         or if the reservation status is not recognized.
 */
ReservationModel::ReservationModel(const JSON& json) {
    std::vector<std::string> requiredTags = {"id", "flightId", "passengerId", "seatNumber", "status", "paymentId"};
    for ( const auto& tag : requiredTags ) {
        if (!json.contains(tag)) {
            throw std::invalid_argument("Invalid JSON for ReservationModel: missing tag '" + tag + "'.");
        }
    }

    reservationId = json.at("id").get<std::string>();
    if (reservationId.substr(0, 4) != "RES-") {
        throw std::invalid_argument("Invalid ID for ReservationModel");
    }
    
    flightId = json.at("flightId").get<std::string>();
    auto flightsRepository = FlightRepository::getInstance();
    if ( !flightsRepository -> findFlightById(flightId).has_value() ) {
        throw std::invalid_argument("Flight ID does not exist.");
    }

    passengerId = json.at("passengerId").get<std::string>();
    auto userRepository = UserRepository::getInstance();
    if ( !userRepository -> findUserById(passengerId).has_value() ) {
        throw std::invalid_argument("Passenger ID does not exist.");
    }

    std::string statusStr = json.at("status").get<std::string>();
    if (statusStr == "CONFIRMED") {
        status = ReservationStatus::CONFIRMED;
    }
    else if (statusStr == "CANCELLED") {
        status = ReservationStatus::CANCELLED;
    } else {
        throw std::invalid_argument("Invalid reservation status provided.");
    }
    
    seatNumber = json.at("seatNumber").get<std::string>();
    auto flightOpt = flightsRepository -> findFlightById(flightId);
    if (!flightOpt.has_value()) {
        throw std::invalid_argument("Flight ID does not exist (flight may have been deleted).");
    }
    auto flight = flightOpt.value();
    flight -> setSeatStatus(seatNumber, (status == ReservationStatus::CONFIRMED));

    paymentId = json.at("paymentId").get<std::string>();
    auto paymentRepository = PaymentRepository::getInstance();
    if ( !paymentRepository -> findPaymentById(paymentId).has_value() ) {
        throw std::invalid_argument("Payment ID does not exist.");
    }
}

/**
 * @brief Serializes the ReservationModel object to a JSON representation.
 *
 * This method populates the provided JSON object with the reservation's details,
 * including reservation ID, flight ID, passenger ID, seat number, status, and payment ID.
 * The status field is serialized as a string, with possible values "CONFIRMED" or "CANCELLED".
 *
 * @param json Reference to a JSON object to be populated with the reservation data.
 */
void ReservationModel::to_json(JSON& json) const {
    json = JSON {
        {"id", reservationId},
        {"flightId", flightId},
        {"passengerId", passengerId},
        {"seatNumber", seatNumber},
        {"status", ((status == ReservationStatus::CONFIRMED) ? "CONFIRMED" : "CANCELLED")},
        {"paymentId", paymentId}
    };
}

void ReservationModel::setSeatNumber(const std::string& seatNumber) {
    auto flightOpt = FlightRepository::getInstance() -> findFlightById(flightId);
    if (!flightOpt.has_value()) {
        throw std::invalid_argument("Flight ID does not exist (flight may have been deleted).");
    }
    auto flight = flightOpt.value();
    flight -> setSeatStatus(this -> seatNumber, false);
    this -> seatNumber = seatNumber;
    flight -> setSeatStatus(seatNumber, true);
}