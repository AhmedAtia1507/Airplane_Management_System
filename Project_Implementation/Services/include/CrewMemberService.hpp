#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../../Model/include/CrewMemberModel.hpp"


/**
 * @brief Service class for managing crew member operations in the airplane management system.
 * 
 * This class provides static methods for CRUD operations on crew members including
 * creation, retrieval, updating, and deletion. It serves as an interface between
 * the application logic and the crew member data layer.
 * 
 * @note This class cannot be instantiated as the default constructor is deleted.
 *       All operations are performed through static methods.
 */
class CrewMemberService {
    public:
        CrewMemberService() = delete;

        static std::optional<std::shared_ptr<CrewMemberModel>> createCrewMember(const std::string& name, const CrewMemberModel::CrewType& role);
        static std::optional<std::shared_ptr<CrewMemberModel>> getCrewMemberById(const std::string& id);
        static bool updateCrewMember(const std::string& id, const std::string& name, const CrewMemberModel::CrewType& role);
        static bool deleteCrewMember(const std::string& id);
        static std::vector<std::shared_ptr<CrewMemberModel>> getAllCrewMembers();
        static std::vector<std::shared_ptr<CrewMemberModel>> getCrewMembersByRole(const CrewMemberModel::CrewType& role);
};