#pragma once

#include <string>
#include <optional>
#include <memory>
#include "../../Model/include/UserModel.hpp"
#include <vector>

/**
 * @brief Service class for managing user operations in the airplane management system.
 * 
 * This class provides static methods for user authentication, CRUD operations,
 * and role-based user management. All methods are static and the class cannot
 * be instantiated.
 */
class UserManagementService {
    public:
        UserManagementService() = delete;
        static UserModel::UserType getUserRole(const std::string& userId);
        static std::optional<std::shared_ptr<UserModel>> authenticateUser (
            const std::string& username,
            const std::string& password
        );
        static std::vector<std::shared_ptr<UserModel>> getAllUsers();
        static std::vector<std::shared_ptr<UserModel>> getUsersByRole(const UserModel::UserType& role);
        static std::optional<std::shared_ptr<UserModel>> createUser (
            const std::string& username,
            const std::string& password,
            const UserModel::UserType& role
        );
        static std::optional<std::shared_ptr<UserModel>> getUserById (const std::string& userId);
        static bool updateUser (const UserModel& user);
        static bool deleteUser (const std::string& userId);
        static bool updateUserPassword(const std::string& userId, const std::string& newPassword);
};