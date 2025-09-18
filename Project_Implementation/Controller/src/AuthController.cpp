#include "../include/AuthController.hpp"
#include "../../Services/include/UserManagementService.hpp"

/**
 * @brief Attempts to authenticate a user with the provided username and password.
 *
 * This function delegates the authentication process to the UserManagementService.
 * If authentication is successful, it returns an optional containing a shared pointer
 * to the authenticated UserModel. If authentication fails, it returns an empty optional.
 *
 * @param username The username of the user attempting to log in.
 * @param password The password of the user attempting to log in.
 * @return std::optional<std::shared_ptr<UserModel>> An optional containing the authenticated user model,
 *         or std::nullopt if authentication fails.
 */
std::optional<std::shared_ptr<UserModel>> AuthController::login(const std::string& username, const std::string& password) {
    return UserManagementService::authenticateUser(username, password);
}
