#include "../include/UserManagementService.hpp"
#include "../../Repositories/include/UserRepository.hpp"
#include "../../Model/include/UserFactory.hpp"

/**
 * @brief Retrieves the role/type of a user by their unique identifier
 * 
 * This method queries the user repository to find a user by their ID and returns
 * their associated role/type. If the user is not found in the repository, it
 * returns an INVALID user type.
 * 
 * @param userId The unique string identifier of the user whose role is to be retrieved
 * @return UserModel::UserType The role/type of the user if found, otherwise UserModel::UserType::INVALID
 * 
 * @note This method uses the singleton instance of UserRepository to perform the lookup
 * @see UserRepository::findUserById()
 * @see UserModel::UserType
 */
UserModel::UserType UserManagementService::getUserRole(const std::string& userId) {
    auto userOpt = UserRepository::getInstance() -> findUserById(userId);
    if (userOpt.has_value()) {
        return userOpt.value() -> getRole();
    }
    return UserModel::UserType::INVALID;
}

/**
 * @brief Attempts to authenticate a user with the provided username and password.
 *
 * This function searches for a user by username using the UserRepository.
 * If a user is found and the password matches, it returns an optional containing a shared pointer to the UserModel.
 * If authentication fails, it returns std::nullopt.
 *
 * @param username The username of the user attempting to log in.
 * @param password The password provided for authentication.
 * @return std::optional<std::shared_ptr<UserModel>> An optional containing a shared pointer to the authenticated UserModel, or std::nullopt if authentication fails.
 */
std::optional<std::shared_ptr<UserModel>> UserManagementService::authenticateUser(const std::string& username, const std::string& password) {
    auto userOpt = UserRepository::getInstance() -> findUserByUsername(username);
    if ((userOpt.has_value()) && (userOpt.value() -> getPassword() == password)) {
        return userOpt;
    }
    return std::nullopt;
}
/**
 * @brief Retrieves a list of all users in the system.
 *
 * This method fetches all user records from the underlying user repository
 * and returns them as a vector of shared pointers to UserModel objects.
 *
 * @return std::vector<std::shared_ptr<UserModel>> A vector containing shared pointers to all UserModel instances.
 */
std::vector<std::shared_ptr<UserModel>> UserManagementService::getAllUsers() {
    return UserRepository::getInstance() -> getAllUsers();
}

/**
 * @brief Creates a new user with the specified username, password, and role.
 *
 * This function utilizes the UserFactory to instantiate a new UserModel object,
 * adds the created user to the UserRepository, and returns a shared pointer to the new user.
 *
 * @param username The username for the new user.
 * @param password The password for the new user.
 * @param role The role/type of the user (as defined in UserModel::UserType).
 * @return std::optional<std::shared_ptr<UserModel>> An optional containing a shared pointer to the newly created user.
 */
std::optional<std::shared_ptr<UserModel>> UserManagementService::createUser(
    const std::string& username,
    const std::string& password,
    const UserModel::UserType& role
) {
    auto newUser = UserFactory::createUser(username, password, role);
    if(UserRepository::getInstance() -> addUser(*newUser)) {
        return UserRepository::getInstance() -> findUserById(newUser -> getUserId());
    }
    return std::nullopt;
}

/**
 * @brief Retrieves a user by their unique identifier.
 *
 * This method attempts to find a user in the repository using the provided user ID.
 * If a user with the specified ID exists, a shared pointer to the UserModel is returned
 * wrapped in an std::optional. If no user is found, std::nullopt is returned.
 *
 * @param userId The unique identifier of the user to retrieve.
 * @return std::optional<std::shared_ptr<UserModel>> An optional containing a shared pointer
 *         to the UserModel if found, or std::nullopt if not found.
 */
std::optional<std::shared_ptr<UserModel>> UserManagementService::getUserById(const std::string& userId) {
    auto userOpt = UserRepository::getInstance() -> findUserById(userId);
    if (userOpt.has_value()) {
        return userOpt;
    }
    return std::nullopt;
}

/**
 * @brief Updates the information of an existing user.
 *
 * This method attempts to update the details of the specified user in the repository.
 *
 * @param user The UserModel object containing updated user information.
 * @return true if the user was successfully updated; false otherwise.
 */
bool UserManagementService::updateUser(const UserModel& user) {
    return UserRepository::getInstance() -> updateUser(user);
}

/**
 * @brief Deletes a user with the specified user ID.
 *
 * This function attempts to remove the user identified by the given userId
 * from the user repository. It delegates the deletion operation to the
 * UserRepository singleton instance.
 *
 * @param userId The unique identifier of the user to be deleted.
 * @return true if the user was successfully deleted; false otherwise.
 */
bool UserManagementService::deleteUser(const std::string& userId) {
    return UserRepository::getInstance() -> deleteUser(userId);
}
/**
 * @brief Updates the password of a user identified by userId.
 *
 * This method attempts to find the user with the specified userId.
 * If the user exists, their password is updated to the provided newPassword,
 * and the change is persisted in the user repository.
 *
 * @param userId The unique identifier of the user whose password is to be updated.
 * @param newPassword The new password to set for the user.
 * @return true if the password was successfully updated; false if the user does not exist or the update failed.
 */
bool UserManagementService::updateUserPassword(const std::string& userId, const std::string& newPassword) {
    auto userOpt = UserRepository::getInstance() -> findUserById(userId);
    if (userOpt.has_value()) {
        auto user = userOpt.value();
        user -> setPassword(newPassword);
        return UserRepository::getInstance() -> updateUser(*user);
    }
    return false;
}
/**
 * @brief Retrieves all users with a specific role from the repository.
 * 
 * This method queries the UserRepository to fetch all users that have
 * the specified role/user type. It acts as a service layer wrapper
 * around the repository's getUsersByRole functionality.
 * 
 * @param role The user role/type to filter by (UserModel::UserType enum)
 * @return std::vector<std::shared_ptr<UserModel>> A vector containing 
 *         shared pointers to all UserModel objects that match the specified role.
 *         Returns an empty vector if no users with the given role are found.
 * 
 * @note This method delegates the actual data retrieval to the UserRepository
 *       singleton instance and does not perform any additional business logic.
 */
std::vector<std::shared_ptr<UserModel>> UserManagementService::getUsersByRole(const UserModel::UserType& role) {
    return UserRepository::getInstance() -> getUsersByRole(role);
}