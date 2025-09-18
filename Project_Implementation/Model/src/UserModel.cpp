#include "../include/UserModel.hpp"
#include "../../Utils/include/IDGenerator.hpp"
#include "../../Repositories/include/UserRepository.hpp"
#include <vector>
#include <stdexcept>

/**
 * @brief Constructs a UserModel object with the specified username, password, and role.
 *
 * Initializes the UserModel with the provided username, password, and user role.
 * Throws an exception if the username or password is empty, or if the role is invalid.
 *
 * @param username The username of the user. Must not be empty.
 * @param password The password of the user. Must not be empty.
 * @param role The role of the user. Must be one of UserType::Passenger, UserType::BookingManager, or UserType::Admin.
 *
 * @throws std::invalid_argument If the username or password is empty, or if the role is invalid.
 */
UserModel::UserModel(const std::string& username, const std::string& password, const UserType& role) : 
    userId(""), username(username), password(password), role(role) {
        if(username.empty() || password.empty()) {
            throw std::invalid_argument("Username and password cannot be empty.");
        }
        if(role != UserType::Passenger && role != UserType::BookingManager && role != UserType::Admin) {
            throw std::invalid_argument("Invalid user role provided.");
        }
}

/**
 * @brief Constructs a UserModel object from a JSON representation.
 *
 * This constructor initializes the UserModel instance using the provided JSON object.
 * It checks for the presence of required fields ("id", "username", "password", "role") and
 * throws std::invalid_argument if any are missing. The "role" field must be one of the
 * accepted values ("Passenger", "BookingManager", "Admin"); otherwise, an exception is thrown.
 * Extra fields in the JSON are ignored. If required fields are of unexpected types, an exception is thrown.
 *
 * @param json The JSON object containing user data.
 * @throws std::invalid_argument If required fields are missing, have unexpected types, or the role is invalid.
 */
UserModel::UserModel(const JSON& json) {
    std::vector<std::string> requiredTags = {"id", "username", "password", "role"};
    for ( const auto& tag : requiredTags ) {
        if (!json.contains(tag)) {
            throw std::invalid_argument("Error: Invalid JSON for UserModel: missing tag '" + tag + "'.");
        }
    }
    
    userId = json["id"].get<std::string>();
    username = json["username"].get<std::string>();
    password = json["password"].get<std::string>();

    std::string roleStr;
    if (json["role"].is_string()) {
        roleStr = json["role"].get<std::string>();
    } else {
        roleStr = json["role"].dump();
    }

    if(roleStr == "Passenger") {
        role = UserType::Passenger;
    } else if(roleStr == "BookingManager") {
        role = UserType::BookingManager;
    } else if(roleStr == "Admin") {
        role = UserType::Admin;
    } else {
        throw std::invalid_argument("Error: Invalid User Role in JSON Content");
    }
}