#include "../include/CrewMemberRepository.hpp"
#include "../../Utils/include/JSONManager.hpp"
#include "../../Utils/include/IDGenerator.hpp"
#include "../../Utils/include/DatabasePathResolver.hpp"
/**
 * @brief Path to the crew members database file.
 *
 * This constant defines the relative path to the JSON file
 * containing crew member data used by the repository.
 */
const std::string CREW_MEMBER_DATABASE_PATH = DatabasePathResolver::getDatabasePath() + "crew_members.json";

/**
 * @brief Constructs a CrewMemberRepository object and initializes the crewMembers collection.
 *
 * This constructor loads crew member data from the JSON database file specified by
 * CREW_MEMBER_DATABASE_PATH using the JSONManager::parseJSON function.
 */
CrewMemberRepository::CrewMemberRepository() {
    JSONManager::parseJSON(crewMembers, CREW_MEMBER_DATABASE_PATH);
}

/**
 * @brief Returns the singleton instance of CrewMemberRepository.
 *
 * Implements Meyers' Singleton pattern using a static local variable,
 * ensuring thread-safe and lazy initialization of the repository instance.
 *
 * @return std::shared_ptr<CrewMemberRepository> Shared pointer to the singleton instance.
 */
std::shared_ptr<CrewMemberRepository> CrewMemberRepository::getInstance() {
    // Meyers' Singleton: thread-safe and lazy-initialized.
    static std::shared_ptr<CrewMemberRepository> instance(new CrewMemberRepository());
    return instance;
}

/**
 * @brief Finds a crew member by their unique identifier.
 *
 * Searches the repository for a crew member with the specified ID.
 * If found, returns a shared pointer to the CrewMemberModel wrapped in an std::optional.
 * If not found, returns std::nullopt.
 *
 * @param crewId The unique identifier of the crew member to find.
 * @return std::optional<std::shared_ptr<CrewMemberModel>> 
 *         An optional containing a shared pointer to the CrewMemberModel if found, or std::nullopt if not found.
 */
std::optional<std::shared_ptr<CrewMemberModel>> CrewMemberRepository::findCrewMemberById(const std::string& crewId) const {
    if(crewMembers.find(crewId) == crewMembers.end()) {
        return std::nullopt;
    }

    return crewMembers.at(crewId);
}
/**
 * @brief Retrieves all crew members from the repository.
 * 
 * This method returns a vector containing shared pointers to all crew member
 * models currently stored in the repository. The returned vector contains
 * copies of the shared pointers, allowing safe access to the crew member
 * objects without affecting the repository's internal storage.
 * 
 * @return std::vector<std::shared_ptr<CrewMemberModel>> A vector containing
 *         shared pointers to all crew member models in the repository.
 *         Returns an empty vector if no crew members are stored.
 * 
 * @note This method is const-qualified and does not modify the repository state.
 * @note The returned shared pointers maintain reference counting for safe
 *       memory management.
 */
std::vector<std::shared_ptr<CrewMemberModel>> CrewMemberRepository::getAllCrewMembers() const {
    std::vector<std::shared_ptr<CrewMemberModel>> members;
    for ( const auto& [id, crewMember] : crewMembers ) {
        members.push_back(crewMember);
    }

    return members;
}
/**
 * @brief Finds and returns all crew members with the specified role.
 *
 * Iterates through the repository of crew members and collects those whose role matches
 * the provided CrewType. The result is returned as a vector of shared pointers
 * to CrewMemberModel objects. If no crew members match the role, an empty vector is returned.
 *
 * @param role The crew member role to search for.
 * @return std::vector<std::shared_ptr<CrewMemberModel>>
 *         A vector of shared pointers to CrewMemberModel objects that match the specified role.
 */
std::vector<std::shared_ptr<CrewMemberModel>> CrewMemberRepository::findCrewMembersByRole(const CrewMemberModel::CrewType& role) const {
    std::vector<std::shared_ptr<CrewMemberModel>> members;
    for ( const auto& [id, crewMember] : crewMembers ) {
        if( crewMember -> getRole() == role ) {
            members.push_back(crewMember);
        }
    }

    return members;
}
/**
 * @brief Adds a new crew member to the repository.
 *
 * This function attempts to add a new crew member to the internal collection.
 * If a crew member with the same ID already exists, the function returns false
 * and does not add the new crew member. Otherwise, the new crew member is added
 * and the function returns true.
 *
 * @param newCrewMember The crew member model to be added.
 * @return true if the crew member was successfully added; false if a crew member
 *         with the same ID already exists.
 */
bool CrewMemberRepository::addCrewMember(const CrewMemberModel& newCrewMember) {
    if ( crewMembers.find( newCrewMember.getCrewId() ) != crewMembers.end() ) {
        return false;
    }
    crewMembers[newCrewMember.getCrewId()] = std::make_shared<CrewMemberModel>(newCrewMember);
    return true;
}
/**
 * @brief Updates an existing crew member in the repository.
 *
 * This function checks if a crew member with the specified ID exists in the repository.
 * If the crew member exists, their information is updated with the provided data.
 * If the crew member does not exist, the function returns false and no update is performed.
 *
 * @param crewMember The CrewMemberModel object containing updated crew member information.
 * @return true if the crew member was successfully updated; false if the crew member does not exist.
 */
bool CrewMemberRepository::updateCrewMember(const CrewMemberModel& crewMember) {
    if ( crewMembers.find( crewMember.getCrewId() ) == crewMembers.end() ) {
        return false;
    }
    crewMembers[crewMember.getCrewId()] = std::make_shared<CrewMemberModel>(crewMember);
    return true;
}
/**
 * @brief Deletes a crew member from the repository by their ID.
 *
 * This function attempts to remove a crew member identified by the given crewId
 * from the internal collection. If the crew member does not exist, the function
 * returns false. Otherwise, it erases the crew member and returns true.
 *
 * @param crewId The unique identifier of the crew member to be deleted.
 * @return true if the crew member was found and deleted; false otherwise.
 */
bool CrewMemberRepository::deleteCrewMember(const std::string& crewId) {
    if ( crewMembers.find( crewId ) == crewMembers.end() ) {
        return false;
    }
    crewMembers.erase(crewId);
    return true;
}

/**
 * @brief Destructor for CrewMemberRepository.
 *
 * This destructor saves the current state of the crewMembers collection
 * to the JSON database file specified by CREW_MEMBER_DATABASE_PATH using
 * the JSONManager::saveToJSON function. It ensures that any changes made
 * to the repository during its lifetime are persisted to the database file
 * before the repository is destroyed.
 */
CrewMemberRepository::~CrewMemberRepository() {
    JSONManager::saveToJSON(crewMembers, CREW_MEMBER_DATABASE_PATH);
    crewMembers.clear();
}