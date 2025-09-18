#pragma once

#include "UserModel.hpp"

/**
 * @class Admin
 * @brief Represents an administrative user in the system, derived from UserModel.
 *
 * Provides constructors for creating an Admin instance from username/password or from a JSON object.
 * Supports serialization to JSON.
 *
 * @note This class overrides the to_json method from UserModel.
 */
class Admin : public UserModel {
    public:
        Admin();
        Admin(const std::string& username, const std::string& password);
        Admin(const JSON& json);

        void to_json(JSON& json) const override;

        ~Admin() = default;
};