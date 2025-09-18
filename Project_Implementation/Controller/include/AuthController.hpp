#pragma once

#include <string>
#include <memory>
#include <optional>
#include "../../Model/include/UserModel.hpp"

/**
 * @class AuthController
 * @brief Provides authentication-related operations for user login.
 *
 * This class is non-instantiable and serves as a static interface for user authentication.
 */
class AuthController {
    public:
        AuthController() = delete;

        static std::optional<std::shared_ptr<UserModel>> login(const std::string& username, const std::string& password);
};