#pragma once

#include <vector>
#include <memory>
#include "../../Model/include/FlightModel.hpp"
#include "../../Model/include/CrewMemberModel.hpp"


/**
 * @brief Service class for managing flight operations in the airplane management system.
 * 
 * The FlightService class provides static methods for performing CRUD operations
 * on flights, managing crew assignments, and querying flight data. This class
 * follows a static utility pattern and cannot be instantiated.
 * 
 * @note This class is designed as a utility class with deleted constructor to prevent instantiation.
 */

/**
 * @brief Retrieves all flights from the system.
 * 
 * @return std::vector<std::shared_ptr<FlightModel>> Vector containing all flights in the system
 */

/**
 * @brief Retrieves a specific flight by its unique identifier.
 * 
 * @param flightId The unique identifier of the flight to retrieve
 * @return std::optional<std::shared_ptr<FlightModel>> The flight if found, std::nullopt otherwise
 */

/**
 * @brief Retrieves flights matching the specified route and departure date.
 * 
 * @param origin The departure airport/location
 * @param destination The arrival airport/location
 * @param departureDate The date of departure
 * @return std::vector<std::shared_ptr<FlightModel>> Vector of flights matching the criteria
 */

/**
 * @brief Creates and adds a new flight to the system.
 * 
 * @param origin The departure airport/location
 * @param destination The arrival airport/location
 * @param departureTime The scheduled departure time
 * @param arrivalTime The scheduled arrival time
 * @param aircraftId The unique identifier of the aircraft assigned to this flight
 * @param crewMemberIds Optional vector of crew member IDs to assign to the flight
 * @return std::optional<std::shared_ptr<FlightModel>> The created flight if successful, std::nullopt otherwise
 */

/**
 * @brief Adds a single crew member to an existing flight.
 * 
 * @param flightId The unique identifier of the flight
 * @param crewMemberId The unique identifier of the crew member to add
 * @return bool True if the crew member was successfully added, false otherwise
 */

/**
 * @brief Adds multiple crew members to an existing flight.
 * 
 * @param flightId The unique identifier of the flight
 * @param crewIds Vector of crew member IDs to add to the flight
 * @return bool True if all crew members were successfully added, false otherwise
 */

/**
 * @brief Removes a crew member from an existing flight.
 * 
 * @param flightId The unique identifier of the flight
 * @param crewMemberId The unique identifier of the crew member to remove
 * @return bool True if the crew member was successfully removed, false otherwise
 */

/**
 * @brief Retrieves all crew members assigned to a specific flight.
 * 
 * @param flightId The unique identifier of the flight
 * @return std::vector<std::shared_ptr<CrewMemberModel>> Vector of crew members assigned to the flight
 */

/**
 * @brief Updates an existing flight with new information from a FlightModel object.
 * 
 * @param flight The FlightModel object containing updated flight information
 * @return bool True if the flight was successfully updated, false otherwise
 */

/**
 * @brief Updates an existing flight with new information using individual parameters.
 * 
 * @param flightId The unique identifier of the flight to update
 * @param origin The new departure airport/location
 * @param destination The new arrival airport/location
 * @param departureTime The new scheduled departure time
 * @param arrivalTime The new scheduled arrival time
 * @param aircraftId The new aircraft identifier
 * @return bool True if the flight was successfully updated, false otherwise
 */

/**
 * @brief Removes a flight from the system.
 * 
 * @param flightId The unique identifier of the flight to delete
 * @return bool True if the flight was successfully deleted, false otherwise
 */
class FlightService {
    public:
        FlightService() = delete;

        static std::vector<std::shared_ptr<FlightModel>> getAllFlights();
        static std::optional<std::shared_ptr<FlightModel>> getFlightById(const std::string& flightId);
        static std::vector<std::shared_ptr<FlightModel>> getFlightsByRouteAndDate (
            const std::string& origin,
            const std::string& destination,
            const DateTime& departureDate
        );
        static std::optional<std::shared_ptr<FlightModel>> addFlight(
            const std::string& origin,
            const std::string& destination,
            const DateTime& departureTime,
            const DateTime& arrivalTime,
            const std::string& aircraftId,
            const std::vector<std::string>& crewMemberIds = {}
        );
        static bool addCrewToFlight(const std::string& flightId, const std::string& crewMemberId);
        static bool addCrewToFlight(const std::string& flightId, const std::vector<std::string>& crewIds);
        static bool removeCrewMemberFromFlight(const std::string& flightId, const std::string& crewMemberId);
        static std::vector<std::shared_ptr<CrewMemberModel>> getCrewMembersOfFlight(const std::string& flightId);
        static bool updateFlight(const FlightModel& flight);
        static bool updateFlight(
            const std::string& flightId,
            const std::string& origin,
            const std::string& destination,
            const DateTime& departureTime,
            const DateTime& arrivalTime,
            const std::string& aircraftId
        );
        static bool deleteFlight(const std::string& flightId);
};