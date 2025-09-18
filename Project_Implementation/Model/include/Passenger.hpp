#pragma once

#include "UserModel.hpp"

/**
 * @class Passenger
 * @brief Represents a passenger user in the system, inheriting from UserModel.
 *
 * The Passenger class extends the UserModel to include loyalty points functionality.
 * It provides constructors for initialization from username/password, from JSON, and
 * methods to get and set loyalty points. The class also supports serialization to JSON.
 *
 * @note Loyalty points are stored as a floating-point value.
 */
class Passenger : public UserModel {
    float loyaltyPoints;

    public:
        Passenger();
        Passenger(const std::string& username, const std::string& password, float loyaltyPoints = 0.0f);
        Passenger(const JSON& json);

        inline void setLoyaltyPoints(float points) { loyaltyPoints = points; }
        inline float getLoyaltyPoints() const { return loyaltyPoints; }

        void to_json(JSON& json) const override;

        ~Passenger() = default;
};