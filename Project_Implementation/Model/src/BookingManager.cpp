#include "../include/BookingManager.hpp"
#include "../../Utils/include/IDGenerator.hpp"
#include "../../Repositories/include/UserRepository.hpp"

/**
 * @brief Constructs a BookingManager object and initializes its role.
 *
 * This constructor initializes the BookingManager by calling the base UserModel
 * constructor and sets the role to UserType::BookingManager.
 */
BookingManager::BookingManager() : UserModel() {
    this -> role = UserModel::UserType::BookingManager;
}
/**
 * @brief Constructs a BookingManager object with the specified username and password.
 *
 * Initializes the BookingManager by calling the UserModel constructor with the provided
 * credentials and the BookingManager user type. Generates a unique user ID prefixed with "BM-"
 * and ensures its uniqueness by checking against existing user IDs in the UserRepository.
 *
 * @param username The username for the BookingManager.
 * @param password The password for the BookingManager.
 */
BookingManager::BookingManager(const std::string& username, const std::string& password) :
    UserModel(username, password, UserModel::UserType::BookingManager) {
        std::string userId = "BM-" + IDGenerator::generateUniqueID();
        auto userRepository = UserRepository::getInstance();
        while (userRepository->findUserById(userId).has_value()) {
            userId = "BM-" + IDGenerator::generateUniqueID();
        }
        this->userId = userId;
}
/**
 * @brief Constructs a BookingManager object from a JSON representation.
 *
 * This constructor initializes a BookingManager instance using the provided JSON object.
 * It validates that the "role" field in the JSON is set to "BookingManager" and that the
 * user ID has the correct prefix ("BM-"). If either validation fails, an std::invalid_argument
 * exception is thrown.
 *
 * @param json The JSON object containing user data for initialization.
 * @throws std::invalid_argument If the "role" is not "BookingManager" or the user ID does not start with "BM-".
 */
BookingManager::BookingManager(const JSON& json) : UserModel(json) {
    if (json.contains("role") && json["role"].get<std::string>() != "BookingManager") {
        throw std::invalid_argument("Invalid role for BookingManager user");
    }
    if (userId.substr(0, 3) != "BM-") {
        throw std::invalid_argument("Invalid ID for BookingManager user");
    }
}

/**
 * @brief Serializes the BookingManager object to a JSON representation.
 *
 * This method populates the provided JSON object with the BookingManager's
 * user ID, username, password, and role information.
 *
 * @param json Reference to a JSON object that will be populated with the BookingManager's data.
 */
void BookingManager::to_json(JSON& json) const {
    json = JSON {
        {"id", userId},
        {"username", username},
        {"password", password},
        {"role", "BookingManager"}
    };
}