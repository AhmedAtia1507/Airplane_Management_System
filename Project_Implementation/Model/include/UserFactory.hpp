#pragma once

#include "UserModel.hpp"
#include "Admin.hpp"
#include "Passenger.hpp"
#include "BookingManager.hpp"
#include "../../Third_Party/json.hpp"

using JSON = nlohmann::json;

#include <memory>
#include <string>

/**
 * @class UserFactory
 * @brief Factory class for creating UserModel instances.
 *
 * Provides static methods to instantiate UserModel objects either from a JSON object
 * or from individual user attributes.
 *
 * @note All methods return shared pointers to UserModel for memory management.
 */
class UserFactory {
public:
    UserFactory() = delete;
    static std::shared_ptr<UserModel> createUser(const JSON& json);
    static std::shared_ptr<UserModel> createUser(const std::string& username, const std::string& password, const UserModel::UserType& role, float loyaltyPoints = 0.0f);
};
