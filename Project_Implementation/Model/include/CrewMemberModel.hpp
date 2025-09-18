#pragma once
#include <string>
#include "../../Third_Party/json.hpp"

using JSON = nlohmann::json;

/**
 * @class CrewMemberModel
 * @brief Represents a crew member in the system, such as a pilot or flight attendant.
 *
 * This class encapsulates the properties and behaviors of a crew member, including their unique ID,
 * name, and role within the crew. It provides constructors for initialization from parameters or JSON,
 * as well as getter and setter methods for its attributes.
 *
 * @enum CrewType
 *      Defines the type of crew member:
 *          - Pilot
 *          - FlightAttendant
 *
 * @note The class supports serialization to JSON.
 */
class CrewMemberModel {
    public:
        enum class CrewType {
            Pilot,
            FlightAttendant
        };
    private:
        std::string crewId;
        std::string name;
        CrewType role;

    public:
        CrewMemberModel() = default;
        CrewMemberModel(const std::string& name, const CrewType& role);
        CrewMemberModel(const JSON& json);

        inline void setCrewId(const std::string& id)    { crewId = id; }
        inline void setName(const std::string& name)    { this->name = name; }
        inline void setRole(const CrewType& role)       { this->role = role; }

        inline std::string getCrewId() const            { return crewId; }
        inline std::string getName() const              { return name; }
        inline CrewType getRole() const                 { return role; }

        void to_json(JSON& json) const;

        ~CrewMemberModel() = default;
};