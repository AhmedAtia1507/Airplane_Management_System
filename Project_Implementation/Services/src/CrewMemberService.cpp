#include "../include/CrewMemberService.hpp"
#include "../../Repositories/include/CrewMemberRepository.hpp"

/**
 * @brief Creates a new crew member with the specified name and role.
 *
 * This function constructs a CrewMemberModel object and attempts to add it to the CrewMemberRepository.
 * If the addition is successful, it returns a shared pointer to the created CrewMemberModel wrapped in std::optional.
 * If the addition fails, it returns std::nullopt.
 *
 * @param name The name of the crew member to create.
 * @param role The role assigned to the crew member.
 * @return std::optional<std::shared_ptr<CrewMemberModel>> 
 *         A shared pointer to the created CrewMemberModel if successful, otherwise std::nullopt.
 */
std::optional<std::shared_ptr<CrewMemberModel>> CrewMemberService::createCrewMember(const std::string& name, const CrewMemberModel::CrewType& role) {
    CrewMemberModel crewMember (name, role);
    if ( CrewMemberRepository::getInstance() -> addCrewMember(crewMember) ) {
        return CrewMemberRepository::getInstance() -> findCrewMemberById(crewMember.getCrewId());
    }
    return std::nullopt;
}
/**
 * @brief Updates the name and role of a crew member identified by the given ID.
 *
 * This function searches for a crew member using the provided ID. If the crew member exists,
 * it updates their name and role with the specified values and saves the changes to the repository.
 *
 * @param id The unique identifier of the crew member to update.
 * @param name The new name to assign to the crew member.
 * @param role The new role to assign to the crew member.
 * @return true if the crew member was found and successfully updated; false otherwise.
 */
bool CrewMemberService::updateCrewMember(const std::string& id, const std::string& name, const CrewMemberModel::CrewType& role) {
    auto crewMember = CrewMemberRepository::getInstance() -> findCrewMemberById(id);
    if ( crewMember ) {
        crewMember.value() -> setName(name);
        crewMember.value() -> setRole(role);
        return CrewMemberRepository::getInstance() -> updateCrewMember(*crewMember.value());
    }
    return false;
}
/**
 * @brief Deletes a crew member with the specified ID.
 *
 * This function attempts to remove a crew member from the repository
 * using the provided unique identifier. It delegates the deletion
 * operation to the CrewMemberRepository singleton instance.
 *
 * @param id The unique identifier of the crew member to be deleted.
 * @return true if the crew member was successfully deleted; false otherwise.
 */
bool CrewMemberService::deleteCrewMember(const std::string& id) {
    return CrewMemberRepository::getInstance() -> deleteCrewMember(id);
}
/**
 * @brief Retrieves all crew members from the repository.
 *
 * This method fetches a list of all crew members by delegating the call
 * to the CrewMemberRepository singleton instance. Each crew member is
 * represented as a shared pointer to a CrewMemberModel object.
 *
 * @return std::vector<std::shared_ptr<CrewMemberModel>>
 *         A vector containing shared pointers to all crew member models.
 */
std::vector<std::shared_ptr<CrewMemberModel>> CrewMemberService::getAllCrewMembers() {
    return CrewMemberRepository::getInstance() -> getAllCrewMembers();
}
/**
 * @brief Retrieves a list of crew members filtered by their role.
 * 
 * This function queries the CrewMemberRepository for crew members that match the specified role.
 * 
 * @param role The role (CrewType) to filter crew members by.
 * @return std::vector<std::shared_ptr<CrewMemberModel>> 
 *         A vector containing shared pointers to CrewMemberModel instances that match the given role.
 */
std::vector<std::shared_ptr<CrewMemberModel>> CrewMemberService::getCrewMembersByRole(const CrewMemberModel::CrewType& role) {
    return CrewMemberRepository::getInstance() -> findCrewMembersByRole(role);
}

/**
 * @brief Retrieves a crew member by their unique identifier.
 * 
 * This method queries the crew member repository to find and return a crew member
 * with the specified ID. If no crew member is found with the given ID, an empty
 * optional is returned.
 * 
 * @param id The unique identifier of the crew member to retrieve
 * @return std::optional<std::shared_ptr<CrewMemberModel>> A shared pointer to the
 *         crew member model wrapped in an optional if found, otherwise std::nullopt
 */
std::optional<std::shared_ptr<CrewMemberModel>> CrewMemberService::getCrewMemberById(const std::string& id) {
    return CrewMemberRepository::getInstance() -> findCrewMemberById(id);
}