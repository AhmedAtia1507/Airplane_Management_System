#pragma once

#include "FlightModel.hpp"

/**
 * @class FlightModelBuilder
 * @brief Builder class for constructing FlightModel objects.
 *
 * This class provides a fluent interface for setting various properties required to create a FlightModel instance,
 * such as origin, destination, departure and arrival times, aircraft ID, and crew member IDs.
 *
 * Usage example:
 * @code
 * auto flight = FlightModelBuilder()
 *                  .setOrigin("JFK")
 *                  .setDestination("LAX")
 *                  .setDepartureTime(DateTime(2024, 6, 1, 8, 0, 0))
 *                  .setArrivalTime(DateTime(2024, 6, 1, 11, 0, 0))
 *                  .setAircraftId("AC-320")
 *                  .addCrewMemberId("CM-001")
 *                  .addCrewMemberId("CM-002")
 *                  .build();
 * @endcode
 */
class FlightModelBuilder {
    std::string origin = "";
    std::string destination = "";
    DateTime departureTime = DateTime(0, 0, 0, 0, 0);
    DateTime arrivalTime = DateTime(0, 0, 0, 0, 0);
    std::string aircraftId = "";
    std::vector<std::string> crewMemberIds = {};

    public:
        FlightModelBuilder& setOrigin(const std::string& origin);
        FlightModelBuilder& setDestination(const std::string& destination);
        FlightModelBuilder& setDepartureTime(const DateTime& time);
        FlightModelBuilder& setArrivalTime(const DateTime& time);
        FlightModelBuilder& setAircraftId(const std::string& id);
        FlightModelBuilder& setCrewMemberIds(const std::vector<std::string>& ids);
        FlightModelBuilder& addCrewMemberId(const std::string& id);

        std::shared_ptr<FlightModel> build() const;

        ~FlightModelBuilder() = default;
};