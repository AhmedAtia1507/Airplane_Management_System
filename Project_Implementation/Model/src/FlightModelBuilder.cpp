#include "../include/FlightModelBuilder.hpp"

/**
 * @brief Sets the origin for the flight model.
 * 
 * This method assigns the provided origin string to the flight model's origin attribute.
 * 
 * @param origin The origin location as a string.
 * @return Reference to the current FlightModelBuilder instance for method chaining.
 */
FlightModelBuilder& FlightModelBuilder::setOrigin(const std::string& origin) {
    (this -> origin) = origin;
    return *this;
}
/**
 * @brief Sets the destination for the flight model.
 * 
 * This method assigns the provided destination string to the internal destination member
 * and returns a reference to the current FlightModelBuilder instance to allow method chaining.
 * 
 * @param destination The destination to set for the flight model.
 * @return Reference to the current FlightModelBuilder instance.
 */
FlightModelBuilder& FlightModelBuilder::setDestination(const std::string& destination) {
    (this -> destination) = destination;
    return *this;
}
/**
 * @brief Sets the departure time for the flight.
 * 
 * This method assigns the specified departure time to the flight model builder.
 * 
 * @param time The departure time to set.
 * @return Reference to the current FlightModelBuilder instance for method chaining.
 */
FlightModelBuilder& FlightModelBuilder::setDepartureTime(const DateTime& time) {
    (this -> departureTime) = time;
    return *this;
}
/**
 * @brief Sets the arrival time for the flight.
 * 
 * This method assigns the specified arrival time to the flight model being built.
 * 
 * @param time The arrival time as a DateTime object.
 * @return Reference to the current FlightModelBuilder instance for method chaining.
 */
FlightModelBuilder& FlightModelBuilder::setArrivalTime(const DateTime& time) {
    (this -> arrivalTime) = time;
    return *this;
}
/**
 * @brief Sets the aircraft ID for the flight model.
 * 
 * This method assigns the provided aircraft ID to the internal member variable.
 * 
 * @param id The aircraft ID to set.
 * @return Reference to the current FlightModelBuilder instance for method chaining.
 */
FlightModelBuilder& FlightModelBuilder::setAircraftId(const std::string& id) {
    (this -> aircraftId) = id;
    return *this;
}
/**
 * @brief Sets the crew member IDs for the flight model.
 * 
 * This method assigns the provided vector of crew member IDs to the internal
 * crewMemberIds member variable of the FlightModelBuilder. It enables method
 * chaining by returning a reference to the current builder instance.
 * 
 * @param ids A vector of strings representing the crew member IDs.
 * @return Reference to the current FlightModelBuilder instance for chaining.
 */
FlightModelBuilder& FlightModelBuilder::setCrewMemberIds(const std::vector<std::string>& ids) {
    (this -> crewMemberIds) = ids;
    return *this;
}
/**
 * @brief Adds a crew member ID to the list of crew member IDs.
 *
 * This method appends the provided crew member ID to the internal collection
 * of crew member IDs associated with the flight model being built.
 *
 * @param id The crew member ID to add.
 * @return Reference to the current FlightModelBuilder instance for method chaining.
 */
FlightModelBuilder& FlightModelBuilder::addCrewMemberId(const std::string& id) {
    (this -> crewMemberIds).push_back(id);
    return *this;
}

/**
 * @brief Builds and returns a shared pointer to a FlightModel instance.
 *
 * This method constructs a FlightModel object using the parameters set in the builder.
 * It validates that all required parameters (origin, destination, aircraftId, crewMemberIds)
 * are provided and throws std::invalid_argument if any are missing.
 *
 * @return std::shared_ptr<FlightModel> A shared pointer to the constructed FlightModel.
 * @throws std::invalid_argument If any required flight parameter is missing.
 */
std::shared_ptr<FlightModel> FlightModelBuilder::build() const {
    if (origin.empty() || destination.empty() || aircraftId.empty()) {
        throw std::invalid_argument("Missing required flight parameters.");
    }
    return std::make_shared<FlightModel>(
        origin, destination, departureTime, arrivalTime, aircraftId, crewMemberIds
    );
}