#pragma once

#include "../../Model/include/FlightModel.hpp"
#include <memory>
#include <unordered_map>
#include <optional>
#include <string>


/**
 * @class FlightRepository
 * @brief Singleton repository for managing flight records.
 *
 * This class provides an interface for storing, retrieving, updating, and deleting
 * flight information using an internal unordered map. It follows the singleton pattern
 * to ensure a single instance throughout the application.
 *
 * Copy and move operations are deleted to maintain singleton integrity.
 *
 * Public Methods:
 * - getInstance(): Returns the singleton instance of FlightRepository.
 * - findFlightById(const std::string&): Searches for a flight by its ID.
 * - addFlight(const FlightModel&): Adds a new flight to the repository.
 * - updateFlight(const FlightModel&): Updates an existing flight's information.
 * - deleteFlight(const std::string&): Removes a flight from the repository by its ID.
 *
 * Destructor ensures saving the data in the database before destruction.
 */
class FlightRepository {
    std::unordered_map<std::string, std::shared_ptr<FlightModel>> flights;

    FlightRepository();
    FlightRepository(const FlightRepository&) = delete;
    FlightRepository& operator=(const FlightRepository&) = delete;
    FlightRepository(FlightRepository&&) = delete;
    FlightRepository& operator=(FlightRepository&&) = delete;


    public:
        static std::shared_ptr<FlightRepository> getInstance();

        std::optional<std::shared_ptr<FlightModel>> findFlightById(const std::string& flightId) const;
        std::vector<std::shared_ptr<FlightModel>> getAllFlights() const;
        std::vector<std::shared_ptr<FlightModel>> getFlightsByCriteria (
            const std::string& origin,
            const std::string& destination,
            const DateTime& departureDate
        );
        bool addFlight(const FlightModel& newFlight);
        bool updateFlight(const FlightModel& flight);
        bool deleteFlight(const std::string& flightId);

        ~FlightRepository();
};
