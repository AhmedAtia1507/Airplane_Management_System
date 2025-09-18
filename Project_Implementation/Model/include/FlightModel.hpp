#pragma once
#include <string>
#include <vector>
#include "../../Third_Party/json.hpp"
#include "../../Utils/include/DateTime.hpp"

using JSON = nlohmann::json;

/**
 * @class FlightModel
 * @brief Represents a flight with its details, crew, and seat map.
 *
 * This class encapsulates all relevant information about a flight, including its
 * identifiers, origin and destination, departure and arrival times, associated aircraft,
 * crew members, and seat occupancy map. It provides methods to set and retrieve these
 * properties, as well as to manipulate seat statuses.
 *
 * @note Seat map is represented as a 2D vector of booleans, where each element indicates
 *       whether a seat is occupied (true) or available (false).
 *
 */
class FlightModel {
    std::string flightId;
    std::string origin;
    std::string destination;
    DateTime departureTime;
    DateTime arrivalTime;
    std::string aircraftId;
    std::vector<std::string> crewMemberIds;
    std::vector<std::vector<bool>> seatMap;

    private:
        std::pair<int, int> getSeatIndices(const std::string& seatNumber) const;

    public:
        FlightModel() = default;
        FlightModel(const std::string& origin, const std::string& destination,
                     const DateTime& departureTime, const DateTime& arrivalTime, const std::string& aircraftId,
                     const std::vector<std::string>& crewMemberIds = {});
        FlightModel(const JSON& json);

        inline void setFlightId(const std::string& id)                      { flightId = id; }
        inline void setOrigin(const std::string& origin)                    { this->origin = origin;}
        inline void setDestination(const std::string& destination)          { this->destination = destination; }
        inline void setDepartureTime(const DateTime& time)                  { this->departureTime = time; }
        inline void setArrivalTime(const DateTime& time)                    { this->arrivalTime = time; }
        inline void setAircraftId(const std::string& id)                    { this->aircraftId = id; }
        inline void setCrewMemberIds(const std::vector<std::string>& ids)   { this->crewMemberIds = ids; }
        inline void addCrewMemberId(const std::string& id)                  { crewMemberIds.push_back(id); }
        bool removeCrewMemberId(const std::string& id);
        void setSeatStatus(const std::string& seatNumber, bool status);
        bool isValidSeat(const std::string& seatNumber) const;
        inline const std::string& getFlightId() const                       { return flightId; }
        inline const std::string& getOrigin() const                         { return origin; }
        inline const std::string& getDestination() const                    { return destination; }
        inline const DateTime& getDepartureTime() const                     { return departureTime; }
        inline const DateTime& getArrivalTime() const                       { return arrivalTime; }
        inline const std::string& getAircraftId() const                     { return aircraftId; }
        inline const std::vector<std::string>& getCrewMemberIds() const     { return crewMemberIds; }
        inline const std::vector<std::vector<bool>>& getSeatMap() const     { return seatMap; }
        bool getSeatStatus(const std::string& seatNumber) const;
        
        void to_json(JSON& json) const;

        ~FlightModel() = default;
};