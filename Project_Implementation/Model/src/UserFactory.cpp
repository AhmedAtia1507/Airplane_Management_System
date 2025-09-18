#include "../include/UserFactory.hpp"
#include <stdexcept>

/**
 * @brief Creates a user object based on the provided JSON data.
 *
 * This factory method examines the "role" field in the input JSON and constructs
 * an appropriate subclass of UserModel (Passenger, BookingManager, or Admin).
 *
 * @param json The JSON object containing user data, including the "role" field.
 * @return std::shared_ptr<UserModel> A shared pointer to the created user object.
 *
 * @throws std::invalid_argument If the "role" field does not match any known user type.
 */
std::shared_ptr<UserModel> UserFactory::createUser(const JSON& json) {
    std::string userType = json["role"].get<std::string>();

    if(!userType.compare("Passenger")) {
        return std::make_shared<Passenger>(json);
    }
    else if(!userType.compare("BookingManager")) {
        return std::make_shared<BookingManager>(json);
    }
    else if(!userType.compare("Admin")) {
        return std::make_shared<Admin>(json);
    }
    else {
        throw std::invalid_argument("Unknown user type: " + userType);
    }
}

/**
 * @brief Creates a user object based on the specified user type.
 *
 * This factory method instantiates and returns a shared pointer to a derived UserModel object
 * corresponding to the provided user type. For Passenger users, loyalty points are also set.
 *
 * @param username The username for the new user.
 * @param password The password for the new user.
 * @param role The type of user to create (Passenger, BookingManager, Admin).
 * @param loyaltyPoints The loyalty points for the user (used only for Passenger type).
 * @return std::shared_ptr<UserModel> A shared pointer to the created user object.
 * @throws std::invalid_argument If the user type is unknown.
 */
std::shared_ptr<UserModel> UserFactory::createUser(const std::string& username, const std::string& password, const UserModel::UserType& role, float loyaltyPoints) {
    if(role == UserModel::UserType::Passenger) {
        return std::make_shared<Passenger>(username, password, loyaltyPoints);
    }
    else if(role == UserModel::UserType::BookingManager) {
        return std::make_shared<BookingManager>(username, password);
    }
    else if(role == UserModel::UserType::Admin) {
        return std::make_shared<Admin>(username, password);
    }
    else {
        throw std::invalid_argument("Unknown user type: " + std::to_string(static_cast<int>(role)));
    }
}