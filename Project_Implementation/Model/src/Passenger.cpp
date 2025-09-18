#include "../include/Passenger.hpp"
#include "../../Utils/include/IDGenerator.hpp"
#include "../../Repositories/include/UserRepository.hpp"

/**
 * @brief Default constructor for the Passenger class.
 *
 * Initializes a Passenger object by calling the base UserModel constructor
 * and sets the user role to Passenger.
 */
Passenger::Passenger() : UserModel() {
    this -> role = UserModel::UserType::Passenger;
}
/**
 * @brief Constructs a Passenger object with the specified username, password, and loyalty points.
 *
 * This constructor initializes the Passenger by calling the UserModel base class constructor
 * with the provided username, password, and sets the user type to Passenger. It also generates
 * a unique user ID prefixed with "PAS-" using the IDGenerator utility. The constructor ensures
 * that the generated user ID does not already exist in the UserRepository by checking for uniqueness.
 * If a collision is detected, it continues generating new IDs until a unique one is found.
 *
 * @param username The username for the passenger.
 * @param password The password for the passenger.
 * @param loyaltyPoints The initial loyalty points assigned to the passenger.
 */
Passenger::Passenger(const std::string& username, const std::string& password, float loyaltyPoints) :
    UserModel(username, password, UserModel::UserType::Passenger), loyaltyPoints(loyaltyPoints) {
        std::string userId = "PAS-" + IDGenerator::generateUniqueID();
        auto userRepository = UserRepository::getInstance();
        while( userRepository -> findUserById(userId).has_value() ) {
            userId = "PAS-" + IDGenerator::generateUniqueID();
        }
        this -> userId = userId;
}

/**
 * @brief Constructs a Passenger object from a JSON representation.
 *
 * This constructor initializes a Passenger instance using the provided JSON object.
 * It validates the presence and value of the "loyaltyPoints" field, ensuring it is non-negative.
 * It also checks that the "role" field, if present, is set to "Passenger", and that the user ID
 * starts with the prefix "PAS-". If any validation fails, an std::invalid_argument exception is thrown.
 *
 * @param json The JSON object containing Passenger data.
 * @throws std::invalid_argument If required fields are missing, values are invalid, or constraints are not met.
 */
Passenger::Passenger(const JSON& json) : UserModel(json) {
    if(!json.contains("loyaltyPoints")) {
        throw std::invalid_argument("Error: Invalid JSON Content for Passenger Loyalty Points");
    }
    loyaltyPoints = json["loyaltyPoints"].get<float>();
    if (loyaltyPoints < 0) {
        throw std::invalid_argument("Error: Loyalty Points cannot be negative");
    }
    if (json.contains("role") && json["role"].get<std::string>() != "Passenger") {
        throw std::invalid_argument("Invalid role for Passenger user");
    }
    if (userId.substr(0, 4) != "PAS-") {
        throw std::invalid_argument("Invalid ID for Passenger user");
    }
}

/**
 * @brief Serializes the Passenger object to a JSON representation.
 *
 * This method populates the provided JSON object with the Passenger's data,
 * including user ID, username, password, role (set as "Passenger"), and loyalty points.
 *
 * @param json Reference to a JSON object that will be assigned the serialized Passenger data.
 */
void Passenger::to_json(JSON& json) const {
    json = JSON {
        {"id", userId},
        {"username", username},
        {"password", password},
        {"role", "Passenger"},
        {"loyaltyPoints", loyaltyPoints}
    };
}