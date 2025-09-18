#include "../include/FlightService.hpp"
#include "../../Model/include/FlightModelBuilder.hpp"
#include "../../Repositories/include/FlightRepository.hpp"
#include "../../Repositories/include/AircraftRepository.hpp"
#include "../../Services/include/CrewMemberService.hpp"
/**
 * @brief Retrieves all available flights.
 *
 * This method fetches a list of all flights from the flight repository.
 *
 * @return A vector containing shared pointers to FlightModel objects representing all flights.
 */
std::vector<std::shared_ptr<FlightModel>> FlightService::getAllFlights() {
    return FlightRepository::getInstance() -> getAllFlights();
}
/**
 * @brief Retrieves a flight by its unique identifier.
 *
 * This method queries the FlightRepository for a flight matching the provided flightId.
 * If a flight is found, it returns an optional containing a shared pointer to the FlightModel.
 * If no flight is found, it returns an empty optional.
 *
 * @param flightId The unique identifier of the flight to retrieve.
 * @return std::optional<std::shared_ptr<FlightModel>> An optional containing the flight if found, or empty if not found.
 */
std::optional<std::shared_ptr<FlightModel>> FlightService::getFlightById(const std::string& flightId) {
    return FlightRepository::getInstance() -> findFlightById(flightId);
}
/**
 * @brief Retrieves a list of flights matching the specified route and departure date.
 *
 * This function queries the flight repository for flights that originate from the given
 * origin, arrive at the specified destination, and depart on the provided date.
 *
 * @param origin The IATA code or name of the departure location.
 * @param destination The IATA code or name of the arrival location.
 * @param departureDate The date and time of departure to filter flights.
 * @return std::vector<std::shared_ptr<FlightModel>> A vector containing shared pointers to matching FlightModel instances.
 */
std::vector<std::shared_ptr<FlightModel>> FlightService::getFlightsByRouteAndDate (
            const std::string& origin,
            const std::string& destination,
            const DateTime& departureDate
) {
    return FlightRepository::getInstance() -> getFlightsByCriteria(origin, destination, departureDate);
}
/**
 * @brief Adds a new flight to the system.
 *
 * Creates a FlightModel instance with the provided origin, destination, departure time,
 * arrival time, and aircraft ID. Attempts to add the flight to the FlightRepository.
 * If successful, returns an optional containing a shared pointer to the newly added FlightModel.
 * If the addition fails, returns std::nullopt.
 *
 * @param origin The origin airport code or name.
 * @param destination The destination airport code or name.
 * @param departureTime The scheduled departure time of the flight.
 * @param arrivalTime The scheduled arrival time of the flight.
 * @param aircraftId The identifier of the aircraft assigned to the flight.
 * @return std::optional<std::shared_ptr<FlightModel>> Optional containing the added flight if successful, std::nullopt otherwise.
 */
std::optional<std::shared_ptr<FlightModel>> FlightService::addFlight(
    const std::string& origin,
    const std::string& destination,
    const DateTime& departureTime,
    const DateTime& arrivalTime,
    const std::string& aircraftId,
    const std::vector<std::string>& crewMemberIds
) {
    auto flight = FlightModelBuilder()
        .setOrigin(origin)
        .setDestination(destination)
        .setDepartureTime(departureTime)
        .setArrivalTime(arrivalTime)
        .setAircraftId(aircraftId)
        .setCrewMemberIds(crewMemberIds)
        .build();
    if (FlightRepository::getInstance() -> addFlight(*flight)) {
        return FlightRepository::getInstance() -> findFlightById(flight->getFlightId());
    }
    return std::nullopt;
}
/**
 * @brief Updates the details of an existing flight.
 *
 * This method attempts to update the flight information in the repository
 * using the provided FlightModel object. It delegates the update operation
 * to the FlightRepository singleton instance.
 *
 * @param flight The FlightModel object containing updated flight details.
 * @return true if the update was successful, false otherwise.
 */
bool FlightService::updateFlight(const FlightModel& flight) {
    return FlightRepository::getInstance() -> updateFlight(flight);
}
/**
 * @brief Updates the details of an existing flight.
 *
 * This method attempts to update the flight information in the repository
 * using the provided flight details.
 *
 * @param flightId The unique identifier of the flight to update.
 * @param origin The new origin location for the flight.
 * @param destination The new destination location for the flight.
 * @param departureTime The new departure time for the flight.
 * @param arrivalTime The new arrival time for the flight.
 * @param aircraftId The unique identifier of the aircraft to assign to the flight.
 * @return true if the flight was successfully updated; false otherwise.
 */
bool FlightService::updateFlight(
    const std::string& flightId,
    const std::string& origin,
    const std::string& destination,
    const DateTime& departureTime,
    const DateTime& arrivalTime,
    const std::string& aircraftId
) {
    auto flightOpt = FlightRepository::getInstance() -> findFlightById(flightId);
    if (!flightOpt.has_value()) {
        return false;
    }
    if (!origin.empty() && !destination.empty() && origin == destination) {
        return false; // Origin and destination cannot be the same
    }
    if (!departureTime.isValid() || !arrivalTime.isValid() || arrivalTime <= departureTime) {
        return false; // Invalid date/time
    }
    if (!aircraftId.empty() && !AircraftRepository::getInstance() -> findAircraftById(aircraftId).has_value()) {
        return false; // Aircraft does not exist
    }
    auto flight = flightOpt.value();
    flight -> setOrigin(origin);
    flight -> setDestination(destination);
    flight -> setDepartureTime(departureTime);
    flight -> setArrivalTime(arrivalTime);
    flight -> setAircraftId(aircraftId);
    return FlightRepository::getInstance() -> updateFlight(*flight);
}
/**
 * @brief Deletes a flight with the specified flight ID.
 *
 * This method attempts to remove the flight identified by the given flightId
 * from the flight repository. It delegates the deletion operation to the
 * FlightRepository singleton instance.
 *
 * @param flightId The unique identifier of the flight to be deleted.
 * @return true if the flight was successfully deleted; false otherwise.
 */
bool FlightService::deleteFlight(const std::string& flightId) {
    return FlightRepository::getInstance() -> deleteFlight(flightId);
}
/**
 * @brief Assigns a list of crew member IDs to a specific flight.
 *
 * This method attempts to find a flight by its unique identifier and, if found,
 * sets the crew member IDs for that flight. If the flight does not exist, the operation fails.
 *
 * @param flightId The unique identifier of the flight to which crew members will be assigned.
 * @param crewIds A vector containing the unique identifiers of the crew members to assign.
 * @return true if the crew members were successfully assigned to the flight; false if the flight was not found.
 */
bool FlightService::addCrewToFlight(const std::string& flightId, const std::vector<std::string>& crewIds) {
    auto FlightOpt = FlightRepository::getInstance() -> findFlightById(flightId);
    if (!FlightOpt.has_value()) {
        return false;
    }
    FlightOpt.value() -> setCrewMemberIds(crewIds);
    return true;
}
/**
 * @brief Adds a crew member to a specified flight.
 *
 * This method attempts to add the crew member identified by `crewMemberId`
 * to the flight identified by `flightId`. If the flight does not exist,
 * the operation fails and returns false.
 *
 * @param flightId The unique identifier of the flight.
 * @param crewMemberId The unique identifier of the crew member to add.
 * @return true if the crew member was successfully added to the flight; false otherwise.
 */
bool FlightService::addCrewToFlight(const std::string& flightId, const std::string& crewMemberId) {
    auto FlightOpt = FlightRepository::getInstance() -> findFlightById(flightId);
    if (!FlightOpt.has_value()) {
        return false;
    }
    FlightOpt.value() -> addCrewMemberId(crewMemberId);
    return true;
}
/**
 * @brief Removes a crew member from a specific flight.
 * 
 * This method attempts to remove a crew member from the specified flight by their ID.
 * The operation will only update the repository if the crew member was successfully
 * removed from the flight's crew list.
 * 
 * @param flightId The unique identifier of the flight from which to remove the crew member
 * @param crewMemberId The unique identifier of the crew member to be removed
 * 
 * @return true if the crew member was successfully removed from the flight and the 
 *         repository was updated; false if the flight was not found, the crew member
 *         was not part of the flight's crew, or the repository update failed
 * 
 * @note The method performs validation to ensure the flight exists before attempting
 *       to remove the crew member
 */
bool FlightService::removeCrewMemberFromFlight(const std::string& flightId, const std::string& crewMemberId) {
    auto flightOpt = FlightRepository::getInstance() -> findFlightById(flightId);
    if (!flightOpt.has_value()) {
        return false;
    }
    auto flight = flightOpt.value();
    if (!flight -> removeCrewMemberId(crewMemberId)) {
        return false;
    }
    // Only update repository if a crew member was actually removed
    return FlightRepository::getInstance()->updateFlight(*flight);
}
/**
 * @brief Retrieves all crew members associated with a specific flight.
 * 
 * This method looks up a flight by its ID and returns a vector containing
 * all crew members assigned to that flight. If the flight is not found,
 * an empty vector is returned. Any crew member IDs that cannot be resolved
 * to actual crew member objects are silently skipped.
 * 
 * @param flightId The unique identifier of the flight to get crew members for
 * @return std::vector<std::shared_ptr<CrewMemberModel>> Vector of shared pointers
 *         to crew member objects. Returns empty vector if flight not found or
 *         if no crew members are assigned to the flight.
 * 
 * @note This method performs multiple database/repository lookups and may be
 *       expensive for flights with many crew members.
 */
std::vector<std::shared_ptr<CrewMemberModel>> FlightService::getCrewMembersOfFlight(const std::string& flightId) {
    auto flightOpt = FlightRepository::getInstance() -> findFlightById(flightId);
    if (!flightOpt.has_value()) {
        return {};
    }
    auto flight = flightOpt.value();
    std::vector<std::shared_ptr<CrewMemberModel>> crewMembers;
    for (const auto& crewId : flight->getCrewMemberIds()) {
        auto crewOpt = CrewMemberService::getCrewMemberById(crewId);
        if (crewOpt.has_value()) {
            crewMembers.push_back(crewOpt.value());
        }
    }
    return crewMembers;
}