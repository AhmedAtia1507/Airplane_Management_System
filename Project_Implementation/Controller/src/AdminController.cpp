#include "../include/AdminController.hpp"
#include "../../Services/include/UserManagementService.hpp"
#include "../../Services/include/CrewMemberService.hpp"
#include "../../Services/include/FlightService.hpp"
#include "../../Services/include/AircraftService.hpp"

/**
 * @brief Confirms whether the given user ID belongs to a valid admin.
 *
 * This function checks if the user with the specified adminId exists in the repository
 * and has the role of Admin. If the user does not exist or is not an admin, the function
 * returns false. Otherwise, it returns true.
 *
 * @param adminId The ID of the user to be checked.
 * @return true if the user exists and is an admin, false otherwise.
 */
bool AdminController::confirmAdmin(const std::string& adminId) {
    // Check if admin is valid
    if (UserManagementService::getUserRole(adminId) != UserModel::UserType::Admin) {
        return false;
    }
    return true;
}
/**
 * @brief Creates a new user in the system if the requesting admin is confirmed.
 * 
 * @param adminId The unique identifier of the admin requesting the user creation.
 * @param username The username for the new user.
 * @param password The password for the new user.
 * @param role The role/type of the new user (e.g., admin, regular user).
 * @return std::optional<std::shared_ptr<UserModel>>
 *         Returns a shared pointer to the newly created UserModel if successful,
 *         or std::nullopt if the admin is not confirmed or user creation fails.
 */
std::optional<std::shared_ptr<UserModel>> AdminController::createUser(const std::string& adminId, const std::string& username,
    const std::string& password, const UserModel::UserType& role) {

    if (!confirmAdmin(adminId)) {
        return std::nullopt;
    }

    auto newUser = UserManagementService::createUser(username, password, role);
    if (newUser.has_value()) {
        return newUser;
    }
    return std::nullopt;
}

/**
 * @brief Updates the password of a target user by an admin.
 *
 * This function verifies if the provided admin ID corresponds to a valid admin.
 * If confirmed, it attempts to find the user with the specified target user ID.
 * If the user exists, their password is updated to the new password provided.
 * The function then updates the user information in the repository.
 *
 * @param adminId The ID of the admin requesting the password update.
 * @param targetUserId The ID of the user whose password is to be updated.
 * @param newPassword The new password to set for the target user.
 * @return true if the password was successfully updated; false otherwise.
 */
bool AdminController::updateUserPassword(const std::string& adminId, const std::string& targetUserId, const std::string& newPassword) {
    if (!confirmAdmin(adminId)) {
        return false;
    }
    return UserManagementService::updateUserPassword(targetUserId, newPassword);
}
/**
 * @brief Deletes a user from the system if the requesting user is an admin.
 * 
 * This function first verifies that the provided adminId belongs to a valid administrator.
 * If the check passes, it attempts to delete the user with the specified targetUserId
 * from the user repository.
 * 
 * @param adminId The ID of the administrator requesting the deletion.
 * @param targetUserId The ID of the user to be deleted.
 * @return true if the user was successfully deleted; false otherwise (including if the adminId is not valid).
 */
bool AdminController::deleteUser(const std::string& adminId, const std::string& targetUserId) {
    if (!confirmAdmin(adminId)) {
        return false;
    }
    return UserManagementService::deleteUser(targetUserId);
}

/**
 * @brief Retrieves all users from the repository if the provided admin ID is valid.
 *
 * This function checks whether the given admin ID corresponds to a valid administrator.
 * If the admin ID is confirmed, it fetches and returns a list of all users from the user repository.
 * If the admin ID is not valid, it returns an empty vector.
 *
 * @param adminId The ID of the administrator requesting the user list.
 * @return std::vector<std::shared_ptr<UserModel>> A vector containing shared pointers to UserModel objects,
 *         or an empty vector if the admin ID is not valid.
 */
std::vector<std::shared_ptr<UserModel>> AdminController::getAllUsers(const std::string& adminId) {
    if (!confirmAdmin(adminId)) {
        return {};
    }
    return UserManagementService::getAllUsers();
}
/**
 * @brief Retrieves a user by their ID if the requesting user is an admin.
 * 
 * This function checks whether the provided adminId corresponds to a valid admin.
 * If the admin is confirmed, it attempts to retrieve the user with the specified userId.
 * Returns an optional containing a shared pointer to the UserModel if found, or std::nullopt otherwise.
 * 
 * @param adminId The ID of the admin requesting the user information.
 * @param userId The ID of the user to retrieve.
 * @return std::optional<std::shared_ptr<UserModel>> Optional containing the user model if found and admin is confirmed, std::nullopt otherwise.
 */
std::optional<std::shared_ptr<UserModel>> AdminController::getUserById(const std::string& adminId, const std::string& userId) {
    if (!confirmAdmin(adminId)) {
        return std::nullopt;
    }
    return UserManagementService::getUserById(userId);
}
/**
 * @brief Retrieves all crew members of a specified role if the requester is a confirmed admin.
 * 
 * @param adminId The unique identifier of the admin requesting the crew members.
 * @param role The role of crew members to retrieve (e.g., Pilot, Engineer).
 * @return std::vector<std::shared_ptr<CrewMemberModel>> 
 *         A vector of shared pointers to CrewMemberModel objects matching the specified role.
 *         Returns an empty vector if the adminId is not confirmed.
 */
std::vector<std::shared_ptr<CrewMemberModel>> AdminController::getAllCrewMembers(const std::string& adminId, const CrewMemberModel::CrewType& role) {
    if (!confirmAdmin(adminId)) {
        return {};
    }
    return CrewMemberService::getCrewMembersByRole(role);
}
/**
 * @brief Retrieves a crew member by their ID with admin authorization
 * 
 * This method first verifies that the requesting user has admin privileges
 * before attempting to retrieve the crew member information. Only authorized
 * admins can access crew member details.
 * 
 * @param adminId The ID of the admin making the request
 * @param crewMemberId The ID of the crew member to retrieve
 * @return std::optional<std::shared_ptr<CrewMemberModel>> Returns a shared pointer
 *         to the crew member model if found and admin is authorized, std::nullopt otherwise
 * 
 * @note Returns std::nullopt if admin authorization fails or crew member is not found
 */
std::optional<std::shared_ptr<CrewMemberModel>> AdminController::getCrewMemberById(const std::string& adminId, const std::string& crewMemberId) {
    if (!confirmAdmin(adminId)) {
        return std::nullopt;
    }
    return CrewMemberService::getCrewMemberById(crewMemberId);
}
/**
 * @brief Adds a new flight to the system if the provided admin ID is valid.
 *
 * This method creates a new flight using the specified origin, destination,
 * departure and arrival times, and aircraft ID. The flight is added to the
 * flight repository if the admin ID is confirmed to be valid.
 *
 * @param adminId The ID of the administrator attempting to add the flight.
 * @param origin The origin location of the flight.
 * @param destination The destination location of the flight.
 * @param departureTime The scheduled departure time of the flight.
 * @param arrivalTime The scheduled arrival time of the flight.
 * @param aircraftId The ID of the aircraft assigned to the flight.
 * @return std::optional<std::shared_ptr<FlightModel>> Returns a shared pointer
 *         to the newly created FlightModel if the flight is successfully added;
 *         otherwise, returns std::nullopt.
 */
std::optional<std::shared_ptr<FlightModel>> AdminController::addFlight(
        const std::string& adminId,
        const std::string& origin,
        const std::string& destination,
        const DateTime& departureTime,
        const DateTime& arrivalTime,
        const std::string& aircraftId,
        const std::vector<std::string>& crewMemberIds
    ) {
    if (!confirmAdmin(adminId)) {
        return std::nullopt;
    }
    return FlightService::addFlight(origin, destination, departureTime, arrivalTime, aircraftId, crewMemberIds);
}

/**
 * @brief Removes a flight from the system if the requesting user is an admin.
 *
 * This function verifies the admin privileges of the user identified by `adminId`.
 * If the user is confirmed as an admin, it attempts to remove the flight specified
 * by `flightId` from the flight repository.
 *
 * @param adminId The unique identifier of the admin requesting the removal.
 * @param flightId The unique identifier of the flight to be removed.
 * @return true if the flight was successfully removed; false if the user is not an admin
 *         or if the removal operation failed.
 */
bool AdminController::removeFlight(const std::string& adminId, const std::string& flightId) {
    if (!confirmAdmin(adminId)) {
        return false;
    }
    return FlightService::deleteFlight(flightId);
}

/**
 * @brief Updates the details of a flight in the system.
 *
 * This method verifies if the provided admin ID belongs to a valid administrator.
 * If the admin is confirmed, it updates the flight information using the provided
 * updated flight data.
 *
 * @param adminId The unique identifier of the administrator requesting the update.
 * @param updatedFlightData The flight data containing updated information.
 * @return true if the flight was successfully updated; false otherwise (e.g., invalid admin).
 */
bool AdminController::updateFlight(const std::string& adminId, const FlightModel& updatedFlightData) {
    if (!confirmAdmin(adminId)) {
        return false;
    }
    return FlightService::updateFlight(updatedFlightData);
}
bool AdminController::updateFlight(
        const std::string& adminId,
        const std::string& flightId,
        const std::string& origin,
        const std::string& destination,
        const DateTime& departureTime,
        const DateTime& arrivalTime,
        const std::string& aircraftId
    ) {
    if (!confirmAdmin(adminId)) {
        return false;
    }
    return FlightService::updateFlight(flightId, origin, destination, departureTime, arrivalTime, aircraftId);
}
/**
 * @brief Retrieves all flights from the flight repository if the provided admin ID is valid.
 * 
 * This function first verifies the admin's identity using the given adminId.
 * If the adminId is not valid, it returns an empty vector.
 * Otherwise, it fetches and returns all flights from the FlightRepository.
 * 
 * @param adminId The unique identifier of the admin requesting the flights.
 * @return std::vector<std::shared_ptr<FlightModel>> A vector containing shared pointers to all FlightModel objects,
 *         or an empty vector if the adminId is not confirmed.
 */
std::vector<std::shared_ptr<FlightModel>> AdminController::getAllFlights(const std::string& adminId) {
    if (!confirmAdmin(adminId)) {
        return {};
    }
    return FlightService::getAllFlights();
}
/**
 * @brief Retrieves a flight by its ID if the requesting user is an admin.
 *
 * This function checks if the provided admin ID corresponds to a valid admin.
 * If the check passes, it attempts to retrieve the flight with the specified flight ID.
 * If the admin confirmation fails, it returns std::nullopt.
 *
 * @param adminId The ID of the admin requesting the flight information.
 * @param flightId The ID of the flight to retrieve.
 * @return std::optional<std::shared_ptr<FlightModel>> The flight model if found and admin is confirmed, otherwise std::nullopt.
 */
std::optional<std::shared_ptr<FlightModel>> AdminController::getFlightById(const std::string& adminId, const std::string& flightId) {
    if (!confirmAdmin(adminId)) {
        return std::nullopt;
    }
    return FlightService::getFlightById(flightId);
}
/**
 * @brief Assigns a list of crew members to a specific flight.
 *
 * This function verifies the admin's identity, locates the flight by its ID,
 * and assigns the provided crew member IDs to the flight. If the admin is not
 * confirmed or the flight does not exist, the assignment fails.
 *
 * @param adminId The unique identifier of the admin performing the assignment.
 * @param flightId The unique identifier of the flight to assign crew to.
 * @param crewIds A vector containing the unique identifiers of crew members to assign.
 * @return true if the crew was successfully assigned to the flight; false otherwise.
 */
bool AdminController::assignCrewToFlight(const std::string& adminId, const std::string& flightId, const std::vector<std::string>& crewIds) {
    if (!confirmAdmin(adminId)) {
        return false;
    }
    return FlightService::addCrewToFlight(flightId, crewIds);
}
/**
 * @brief Assigns a crew to a specific flight if the admin is confirmed.
 *
 * This function checks whether the provided admin ID corresponds to a valid admin.
 * If the admin is confirmed, it assigns the crew (identified by crewId) to the flight (identified by flightId).
 *
 * @param adminId The unique identifier of the admin attempting the assignment.
 * @param flightId The unique identifier of the flight to which the crew will be assigned.
 * @param crewId The unique identifier of the crew to be assigned to the flight.
 * @return true if the crew was successfully assigned to the flight; false otherwise.
 */
bool AdminController::assignCrewToFlight(const std::string& adminId, const std::string& flightId, const std::string& crewId) {
    if (!confirmAdmin(adminId)) {
        return false;
    }
    return FlightService::addCrewToFlight(flightId, crewId);
}
/**
 * @brief Removes a crew member from a specific flight.
 * 
 * This method allows an authenticated admin to remove a crew member from a flight's crew roster.
 * The operation requires valid admin credentials and will only proceed if the admin is confirmed.
 * 
 * @param adminId The unique identifier of the admin performing the operation
 * @param flightId The unique identifier of the flight from which to remove the crew member
 * @param crewMemberId The unique identifier of the crew member to be removed
 * 
 * @return true if the crew member was successfully removed from the flight, false otherwise
 *         (including cases where admin authentication fails or the removal operation fails)
 * 
 * @note The method first validates admin credentials before attempting the removal operation
 * @see FlightService::removeCrewMemberFromFlight()
 * @see confirmAdmin()
 */
bool AdminController::removeCrewMemberFromFlight(const std::string& adminId, const std::string& flightId, const std::string& crewMemberId) {
    if (!confirmAdmin(adminId)) {
        return false;
    }
    return FlightService::removeCrewMemberFromFlight(flightId, crewMemberId);
}
/**
 * @brief Retrieves all crew members assigned to a specific flight
 * 
 * This method verifies admin authorization and fetches all crew members
 * associated with the specified flight ID. Only confirmed admins can
 * access this information.
 * 
 * @param adminId The unique identifier of the admin requesting the information
 * @param flightId The unique identifier of the flight to get crew members for
 * 
 * @return std::vector<std::shared_ptr<CrewMemberModel>> Vector containing 
 *         shared pointers to all crew members assigned to the flight.
 *         Returns empty vector if admin is not confirmed, flight doesn't exist,
 *         or no crew members are found.
 * 
 * @note This method performs admin authentication before processing the request
 * @note Invalid crew member IDs in the flight's crew list are silently skipped
 */
std::vector<std::shared_ptr<CrewMemberModel>> AdminController::getCrewMembersOfFlight(const std::string& adminId, const std::string& flightId) {
    if (!confirmAdmin(adminId)) {
        return {};
    }
    auto flightOpt = FlightService::getFlightById(flightId);
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
/**
 * @brief Adds a new aircraft to the repository if the admin is confirmed.
 *
 * This function creates a new AircraftModel instance with the specified model,
 * capacity, and number of row seats. It first verifies the admin's identity using
 * the provided adminId. If the admin is confirmed and the aircraft is successfully
 * added to the repository, it returns a shared pointer to the newly created AircraftModel.
 * Otherwise, it returns std::nullopt.
 *
 * @param adminId The identifier of the admin attempting to add the aircraft.
 * @param model The model name of the aircraft.
 * @param capacity The seating capacity of the aircraft.
 * @param numOfRowSeats The number of seats per row in the aircraft.
 * @return std::optional<std::shared_ptr<AircraftModel>> Shared pointer to the newly added AircraftModel,
 *         or std::nullopt if the admin is not confirmed or the aircraft could not be added.
 */
std::optional<std::shared_ptr<AircraftModel>> AdminController::addAircraft(
        const std::string& adminId,
        const std::string& model,
        int capacity,
        int numOfRowSeats
) {
    if (!confirmAdmin(adminId)) {
        return std::nullopt;
    }
    return AircraftService::addAircraft(model, capacity, numOfRowSeats);
}
/**
 * @brief Updates an existing aircraft's information in the system
 * 
 * This method allows an authenticated administrator to modify the details of an existing aircraft.
 * The method first verifies the admin's credentials, then retrieves the aircraft by ID,
 * updates its properties, and persists the changes through the AircraftService.
 * 
 * @param adminId The unique identifier of the administrator performing the update
 * @param aircraftId The unique identifier of the aircraft to be updated
 * @param model The new model name/designation for the aircraft
 * @param capacity The new passenger capacity of the aircraft
 * @param numOfRowSeats The new number of seats per row configuration
 * 
 * @return true if the aircraft was successfully updated, false if:
 *         - Admin authentication fails
 *         - Aircraft with the given ID doesn't exist
 *         - Update operation fails in the service layer
 * 
 * @throws May throw exceptions from underlying service calls
 */
bool AdminController::updateAircraft(
        const std::string& adminId,
        const std::string& aircraftId,
        const std::string& model,
        int capacity,
        int numOfRowSeats
) {
    if (!confirmAdmin(adminId)) {
        return false;
    }
    auto aircraftOpt = AircraftService::getAircraftById(aircraftId);
    if (!aircraftOpt.has_value()) {
        return false;
    }
    auto aircraft = aircraftOpt.value();
    aircraft->setModel(model);
    aircraft->setCapacity(capacity);
    aircraft->setNumOfRowSeats(numOfRowSeats);
    return AircraftService::updateAircraft(*aircraft);
}
/**
 * @brief Removes an aircraft from the repository if the admin is confirmed.
 *
 * This function checks whether the provided admin ID corresponds to a confirmed admin.
 * If the admin is confirmed, it attempts to delete the aircraft with the specified aircraft ID
 * from the AircraftRepository.
 *
 * @param adminId The ID of the admin requesting the removal.
 * @param aircraftId The ID of the aircraft to be removed.
 * @return true if the aircraft was successfully removed; false if the admin is not confirmed or the removal failed.
 */
bool AdminController::removeAircraft(const std::string& adminId, const std::string& aircraftId) {
    if (!confirmAdmin(adminId)) {
        return false;
    }
    return AircraftService::deleteAircraft(aircraftId);
}

/**
 * @brief Retrieves all aircraft models from the repository if the admin ID is confirmed.
 * 
 * This function checks whether the provided admin ID is valid and authorized.
 * If the admin ID is confirmed, it returns a vector containing shared pointers
 * to all aircraft models available in the repository. If the admin ID is not
 * confirmed, it returns an empty vector.
 * 
 * @param adminId The ID of the administrator requesting the aircraft models.
 * @return std::vector<std::shared_ptr<AircraftModel>> A vector of shared pointers to AircraftModel objects,
 *         or an empty vector if the admin ID is not confirmed.
 */
std::vector<std::shared_ptr<AircraftModel>> AdminController::getAllAircrafts(const std::string& adminId) {
    if (!confirmAdmin(adminId)) {
        return {};
    }
    return AircraftService::getAllAircrafts();
}
/**
 * @brief Retrieves an aircraft by its ID if the requesting user is an admin.
 * 
 * This function checks whether the provided admin ID corresponds to a valid admin.
 * If the admin is confirmed, it attempts to retrieve the aircraft with the specified aircraft ID.
 * Returns an optional containing a shared pointer to the AircraftModel if found, or std::nullopt otherwise.
 * 
 * @param adminId The ID of the admin requesting the aircraft information.
 * @param aircraftId The ID of the aircraft to retrieve.
 * @return std::optional<std::shared_ptr<AircraftModel>> An optional containing the aircraft model if found and the admin is confirmed; std::nullopt otherwise.
 */
std::optional<std::shared_ptr<AircraftModel>> AdminController::getAircraftById(const std::string& adminId, const std::string& aircraftId) {
    if (!confirmAdmin(adminId)) {
        return std::nullopt;
    }
    return AircraftService::getAircraftById(aircraftId);
}