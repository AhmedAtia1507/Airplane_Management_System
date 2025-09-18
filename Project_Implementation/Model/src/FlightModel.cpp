#include "../include/FlightModel.hpp"
#include "../../Utils/include/JSONManager.hpp"
#include "../../Utils/include/IDGenerator.hpp"
#include "../../Repositories/include/FlightRepository.hpp"
#include "../../Repositories/include/AircraftRepository.hpp"
#include "../../Repositories/include/CrewMemberRepository.hpp"
#include <stdexcept>

std::pair<int, int> FlightModel::getSeatIndices(const std::string& seatNumber) const {
    auto aircraftOpt = AircraftRepository::getInstance() -> findAircraftById(aircraftId);
    if (!aircraftOpt.has_value()) {
        return {-1, -1};
    }
    auto aircraft = aircraftOpt.value();
    if (seatNumber.empty()) {
        return {-1, -1};
    }

    const auto colIndex = seatNumber.find_first_not_of("0123456789");

    // A valid seat number must have a row number and exactly one column character.
    if (colIndex == std::string::npos || colIndex == 0 || (seatNumber.length() - colIndex) != 1) {
        return {-1, -1};
    }

    // Validate column character
    char colChar = seatNumber[colIndex];
    if (colChar < 'A' || colChar > (aircraft -> getNumOfRowSeats() + 'A' - 1)) {
        return {-1, -1};
    }

    // Validate row number
    std::string rowStr = seatNumber.substr(0, colIndex);
    try {
        int rowNum = std::stoi(rowStr);
        if (rowNum <= 0 || rowNum > aircraft -> getNumOfRows() ) {
            return {-1, -1};
        }
        return {rowNum - 1, colChar - 'A'};
    } catch (...) {
        return {-1, -1};
    }
    return {-1, -1};
}

/**
 * @brief Constructs a FlightModel object with the specified flight details.
 *
 * Initializes a flight with origin, destination, departure and arrival times, aircraft ID, and crew member IDs.
 * Validates that origin and destination are not empty, arrival time is after departure time, and that the specified
 * aircraft and crew members exist in their respective repositories. Initializes the seat map based on the aircraft's
 * configuration and generates a unique flight ID.
 *
 * @param origin The origin airport code or name.
 * @param destination The destination airport code or name.
 * @param departureTime The scheduled departure time of the flight.
 * @param arrivalTime The scheduled arrival time of the flight.
 * @param aircraftId The unique identifier of the aircraft assigned to the flight.
 * @param crewMemberIds A vector of unique identifiers for crew members assigned to the flight.
 *
 * @throws std::invalid_argument If origin or destination is empty, arrival time is not after departure time,
 *         the aircraft ID does not exist, or any crew member ID does not exist.
 */
FlightModel::FlightModel(const std::string& origin, const std::string& destination,
                     const DateTime& departureTime, const DateTime& arrivalTime, const std::string& aircraftId,
                     const std::vector<std::string>& crewMemberIds) :
        origin(origin), destination(destination) {

        if (origin.empty() || destination.empty()) {
            throw std::invalid_argument("Origin and Destination cannot be empty");
        }
        if (arrivalTime <= departureTime) {
            throw std::invalid_argument("Arrival Time must be after Departure Time");
        }
        this -> departureTime = departureTime;
        this -> arrivalTime = arrivalTime;
        
        auto airCraftOpt = AircraftRepository::getInstance() -> findAircraftById(aircraftId);
        if ( airCraftOpt.has_value() ) {
            this -> aircraftId = aircraftId;
        } else {
            throw std::invalid_argument("Aircraft with ID " + aircraftId + " does not exist");
        }
        
        auto crewMemberRepository = CrewMemberRepository::getInstance();
        for (const auto& crewMemberId : crewMemberIds) {
            if (crewMemberRepository->findCrewMemberById(crewMemberId).has_value()) {
                (this -> crewMemberIds).push_back(crewMemberId);
            } else {
                throw std::invalid_argument("Crew Member with ID " + crewMemberId + " does not exist");
            }
        }

        seatMap = std::vector<std::vector<bool>>(airCraftOpt.value() -> getNumOfRows(),
                                        std::vector<bool>(airCraftOpt.value() -> getNumOfRowSeats(), false));
        
        flightId = "FL-" + IDGenerator::generateUniqueID();
        auto flightRepository = FlightRepository::getInstance();
        while (flightRepository->findFlightById(flightId).has_value()) {
            flightId = "FL-" + IDGenerator::generateUniqueID();
        }
}
/**
 * @brief Constructs a FlightModel object from a JSON representation.
 *
 * This constructor validates and initializes a FlightModel instance using the provided JSON object.
 * It checks for the presence of required keys, validates the format and values of flight details,
 * ensures the existence of referenced aircraft and crew members, and verifies the seat map dimensions.
 *
 * @param json The JSON object containing flight information.
 *
 * @throws std::invalid_argument If any required key is missing, if the flight ID format is invalid,
 *         if origin or destination is empty, if departure or arrival times are invalid or incorrectly ordered,
 *         if the referenced aircraft or crew members do not exist, or if the seat map size does not match the aircraft configuration.
 */
FlightModel::FlightModel(const JSON& json) {
    const std::vector<std::string> required_keys = {
        "id", "origin", "destination", "departureTime", "arrivalTime", "aircraftId", "crewMemberIds"
    };
    for (const auto& key : required_keys) {
        if (!json.contains(key)) {
            throw std::invalid_argument("Invalid JSON format for FlightModel: missing key '" + key + "'.");
        }
    }

    flightId = json.at("id").get<std::string>();
    if (flightId.substr(0, 3) != "FL-") {
        throw std::invalid_argument("Invalid ID for FlightModel");
    }
    origin = json.at("origin").get<std::string>();
    destination = json.at("destination").get<std::string>();
    if(origin.empty() || destination.empty()) {
        throw std::invalid_argument("Origin and Destination cannot be empty");
    }
    departureTime = DateTime(json.at("departureTime").get<std::string>());
    if (!departureTime.isValid()) {
        throw std::invalid_argument("Invalid departure time format or value.");
    }

    arrivalTime = DateTime(json.at("arrivalTime").get<std::string>());
    if (!arrivalTime.isValid()) {
        throw std::invalid_argument("Invalid arrival time format or value.");
    }

    if (arrivalTime <= departureTime) {
        throw std::invalid_argument("Arrival Time must be after Departure Time.");
    }

    aircraftId = json.at("aircraftId").get<std::string>();
    auto aircraftOpt = AircraftRepository::getInstance()->findAircraftById(aircraftId);
    if (!aircraftOpt.has_value()) {
        throw std::invalid_argument("Aircraft with ID " + aircraftId + " does not exist.");
    }

    crewMemberIds = json.at("crewMemberIds").get<std::vector<std::string>>();
    for (const auto& crewMemberId : crewMemberIds) {
        if (!CrewMemberRepository::getInstance()->findCrewMemberById(crewMemberId).has_value()) {
            throw std::invalid_argument("Crew Member with ID " + crewMemberId + " does not exist.");
        }
    }

    seatMap = json.at("seatMap").get<std::vector<std::vector<bool>>>();
    int rowSize = static_cast<int>(seatMap.size());
    int colSize = static_cast<int>(seatMap[0].size());

    if ( colSize != aircraftOpt.value() -> getNumOfRowSeats() ||  rowSize != aircraftOpt.value() -> getNumOfRows() ) {
        throw std::invalid_argument("Invalid seat map size");
    }
}

/**
 * @brief Serializes the FlightModel object to a JSON representation.
 *
 * This method populates the provided JSON object with the flight's details,
 * including its ID, origin, destination, departure and arrival times, aircraft ID,
 * crew member IDs, and seat map.
 *
 * @param json Reference to a JSON object that will be populated with the flight data.
 */
void FlightModel::to_json(JSON& json) const {
    json = JSON{
        {"id", flightId},
        {"origin", origin},
        {"destination", destination},
        {"departureTime", departureTime.toString()},
        {"arrivalTime", arrivalTime.toString()},
        {"aircraftId", aircraftId},
        {"crewMemberIds", crewMemberIds},
        {"seatMap", seatMap}
    };
}

/**
 * @brief Sets the status of a specific seat in the flight's seat map.
 *
 * This function updates the status (e.g., occupied or available) of the seat identified
 * by the given seat number. If the seat number is invalid, an exception is thrown.
 *
 * @param seatNumber The identifier of the seat (e.g., "12A").
 * @param status The new status to set for the seat (true for occupied, false for available).
 * @throws std::invalid_argument If the seat number is invalid.
 */
void FlightModel::setSeatStatus(const std::string& seatNumber, bool status) {
    auto seatIndices = getSeatIndices(seatNumber);
    if (seatIndices.first == -1 || seatIndices.second == -1) {
        throw std::invalid_argument("Invalid seat number: " + seatNumber);
    }
    seatMap[seatIndices.first][seatIndices.second] = status;
}

/**
 * @brief Retrieves the status of a specific seat.
 *
 * Given a seat number as a string, this function checks if the seat exists and returns its status.
 * The status is represented as a boolean value, where typically 'true' indicates the seat is occupied
 * and 'false' indicates it is available.
 *
 * @param seatNumber The seat identifier (e.g., "12A").
 * @return true if the seat is occupied, false if it is available.
 * @throws std::invalid_argument if the seat number is invalid or does not exist.
 */
bool FlightModel::getSeatStatus(const std::string& seatNumber) const {
    auto seatIndices = getSeatIndices(seatNumber);
    if (seatIndices.first == -1 || seatIndices.second == -1) {
        throw std::invalid_argument("Invalid seat number: " + seatNumber);
    }
    return seatMap[seatIndices.first][seatIndices.second];
}

/**
 * @brief Removes a crew member ID from the flight's crew member list.
 *
 * This method searches for the specified crew member ID in the flight's list of crew members.
 * If found, it removes the ID from the list and returns true. If the ID is not found,
 * it returns false.
 *
 * @param id The unique identifier of the crew member to be removed.
 * @return true if the crew member ID was found and removed; false otherwise.
 */
bool FlightModel::removeCrewMemberId(const std::string& id) {
    auto it = std::find(crewMemberIds.begin(), crewMemberIds.end(), id);
    if (it != crewMemberIds.end()) {
        crewMemberIds.erase(it);
        return true;
    }
    return false;
}

/**
 * @brief Checks if a given seat number is valid for this flight.
 * 
 * This method validates whether the provided seat number corresponds to a valid
 * seat position within the aircraft's seating configuration. The validation is
 * performed by attempting to convert the seat number to internal seat indices.
 * 
 * @param seatNumber The seat number to validate (e.g., "12A", "5F")
 * @return true if the seat number is valid and exists in the flight configuration,
 *         false otherwise
 * 
 * @note A seat is considered valid if both row and column indices can be
 *       successfully determined from the seat number string
 */
bool FlightModel::isValidSeat(const std::string& seatNumber) const {
    auto seatIndices = getSeatIndices(seatNumber);
    return seatIndices.first != -1 && seatIndices.second != -1;
}