#include "../include/UserRepository.hpp"
#include "../../Utils/include/JSONManager.hpp"
#include "../../Model/include/UserFactory.hpp"
#include "../../Utils/include/DatabasePathResolver.hpp"

/**
 * @brief Path to the user database file.
 *
 * This constant defines the relative path to the JSON file
 * that stores user information for the application.
 */
const std::string USER_DATABASE_PATH = DatabasePathResolver::getDatabasePath() + "users.json";

/**
 * @brief Constructs a UserRepository object and initializes user data.
 *
 * This constructor parses the user database JSON file and populates the internal
 * users map. It also builds a mapping from usernames to user IDs for efficient lookup.
 */
UserRepository::UserRepository() {
    JSONManager::parseJSON(users, USER_DATABASE_PATH);
    for (const auto& [id, user] : users) {
        usernameToIdMap[user->getUsername()] = id;
    }
}

/**
 * @brief Returns the singleton instance of UserRepository.
 *
 * This method implements the Meyers' Singleton pattern using a static local variable.
 * In C++11 and later, the initialization of the static variable is guaranteed to be thread-safe.
 *
 * @return std::shared_ptr<UserRepository> Shared pointer to the singleton UserRepository instance.
 */
std::shared_ptr<UserRepository> UserRepository::getInstance() {
    // Using a static local variable for the singleton instance (Meyers' Singleton).
    // C++11 and later guarantees that this is initialized only once and is thread-safe.
    static std::shared_ptr<UserRepository> instance(new UserRepository());
    return instance;
}

/**
 * @brief Finds a user by their unique identifier.
 *
 * Searches the repository for a user with the specified userId.
 * If the user exists, returns a shared pointer to the UserModel wrapped in std::optional.
 * If the user does not exist, returns std::nullopt.
 *
 * @param userId The unique identifier of the user to find.
 * @return std::optional<std::shared_ptr<UserModel>> Optional containing the user if found, or std::nullopt if not found.
 */
std::optional<std::shared_ptr<UserModel>> UserRepository::findUserById(const std::string& userId) const {
    if(users.find(userId) == users.end()) {
        return std::nullopt;
    }

    return users.at(userId);
}

/**
 * @brief Finds a user by their username.
 *
 * Searches for the specified username in the internal mapping. If the username exists,
 * retrieves the corresponding user model by its ID. If the username does not exist,
 * returns an empty optional.
 *
 * @param username The username to search for.
 * @return std::optional<std::shared_ptr<UserModel>> A shared pointer to the user model if found, otherwise std::nullopt.
 */
std::optional<std::shared_ptr<UserModel>> UserRepository::findUserByUsername(const std::string& username) const {
    auto it = usernameToIdMap.find(username);
    if (it == usernameToIdMap.end()) {
        return std::nullopt;
    }
    return findUserById(it -> second);
}

/**
 * @brief Adds a new user to the repository.
 *
 * This function attempts to add a new user to the repository. It checks for duplicate user IDs
 * and usernames to ensure uniqueness. If a user with the same ID or username already exists,
 * the function returns false and does not add the user. The user is created using a factory
 * to ensure the correct derived type is instantiated. If user creation fails, the function
 * returns false.
 *
 * @param newUser The user model to be added.
 * @return true if the user was successfully added; false otherwise.
 */
bool UserRepository::addUser(const UserModel& newUser) {
    if (users.find(newUser.getUserId()) != users.end()) {
        // User with the same ID already exists; do not add and return false.
        return false;
    }
    if (usernameToIdMap.find(newUser.getUsername()) != usernameToIdMap.end()) {
        // Username already taken by another user
        return false;
    }
    // Use the factory to create a polymorphic copy of the correct derived type.
    JSON userJson;
    newUser.to_json(userJson);
    auto createdUser = UserFactory::createUser(userJson);
    if (!createdUser) {
        return false;
    }
    users[newUser.getUserId()] = createdUser;
    usernameToIdMap[newUser.getUsername()] = newUser.getUserId();
    return true;
}
/**
 * @brief Updates an existing user in the repository.
 *
 * This method attempts to update the user information for the user with the given user ID.
 * If the username is being changed, it ensures that the new username is not already taken by another user.
 * The username-to-ID lookup map is updated accordingly to maintain consistency.
 *
 * @param user The UserModel object containing updated user information.
 * @return true if the update was successful; false if the user does not exist or the new username is already taken.
 */
bool UserRepository::updateUser(const UserModel& user) {
    auto it = users.find(user.getUserId());
    if(it == users.end()) {
        return false;
    }

    if (it->second->getUsername() != user.getUsername()) {
        // Check if the new username already exists and is not the current user
        auto usernameIt = usernameToIdMap.find(user.getUsername());
        if (usernameIt != usernameToIdMap.end() && usernameIt->second != user.getUserId()) {
            // Username already taken by another user
            return false;
        }
        usernameToIdMap.erase(it->second->getUsername());
    }

    JSON userJson;
    user.to_json(userJson);
    users[user.getUserId()] = UserFactory::createUser(userJson);
    usernameToIdMap[user.getUsername()] = user.getUserId();
    return true;
}
/**
 * @brief Deletes a user from the repository by their user ID.
 *
 * This function searches for a user with the specified user ID in the repository.
 * If the user is found, it removes the user from both the internal user map and
 * the username-to-ID mapping. If the user is not found, the function returns false.
 *
 * @param userId The unique identifier of the user to be deleted.
 * @return true if the user was found and deleted; false otherwise.
 */
bool UserRepository::deleteUser(const std::string& userId) {
    auto it = users.find(userId);
    if(it == users.end()) {
        return false;
    }
    auto username = it->second->getUsername();
    usernameToIdMap.erase(username);
    users.erase(it);
    return true;
}

/**
 * @brief Retrieves all users stored in the repository.
 *
 * This method returns a vector containing shared pointers to all UserModel
 * instances currently managed by the repository. The returned vector reflects
 * the current state of the internal user storage.
 *
 * @return std::vector<std::shared_ptr<UserModel>> A vector of shared pointers to UserModel objects.
 */
std::vector<std::shared_ptr<UserModel>> UserRepository::getAllUsers() const {
    std::vector<std::shared_ptr<UserModel>> allUsers;
    for (const auto& [id, user] : users) {
        allUsers.push_back(user);
    }
    return allUsers;
}
/**
 * @brief Retrieves all users with a specific role from the repository.
 * 
 * This function filters the users collection and returns all users that match
 * the specified role type. The search is performed by comparing each user's
 * role with the provided role parameter.
 * 
 * @param role The user role type to filter by (UserModel::UserType)
 * @return std::vector<std::shared_ptr<UserModel>> A vector containing shared pointers
 *         to all users that have the specified role. Returns an empty vector if no
 *         users with the given role are found.
 * 
 * @note This function is const and does not modify the repository state.
 * @see UserModel::UserType for available role types
 * @see UserModel::getRole() for role retrieval method
 */
std::vector<std::shared_ptr<UserModel>> UserRepository::getUsersByRole(const UserModel::UserType& role) const {
    std::vector<std::shared_ptr<UserModel>> filteredUsers;
    for (const auto& [id, user] : users) {
        if (user->getRole() == role) {
            filteredUsers.push_back(user);
        }
    }
    return filteredUsers;
}
/**
 * @brief Destructor for the UserRepository class.
 *
 * This destructor is responsible for saving the current state of the user repository
 * to a JSON file before the object is destroyed. It ensures that any changes made
 * to the users are persisted to the file specified by USER_DATABASE_PATH.
 */
UserRepository::~UserRepository() {
    JSONManager::saveToJSON(users, USER_DATABASE_PATH);
    users.clear();
    usernameToIdMap.clear();
}