#pragma once
#include <string>
#include "../../Third_Party/json.hpp"
#include "../../Utils/include/DateTime.hpp"

using JSON = nlohmann::json;

/**
 * @class AircraftModel
 * @brief Represents the model and seating configuration of an aircraft.
 *
 * This class encapsulates information about an aircraft, including its unique identifier,
 * model name, seating capacity, number of seats per row, and total number of rows.
 * It provides constructors for initialization from parameters or a JSON object,
 * as well as accessors for all member variables.
 *
 * @note The maximum number of seats per row is defined as 26, corresponding to the letters of the alphabet.
 *
 * @constructor AircraftModel() Default constructor.
 * @constructor AircraftModel(const std::string& model, int capacity, int numOfRowSeats) Constructs an AircraftModel with the specified model, capacity, and seats per row.
 * @constructor AircraftModel(const JSON& json) Constructs an AircraftModel from a JSON object.
 *
 * @method std::string getAircraftId() const Returns the unique identifier of the aircraft.
 * @method std::string getModel() const Returns the model name of the aircraft.
 * @method int getCapacity() const Returns the seating capacity of the aircraft.
 * @method int getNumOfRowSeats() const Returns the number of seats per row.
 * @method int getNumOfRows() const Returns the total number of rows.
 * @method void to_json(JSON& json) const Serializes the AircraftModel to a JSON object.
 *
 * @var static constexpr int MAX_SEATS_PER_ROW The maximum number of seats per row (26).
 */
class AircraftModel {
    std::string aircraftId;
    std::string model;
    int capacity;
    int numOfRowSeats;
    int numOfRows;

    public:
        static constexpr int MAX_SEATS_PER_ROW = 26; // Based on letters of the alphabet for seat designation

        AircraftModel() = default;
        AircraftModel(const std::string& model, int capacity, int numOfRowSeats);
        AircraftModel(const JSON& json);

        inline std::string getAircraftId() const            { return aircraftId; }
        inline std::string getModel() const                 { return model; }
        inline int getCapacity() const                      { return capacity; }
        inline int getNumOfRowSeats() const                 { return numOfRowSeats; }
        inline int getNumOfRows() const                     { return numOfRows; }

        void setModel(const std::string& model)      { this -> model = model; }
        void setCapacity(int capacity);
        void setNumOfRowSeats(int numOfRowSeats);
        
        void to_json(JSON& json) const;

        ~AircraftModel() = default;
};