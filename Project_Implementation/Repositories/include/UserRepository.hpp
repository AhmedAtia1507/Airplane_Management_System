#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "../../Model/include/UserModel.hpp"

/**
 * @class UserRepository
 * @brief Singleton repository for managing user data.
 *
 * Maintains a collection of users and provides methods for querying,
 * adding, updating, and deleting user records. Uses an unordered map
 * for efficient lookup by user ID and username.
 *
 * Copy and move operations are deleted to enforce singleton pattern.
 */
class UserRepository {
    std::unordered_map<std::string, std::shared_ptr<UserModel>> users;
    std::unordered_map<std::string, std::string> usernameToIdMap;
    
    UserRepository();
    UserRepository(const UserRepository&) = delete;
    UserRepository& operator=(const UserRepository&) = delete;
    UserRepository(UserRepository&&) = delete;
    UserRepository& operator=(UserRepository&&) = delete;


    public:
        static std::shared_ptr<UserRepository> getInstance();
        std::optional<std::shared_ptr<UserModel>> findUserById(const std::string& userId) const;
        std::optional<std::shared_ptr<UserModel>> findUserByUsername(const std::string& username) const;
        std::vector<std::shared_ptr<UserModel>> getAllUsers() const;
        std::vector<std::shared_ptr<UserModel>> getUsersByRole(const UserModel::UserType& role) const;
        bool addUser(const UserModel& newUser);
        bool updateUser(const UserModel& user);
        bool deleteUser(const std::string& userId);

        ~UserRepository();
};
