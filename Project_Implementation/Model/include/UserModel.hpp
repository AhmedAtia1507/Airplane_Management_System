#pragma once

#include <string>
#include "../../Third_Party/json.hpp"

using JSON = nlohmann::json;

/**
 * @class UserModel
 * @brief Abstract base class representing a user in the system.
 *
 * Provides common attributes and methods for different user types, such as Passenger,
 * BookingManager, and Admin. Supports construction from parameters or JSON, and enforces
 * serialization via the pure virtual to_json method.
 *
 * @enum UserType
 *      Passenger       - Regular user who can book tickets.
 *      BookingManager  - User responsible for managing bookings.
 *      Admin           - User with administrative privileges.
 *      INVALID         - User role is not recognized.
 *
 * @protected
 *      std::string userId    - Unique identifier for the user.
 *      std::string username  - Username for authentication.
 *      std::string password  - Password for authentication.
 *      UserType role         - Role of the user.
 *
 * @public
 *      UserModel() - Default constructor.
 *      UserModel(const std::string& username, const std::string& password, const UserType& role) - Parameterized constructor.
 *      UserModel(const JSON& json) - Constructs a user from a JSON object.
 *
 *      void setUserId(const std::string& userId)     - Sets the user ID.
 *      void setUserName(const std::string& username) - Sets the username.
 *      void setPassword(const std::string& password) - Sets the password.
 *      void setRole(const UserType& role)            - Sets the user role.
 *
 *      std::string getUserId() const      - Gets the user ID.
 *      std::string getPassword() const    - Gets the password.
 *      std::string getUsername() const    - Gets the username.
 *      UserType getRole() const           - Gets the user role.
 *
 *      virtual void to_json(JSON& json) const = 0; - Serializes the user to a JSON object.
 *
 *      virtual ~UserModel() = default; - Virtual destructor.
 */
class UserModel {
    public:
        enum class UserType {
            Passenger,
            BookingManager,
            Admin,
            INVALID
        };
    protected:
        std::string userId;
        std::string username;
        std::string password;
        UserType role;
    public:
        UserModel() = default;
        UserModel(const std::string& username, const std::string& password, const UserType& role);
        UserModel(const JSON& json);

        inline void setUserId   (const std::string& userId)     { (this -> userId) = userId; }
        inline void setUserName (const std::string& username)   { (this -> username) = username; }
        inline void setPassword (const std::string& password)   { (this -> password) = password; }
        inline void setRole     (const UserType& role)          { (this -> role) = role; }

        inline std::string  getUserId()      const              { return userId; }
        inline std::string  getPassword()    const              { return password; }
        inline std::string  getUsername()    const              { return username; }
        inline UserType     getRole()        const              { return role; }

        virtual void to_json(JSON& json) const = 0;

        virtual ~UserModel() = default;
};