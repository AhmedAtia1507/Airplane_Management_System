#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "../../Model/include/UserModel.hpp"
#include "../../Model/include/FlightModel.hpp"
#include "../../Model/include/AircraftModel.hpp"
#include "../../Model/include/CrewMemberModel.hpp"
#include "../../Utils/include/DateTime.hpp"

/**
 * @class AdminController
 * @brief Static controller class for administrative operations in the airplane management system.
 * 
 * The AdminController provides a comprehensive set of static methods for managing users, flights,
 * and aircraft within the system. All operations require admin authentication through adminId
 * parameter validation. This class cannot be instantiated as it serves as a utility class.
 * 
 * @note This class is designed as a static utility class and cannot be instantiated.
 * @note All public methods require valid admin credentials for authorization.
 */

/**
 * @brief Validates admin credentials for authorization.
 * @param adminId The unique identifier of the admin user
 * @return true if the admin is confirmed and authorized, false otherwise
 */

/**
 * @brief Creates a new user in the system.
 * @param adminId The unique identifier of the admin performing the operation
 * @param username The username for the new user
 * @param password The password for the new user
 * @param role The user type/role to assign to the new user
 * @return Optional containing shared pointer to the created UserModel if successful, nullopt otherwise
 */

/**
 * @brief Updates the password of an existing user.
 * @param adminId The unique identifier of the admin performing the operation
 * @param targetUserId The unique identifier of the user whose password will be updated
 * @param newPassword The new password to set for the user
 * @return true if password update was successful, false otherwise
 */

/**
 * @brief Deletes a user from the system.
 * @param adminId The unique identifier of the admin performing the operation
 * @param targetUserId The unique identifier of the user to be deleted
 * @return true if user deletion was successful, false otherwise
 */

/**
 * @brief Retrieves all users in the system.
 * @param adminId The unique identifier of the admin performing the operation
 * @return Vector of shared pointers to UserModel objects representing all users
 */

/**
 * @brief Retrieves a specific user by their ID.
 * @param adminId The unique identifier of the admin performing the operation
 * @param userId The unique identifier of the user to retrieve
 * @return Optional containing shared pointer to the UserModel if found, nullopt otherwise
 */

/**
 * @brief Retrieves a specific crew member by their ID.
 * @param adminId The unique identifier of the admin performing the operation
 * @param crewMemberId The unique identifier of the crew member to retrieve
 * @return Optional containing shared pointer to the CrewMemberModel if found, nullopt otherwise
 */

/**
 * @brief Retrieves all crew members, optionally filtered by crew type.
 * @param adminId The unique identifier of the admin performing the operation
 * @param role The crew type to filter by (defaults to Pilot)
 * @return Vector of shared pointers to CrewMemberModel objects
 */

/**
 * @brief Adds a new flight to the system.
 * @param adminId The unique identifier of the admin performing the operation
 * @param origin The departure airport/location
 * @param destination The arrival airport/location
 * @param departureTime The scheduled departure date and time
 * @param arrivalTime The scheduled arrival date and time
 * @param aircraftId The unique identifier of the aircraft assigned to this flight
 * @param crewMemberIds Optional vector of crew member IDs to assign to the flight
 * @return Optional containing shared pointer to the created FlightModel if successful, nullopt otherwise
 */

/**
 * @brief Retrieves a specific flight by its ID.
 * @param adminId The unique identifier of the admin performing the operation
 * @param flightId The unique identifier of the flight to retrieve
 * @return Optional containing shared pointer to the FlightModel if found, nullopt otherwise
 */

/**
 * @brief Updates an existing flight with new data.
 * @param adminId The unique identifier of the admin performing the operation
 * @param updatedFlightData The FlightModel object containing updated flight information
 * @return true if flight update was successful, false otherwise
 */

/**
 * @brief Updates specific fields of an existing flight.
 * @param adminId The unique identifier of the admin performing the operation
 * @param flightId The unique identifier of the flight to update
 * @param origin The new departure airport/location
 * @param destination The new arrival airport/location
 * @param departureTime The new scheduled departure date and time
 * @param arrivalTime The new scheduled arrival date and time
 * @param aircraftId The new aircraft ID to assign to this flight
 * @return true if flight update was successful, false otherwise
 */

/**
 * @brief Removes a flight from the system.
 * @param adminId The unique identifier of the admin performing the operation
 * @param flightId The unique identifier of the flight to remove
 * @return true if flight removal was successful, false otherwise
 */

/**
 * @brief Retrieves all flights in the system.
 * @param adminId The unique identifier of the admin performing the operation
 * @return Vector of shared pointers to FlightModel objects representing all flights
 */

/**
 * @brief Assigns multiple crew members to a flight.
 * @param adminId The unique identifier of the admin performing the operation
 * @param flightId The unique identifier of the flight
 * @param crewIds Vector of crew member IDs to assign to the flight
 * @return true if crew assignment was successful, false otherwise
 */

/**
 * @brief Assigns a single crew member to a flight.
 * @param adminId The unique identifier of the admin performing the operation
 * @param flightId The unique identifier of the flight
 * @param crewId The crew member ID to assign to the flight
 * @return true if crew assignment was successful, false otherwise
 */

/**
 * @brief Removes a crew member from a flight.
 * @param adminId The unique identifier of the admin performing the operation
 * @param flightId The unique identifier of the flight
 * @param crewMemberId The crew member ID to remove from the flight
 * @return true if crew member removal was successful, false otherwise
 */

/**
 * @brief Retrieves all crew members assigned to a specific flight.
 * @param adminId The unique identifier of the admin performing the operation
 * @param flightId The unique identifier of the flight
 * @return Vector of shared pointers to CrewMemberModel objects assigned to the flight
 */

/**
 * @brief Adds a new aircraft to the system.
 * @param adminId The unique identifier of the admin performing the operation
 * @param model The aircraft model/type
 * @param capacity The maximum passenger capacity of the aircraft
 * @param numOfRowSeats The number of seats per row in the aircraft
 * @return Optional containing shared pointer to the created AircraftModel if successful, nullopt otherwise
 */

/**
 * @brief Retrieves a specific aircraft by its ID.
 * @param adminId The unique identifier of the admin performing the operation
 * @param aircraftId The unique identifier of the aircraft to retrieve
 * @return Optional containing shared pointer to the AircraftModel if found, nullopt otherwise
 */

/**
 * @brief Updates an existing aircraft's information.
 * @param adminId The unique identifier of the admin performing the operation
 * @param aircraftId The unique identifier of the aircraft to update
 * @param model The new aircraft model/type
 * @param capacity The new maximum passenger capacity
 * @param numOfRowSeats The new number of seats per row
 * @return true if aircraft update was successful, false otherwise
 */

/**
 * @brief Removes an aircraft from the system.
 * @param adminId The unique identifier of the admin performing the operation
 * @param aircraftId The unique identifier of the aircraft to remove
 * @return true if aircraft removal was successful, false otherwise
 */

/**
 * @brief Retrieves all aircraft in the system.
 * @param adminId The unique identifier of the admin performing the operation
 * @return Vector of shared pointers to AircraftModel objects representing all aircraft
 */
class AdminController {
    static bool confirmAdmin(const std::string& adminId);
public:
    AdminController() = delete;

    // --- User Management ---
    static std::optional<std::shared_ptr<UserModel>> createUser(const std::string& adminId, const std::string& username, const std::string& password, 
        const UserModel::UserType& role);
    static bool updateUserPassword(const std::string& adminId, const std::string& targetUserId, const std::string& newPassword);
    static bool deleteUser(const std::string& adminId, const std::string& targetUserId);
    static std::vector<std::shared_ptr<UserModel>> getAllUsers(const std::string& adminId);
    static std::optional<std::shared_ptr<UserModel>> getUserById(const std::string& adminId, const std::string& userId);
    static std::optional<std::shared_ptr<CrewMemberModel>> getCrewMemberById(const std::string& adminId, const std::string& crewMemberId);
    static std::vector<std::shared_ptr<CrewMemberModel>> getAllCrewMembers(const std::string& adminId, const CrewMemberModel::CrewType& role = CrewMemberModel::CrewType::Pilot);

    // --- Flight Management ---
    static std::optional<std::shared_ptr<FlightModel>> addFlight(
        const std::string& adminId,
        const std::string& origin,
        const std::string& destination,
        const DateTime& departureTime,
        const DateTime& arrivalTime,
        const std::string& aircraftId,
        const std::vector<std::string>& crewMemberIds = {}
    );
    static std::optional<std::shared_ptr<FlightModel>> getFlightById(const std::string& adminId, const std::string& flightId);
    static bool updateFlight(const std::string& adminId, const FlightModel& updatedFlightData);
    static bool updateFlight(
        const std::string& adminId,
        const std::string& flightId,
        const std::string& origin,
        const std::string& destination,
        const DateTime& departureTime,
        const DateTime& arrivalTime,
        const std::string& aircraftId
    );
    static bool removeFlight(const std::string& adminId, const std::string& flightId);
    static std::vector<std::shared_ptr<FlightModel>> getAllFlights(const std::string& adminId);

    static bool assignCrewToFlight(const std::string& adminId, const std::string& flightId, const std::vector<std::string>& crewIds);
    static bool assignCrewToFlight(const std::string& adminId, const std::string& flightId, const std::string& crewId);
    static bool removeCrewMemberFromFlight(const std::string& adminId, const std::string& flightId, const std::string& crewMemberId);
    static std::vector<std::shared_ptr<CrewMemberModel>> getCrewMembersOfFlight(const std::string& adminId, const std::string& flightId);
    
    // --- Aircraft Management ---
    static std::optional<std::shared_ptr<AircraftModel>> addAircraft(
        const std::string& adminId,
        const std::string& model,
        int capacity,
        int numOfRowSeats
    );
    static std::optional<std::shared_ptr<AircraftModel>> getAircraftById(const std::string& adminId, const std::string& aircraftId);
    static bool updateAircraft( 
        const std::string& adminId,
        const std::string& aircraftId,
        const std::string& model,
        int capacity,
        int numOfRowSeats
    );
    static bool removeAircraft(const std::string& adminId, const std::string& aircraftId);
    static std::vector<std::shared_ptr<AircraftModel>> getAllAircrafts(const std::string& adminId);
};