#include "../include/FlightRepository.hpp"
#include "../../Utils/include/JSONManager.hpp"
#include "../../Utils/include/DatabasePathResolver.hpp"

/**
 * @brief Path to the flight database JSON file.
 *
 * This constant defines the relative path to the flights database,
 * which is stored in JSON format and used for flight data operations.
 */
const std::string FLIGHT_DATABASE_PATH = DatabasePathResolver::getDatabasePath() + "flights.json";

/**
 * @brief Constructs a FlightRepository object and initializes the flights data.
 *
 * This constructor parses the flight data from the JSON database file specified
 * by FLIGHT_DATABASE_PATH and populates the flights container using JSONManager.
 */
FlightRepository::FlightRepository() {
    JSONManager::parseJSON(flights, FLIGHT_DATABASE_PATH);
}

/**
 * @brief Returns the singleton instance of FlightRepository.
 *
 * Implements the Meyers' Singleton pattern using a static local variable,
 * ensuring thread-safe and lazy initialization of the FlightRepository instance.
 *
 * @return std::shared_ptr<FlightRepository> Shared pointer to the singleton instance.
 */
std::shared_ptr<FlightRepository> FlightRepository::getInstance() {
    // Meyers' Singleton: thread-safe and lazy-initialized.
    static std::shared_ptr<FlightRepository> instance(new FlightRepository());
    return instance;
}

/**
 * @brief Finds a flight by its unique identifier.
 *
 * Searches the internal flight collection for a flight matching the provided flightId.
 * If found, returns a shared pointer to the FlightModel instance wrapped in std::optional.
 * If not found, returns std::nullopt.
 *
 * @param flightId The unique identifier of the flight to search for.
 * @return std::optional<std::shared_ptr<FlightModel>> Shared pointer to the flight model if found, std::nullopt otherwise.
 */
std::optional<std::shared_ptr<FlightModel>> FlightRepository::findFlightById(const std::string& flightId) const {
    if (flights.find(flightId) == flights.end()) {
        return std::nullopt;
    }
    return flights.at(flightId);
}
/**
 * @brief Retrieves all flights stored in the repository.
 *
 * This method returns a vector containing shared pointers to all FlightModel
 * instances currently managed by the FlightRepository. Each element in the
 * returned vector represents a flight.
 *
 * @return std::vector<std::shared_ptr<FlightModel>> A vector of shared pointers to FlightModel objects.
 */
std::vector<std::shared_ptr<FlightModel>> FlightRepository::getAllFlights() const {
    std::vector<std::shared_ptr<FlightModel>> allFlights;
    for (const auto& pair : flights) {
        allFlights.push_back(pair.second);
    }
    return allFlights;
}
std::vector<std::shared_ptr<FlightModel>> FlightRepository::getFlightsByCriteria (
            const std::string& origin,
            const std::string& destination,
            const DateTime& departureDate
) {
    std::vector<std::shared_ptr<FlightModel>> filteredFlights;
    for (const auto& pair : flights) {
        const auto& flight = pair.second;
        if (flight->getOrigin() == origin &&
            flight->getDestination() == destination &&
            flight -> getDepartureTime().sameDay(departureDate)) {
            filteredFlights.push_back(flight);
        }
    }
    return filteredFlights;
}
/**
 * @brief Adds a new flight to the repository.
 *
 * This method attempts to add a new flight to the internal flights collection.
 * If a flight with the same flight ID already exists, the method returns false
 * and does not add the flight. Otherwise, the flight is added and the method returns true.
 *
 * @param newFlight The flight model to be added.
 * @return true if the flight was successfully added; false if a flight with the same ID already exists.
 */
bool FlightRepository::addFlight(const FlightModel& newFlight) {
    if (flights.find(newFlight.getFlightId()) != flights.end()) {
        return false;
    }
    flights[newFlight.getFlightId()] = std::make_shared<FlightModel>(newFlight);
    return true;
}

/**
 * @brief Updates an existing flight in the repository.
 *
 * This method checks if a flight with the given flight ID exists in the repository.
 * If it exists, the flight information is updated with the provided flight model.
 * If the flight does not exist, the method returns false and no update is performed.
 *
 * @param flight The FlightModel object containing updated flight information.
 * @return true if the flight was successfully updated; false if the flight does not exist.
 */
bool FlightRepository::updateFlight(const FlightModel& flight) {
    if (flights.find(flight.getFlightId()) == flights.end()) {
        return false;
    }
    flights[flight.getFlightId()] = std::make_shared<FlightModel>(flight);
    return true;
}

/**
 * @brief Deletes a flight from the repository by its flight ID.
 *
 * This method searches for a flight with the specified flight ID in the repository.
 * If the flight exists, it is removed from the repository and the method returns true.
 * If the flight does not exist, the method returns false.
 *
 * @param flightId The unique identifier of the flight to be deleted.
 * @return true if the flight was found and deleted; false otherwise.
 */
bool FlightRepository::deleteFlight(const std::string& flightId) {
    if (flights.find(flightId) == flights.end()) {
        return false;
    }
    flights.erase(flightId);
    return true;
}

/**
 * @brief Destructor for the FlightRepository class.
 *
 * This destructor is responsible for saving the current state of the flights
 * collection to a JSON file upon object destruction. It ensures that any changes
 * made to the flights data during the lifetime of the repository are persisted
 * to the file specified by FLIGHT_DATABASE_PATH using the JSONManager utility.
 */
FlightRepository::~FlightRepository() {
    JSONManager::saveToJSON(flights, FLIGHT_DATABASE_PATH);
    flights.clear();
}