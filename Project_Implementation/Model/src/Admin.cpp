#include "../include/Admin.hpp"
#include <stdexcept>
#include "../../Utils/include/IDGenerator.hpp"
#include "../../Repositories/include/UserRepository.hpp"

/**
 * @brief Constructs an Admin object and initializes its role as Admin.
 *
 * This constructor calls the base UserModel constructor and sets the role
 * of the user to UserModel::UserType::Admin.
 */
Admin::Admin() : UserModel() {
    this -> role = UserModel::UserType::Admin;
}
/**
 * @brief Constructs an Admin object with the specified username and password.
 *
 * This constructor initializes the Admin by calling the UserModel base class constructor
 * with the provided username, password, and sets the user type to Admin. It then generates
 * a unique user ID prefixed with "ADM-" using the IDGenerator. If the generated ID already
 * exists in the UserRepository, it continues generating new IDs until a unique one is found.
 *
 * @param username The username for the admin account.
 * @param password The password for the admin account.
 */
Admin::Admin(const std::string& username, const std::string& password) 
    : UserModel(username, password, UserModel::UserType::Admin) {
        std::string userId = "ADM-" + IDGenerator::generateUniqueID();
        auto userRepository = UserRepository::getInstance();
        while( userRepository -> findUserById(userId).has_value() ) {
            userId = "ADM-" + IDGenerator::generateUniqueID();
        }

        this -> userId = userId;
}
/**
 * @brief Constructs an Admin object from a JSON representation.
 *
 * This constructor initializes the Admin object using the provided JSON data.
 * It validates that the "role" field in the JSON is set to "Admin" and that
 * the user ID starts with "ADM-". If either validation fails, an
 * std::invalid_argument exception is thrown.
 *
 * @param json The JSON object containing Admin user data.
 * @throws std::invalid_argument If the "role" is not "Admin" or the user ID does not start with "ADM-".
 */
Admin::Admin(const JSON& json) : UserModel(json) {
    if (json.contains("role") && json["role"].get<std::string>() != "Admin") {
        throw std::invalid_argument("Invalid role for Admin user");
    }
    if (userId.substr(0, 4) != "ADM-") {
        throw std::invalid_argument("Invalid ID for Admin user");
    }
}

/**
 * @brief Serializes the Admin object to a JSON representation.
 *
 * This method populates the provided JSON object with the Admin's
 * user ID, username, password, and sets the role to "Admin".
 *
 * @param json Reference to a JSON object that will be populated with the Admin's data.
 */
void Admin::to_json(JSON& json) const {
    json = JSON {
        {"id", userId},
        {"username", username},
        {"password", password},
        {"role", "Admin"}
    };
}