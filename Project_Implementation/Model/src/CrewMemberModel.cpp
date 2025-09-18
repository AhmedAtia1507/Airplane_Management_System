#include "../include/CrewMemberModel.hpp"
#include "../../Utils/include/IDGenerator.hpp"
#include "../../Repositories/include/CrewMemberRepository.hpp"
#include <vector>
#include <stdexcept>

/**
 * @brief Constructs a CrewMemberModel object with the specified name and role.
 *
 * This constructor initializes a crew member with a given name and role.
 * It validates that the name is not empty, throwing std::invalid_argument if it is.
 * A unique crew ID is generated using IDGenerator, prefixed with "CM-".
 * The constructor ensures the generated crew ID does not already exist in the CrewMemberRepository.
 *
 * @param name The name of the crew member. Must not be empty.
 * @param role The role of the crew member, specified as a CrewType.
 *
 * @throws std::invalid_argument If the provided name is empty.
 */
CrewMemberModel::CrewMemberModel(const std::string& name, const CrewType& role)
    : name(name), role(role) {
        if (name.empty()) {
            throw std::invalid_argument("Crew member name cannot be empty.");
        }
        std::string crewId = "CM-" + IDGenerator::generateUniqueID();
        auto crewMemberRepository = CrewMemberRepository::getInstance();
        while ((crewMemberRepository -> findCrewMemberById(crewId)).has_value()) {
            crewId = "CM-" + IDGenerator::generateUniqueID();
        }
        (this -> crewId) = crewId;
}

/**
 * @brief Constructs a CrewMemberModel object from a JSON representation.
 *
 * This constructor initializes a CrewMemberModel instance using the provided JSON object.
 * It validates the presence of required tags ("id", "name", "role") and checks the format of the crew ID.
 * The crew ID must start with "CM-", and the name must not be empty.
 * The role must be either "Pilot" or "FlightAttendant"; otherwise, an exception is thrown.
 *
 * @param json The JSON object containing crew member data.
 * @throws std::invalid_argument If any required tag is missing, the crew ID format is invalid,
 *         the name is empty, or the role is not recognized.
 */
CrewMemberModel::CrewMemberModel(const JSON& json) {
    std::vector<std::string> requiredTags = {"id", "name", "role"};
    for ( const auto& tag : requiredTags ) {
        if (!json.contains(tag)) {
            throw std::invalid_argument("Invalid JSON for CrewMemberModel: missing tag '" + tag + "'.");
        }
    }
    
    crewId = json.at("id").get<std::string>();
    if (crewId.substr(0, 3) != "CM-") {
        throw std::invalid_argument("Invalid ID for CrewMemberModel");
    }
    name = json.at("name").get<std::string>();
    if(name.empty()) {
        throw std::invalid_argument("Crew member name cannot be empty.");
    }
    std::string roleStr = json.at("role").get<std::string>();
    if (roleStr == "Pilot") {
        role = CrewType::Pilot;
    } else if (roleStr == "Flight Attendant") {
        role = CrewType::FlightAttendant;
    } else {
        throw std::invalid_argument("Invalid role in JSON for CrewMemberModel");
    }
}

/**
 * @brief Serializes the CrewMemberModel object to a JSON representation.
 *
 * This method populates the provided JSON object with the crew member's ID, name,
 * and role. The role is converted to a string ("Pilot" or "FlightAttendant") based
 * on the CrewType enumeration.
 *
 * @param json Reference to a JSON object that will be populated with the crew member's data.
 */
void CrewMemberModel::to_json(JSON& json) const {
    json = JSON{
        {"id", crewId},
        {"name", name},
        {"role", (role == CrewType::Pilot) ? "Pilot" : "Flight Attendant"}
    };
}