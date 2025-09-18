#pragma once

#include "UserModel.hpp"

/**
 * @class BookingManager
 * @brief Manages booking-related operations and inherits user management functionality from UserModel.
 *
 * The BookingManager class provides constructors for initializing booking manager instances
 * with username and password, or from a JSON object. It also supports serialization to JSON.
 */
class BookingManager : public UserModel {
    public:
        BookingManager();
        BookingManager(const std::string& username, const std::string& password);
        BookingManager(const JSON& json);

        void to_json(JSON& json) const override;

        ~BookingManager() = default;
};