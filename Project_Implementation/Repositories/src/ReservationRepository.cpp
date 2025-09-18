#include "../include/ReservationRepository.hpp"
#include "../../Utils/include/JSONManager.hpp"
#include "../../Utils/include/DatabasePathResolver.hpp"

/**
 * @brief Path to the reservations database file.
 *
 * This constant defines the relative path to the JSON file
 * where reservation data is stored and retrieved.
 */
const std::string RESERVATION_DATABASE_PATH = DatabasePathResolver::getDatabasePath() + "reservations.json";

/**
 * @brief Constructs a ReservationRepository object and initializes the reservations data.
 *
 * This constructor parses the reservation data from the JSON file specified by
 * RESERVATION_DATABASE_PATH and loads it into the reservations container.
 */
ReservationRepository::ReservationRepository() {
    JSONManager::parseJSON(reservations, RESERVATION_DATABASE_PATH);
}

/**
 * @brief Returns a shared pointer to the singleton instance of ReservationRepository.
 *
 * This method implements the Meyers' Singleton pattern, ensuring that the instance
 * is created in a thread-safe and lazy-initialized manner. The returned shared pointer
 * guarantees that the singleton object will be properly destroyed when the program exits.
 *
 * @return std::shared_ptr<ReservationRepository> Shared pointer to the singleton instance.
 */
std::shared_ptr<ReservationRepository> ReservationRepository::getInstance() {
    // Meyers' Singleton: thread-safe and lazy-initialized.
    static std::shared_ptr<ReservationRepository> instance(new ReservationRepository());
    return instance;
}

/**
 * @brief Finds a reservation by its unique identifier.
 *
 * Searches the internal reservations collection for a reservation matching the provided ID.
 * If found, returns a shared pointer to the ReservationModel wrapped in an std::optional.
 * If not found, returns std::nullopt.
 *
 * @param reservationId The unique identifier of the reservation to find.
 * @return std::optional<std::shared_ptr<ReservationModel>> Shared pointer to the reservation if found, std::nullopt otherwise.
 */
std::optional<std::shared_ptr<ReservationModel>> ReservationRepository::findReservationById(const std::string& reservationId) const {
    if (reservations.find(reservationId) == reservations.end()) {
        return std::nullopt;
    }
    return reservations.at(reservationId);
}
std::vector<std::shared_ptr<ReservationModel>> ReservationRepository::getAllReservations() const {
    std::vector<std::shared_ptr<ReservationModel>> allReservations;
    for (const auto& [id, reservation] : reservations) {
        allReservations.push_back(reservation);
    }
    return allReservations;
}
/**
 * @brief Adds a new reservation to the repository.
 *
 * This method checks if a reservation with the same ID already exists.
 * If it does, the method returns false and does not add the reservation.
 * Otherwise, it adds the reservation and returns true.
 *
 * @param newReservation The reservation model to be added.
 * @return true if the reservation was added successfully; false if a reservation with the same ID already exists.
 */
bool ReservationRepository::addReservation(const ReservationModel& newReservation) {
    if (reservations.find(newReservation.getReservationId()) != reservations.end()) {
        return false;
    }
    reservations[newReservation.getReservationId()] = std::make_shared<ReservationModel>(newReservation);
    return true;
}

/**
 * @brief Updates an existing reservation in the repository.
 *
 * This function searches for a reservation with the given reservation ID.
 * If the reservation exists, it updates the stored reservation with the provided data.
 * If the reservation does not exist, the function returns false.
 *
 * @param reservation The ReservationModel object containing updated reservation data.
 * @return true if the reservation was successfully updated; false if the reservation does not exist.
 */
bool ReservationRepository::updateReservation(const ReservationModel& reservation) {
    if (reservations.find(reservation.getReservationId()) == reservations.end()) {
        return false;
    }
    reservations[reservation.getReservationId()] = std::make_shared<ReservationModel>(reservation);
    return true;
}

/**
 * @brief Deletes a reservation with the specified reservation ID.
 *
 * This function searches for a reservation in the repository using the provided
 * reservation ID. If the reservation exists, it is removed from the repository.
 *
 * @param reservationId The unique identifier of the reservation to delete.
 * @return true if the reservation was found and deleted; false otherwise.
 */
bool ReservationRepository::deleteReservation(const std::string& reservationId) {
    if (reservations.find(reservationId) == reservations.end()) {
        return false;
    }
    reservations.erase(reservationId);
    return true;
}

/**
 * @brief Finds all reservations associated with a specific passenger.
 *
 * Iterates through the repository's reservations and collects those
 * that match the provided passenger ID.
 *
 * @param passengerId The unique identifier of the passenger.
 * @return A vector of shared pointers to ReservationModel objects
 *         corresponding to the passenger's reservations.
 */
std::vector<std::shared_ptr<ReservationModel>> ReservationRepository::findReservationsByPassenger(const std::string& passengerId) const {
    std::vector<std::shared_ptr<ReservationModel>> passengerReservations;
    for (const auto& [id, reservation] : reservations) {
        if (reservation->getPassengerId() == passengerId) {
            passengerReservations.push_back(reservation);
        }
    }
    return passengerReservations;
}
/**
 * @brief Destructor for the ReservationRepository class.
 *
 * This destructor is responsible for saving the current state of reservations
 * to a JSON file before the object is destroyed. It ensures that any changes
 * made to the reservations during the lifetime of the repository are persisted
 * to the file specified by RESERVATION_DATABASE_PATH.
 */
ReservationRepository::~ReservationRepository() {
    JSONManager::saveToJSON(reservations, RESERVATION_DATABASE_PATH);
    reservations.clear();
}