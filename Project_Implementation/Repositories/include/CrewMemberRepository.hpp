#pragma once
#include <string>
#include "../../Third_Party/json.hpp"
#include "../../Model/include/CrewMemberModel.hpp"

using JSON = nlohmann::json;

/**
 * @class CrewMemberRepository
 * @brief Singleton repository for managing CrewMemberModel instances.
 *
 * Provides methods to add, update, delete, and query crew members by ID or role.
 * Utilizes an internal unordered_map for efficient storage and retrieval.
 * Copy and move operations are disabled to enforce singleton usage.
 */
class CrewMemberRepository {
    std::unordered_map<std::string, std::shared_ptr<CrewMemberModel>> crewMembers;

    CrewMemberRepository();
    CrewMemberRepository(const CrewMemberRepository&) = delete;
    CrewMemberRepository& operator=(const CrewMemberRepository&) = delete;
    CrewMemberRepository(CrewMemberRepository&&) = delete;
    CrewMemberRepository& operator=(CrewMemberRepository&&) = delete;

    public:
        static std::shared_ptr<CrewMemberRepository> getInstance();
        std::optional<std::shared_ptr<CrewMemberModel>> findCrewMemberById(const std::string& crewId) const;
        std::vector<std::shared_ptr<CrewMemberModel>> findCrewMembersByRole(const CrewMemberModel::CrewType& role) const;
        std::vector<std::shared_ptr<CrewMemberModel>> getAllCrewMembers() const;
        bool addCrewMember(const CrewMemberModel& newCrewMember);
        bool updateCrewMember(const CrewMemberModel& crewMember);
        bool deleteCrewMember(const std::string& crewId);
        ~CrewMemberRepository();
};