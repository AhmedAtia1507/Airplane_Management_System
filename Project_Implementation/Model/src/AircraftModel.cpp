#include "../include/AircraftModel.hpp"
#include "../../Utils/include/IDGenerator.hpp"
#include "../../Repositories/include/AircraftRepository.hpp"
#include <stdexcept>

/**
 * @brief Constructs an AircraftModel object with the specified model name, capacity, and number of seats per row.
 *
 * This constructor validates the input parameters:
 * - The model name must not be empty.
 * - The aircraft capacity must be a positive integer.
 * - The number of seats per row must be a positive integer and not exceed MAX_SEATS_PER_ROW.
 * - The aircraft capacity must be a multiple of the number of seats per row.
 *
 * Generates a unique aircraft ID and ensures it does not collide with existing IDs in the AircraftRepository.
 * Calculates the number of rows based on the capacity and seats per row.
 *
 * @param model The name of the aircraft model.
 * @param capacity The total seating capacity of the aircraft.
 * @param numOfRowSeats The number of seats in each row.
 *
 * @throws std::invalid_argument if any parameter is invalid.
 */
AircraftModel::AircraftModel(const std::string& model, int capacity, int numOfRowSeats) :
    model(model), capacity(capacity), numOfRowSeats(numOfRowSeats) {
        if (model.empty()) {
            throw std::invalid_argument("Aircraft model cannot be empty.");
        }
        if (capacity <= 0) {
            throw std::invalid_argument("Aircraft capacity must be positive.");
        }
        if (numOfRowSeats <= 0) {
            throw std::invalid_argument("Number of row seats must be positive.");
        }
        if (numOfRowSeats > MAX_SEATS_PER_ROW) {
            throw std::invalid_argument("Number of row seats cannot be greater than " + std::to_string(MAX_SEATS_PER_ROW) + ".");
        }
        if (capacity % numOfRowSeats != 0) {
            throw std::invalid_argument("Aircraft capacity must be a multiple of the number of seats per row.");
        }

        std::string newAircraftId = "AC-" + IDGenerator::generateUniqueID();
        auto aircraftRepository = AircraftRepository::getInstance();
        while( (aircraftRepository -> findAircraftById(newAircraftId)).has_value() ) {
            newAircraftId = "AC-" + IDGenerator::generateUniqueID();
        }
        this->aircraftId = newAircraftId;
        this -> numOfRows = capacity / numOfRowSeats;
}

/**
 * @brief Constructs an AircraftModel object from a JSON representation.
 *
 * This constructor validates the input JSON to ensure all required fields are present
 * and conform to expected formats and constraints. The required keys are "id", "model",
 * "capacity", and "numOfRowSeats". The "id" must start with "AC-", "model" must not be empty,
 * "capacity" and "numOfRowSeats" must be positive integers, and "numOfRowSeats" must not exceed
 * MAX_SEATS_PER_ROW. Additionally, "capacity" must be a multiple of "numOfRowSeats".
 *
 * @param json The JSON object containing aircraft data.
 * @throws std::invalid_argument If any required key is missing, or if any value fails validation.
 */
AircraftModel::AircraftModel(const JSON& json) {
    const std::vector<std::string> required_keys = {"id", "model", "capacity", "numOfRowSeats"};
    for (const auto& key : required_keys) {
        if (!json.contains(key)) {
            throw std::invalid_argument("Invalid JSON for AircraftModel: missing key '" + key + "'.");
        }
    }
    aircraftId = json.at("id").get<std::string>();
    if (aircraftId.substr(0, 3) != "AC-") {
        throw std::invalid_argument("Invalid ID for AircraftModel");
    }
    model = json.at("model").get<std::string>();
    capacity = json.at("capacity").get<int>();
    numOfRowSeats = json.at("numOfRowSeats").get<int>();

    if (model.empty()) {
        throw std::invalid_argument("Aircraft model cannot be empty.");
    }
    if (capacity <= 0) {
        throw std::invalid_argument("Aircraft capacity must be positive.");
    }
    if (numOfRowSeats <= 0) {
        throw std::invalid_argument("Number of row seats must be positive.");
    }
    if (numOfRowSeats > MAX_SEATS_PER_ROW) {
        throw std::invalid_argument("Number of row seats cannot be greater than " + std::to_string(MAX_SEATS_PER_ROW) + ".");
    }
    if (capacity % numOfRowSeats != 0) {
        throw std::invalid_argument("Aircraft capacity must be a multiple of the number of seats per row.");
    }
    this -> numOfRows = capacity / numOfRowSeats;
}

/**
 * @brief Serializes the AircraftModel object to a JSON representation.
 *
 * This method populates the provided JSON object with the aircraft's properties,
 * including its ID, model name, seating capacity, and number of row seats.
 *
 * @param json Reference to a JSON object that will be assigned the serialized data.
 */
void AircraftModel::to_json(JSON& json) const {
    json = JSON {
        {"id", aircraftId},
        {"model", model},
        {"capacity", capacity},
        {"numOfRowSeats", numOfRowSeats}
    };
}

void AircraftModel::setCapacity(int capacity) {
    if (capacity <= 0) {
        throw std::invalid_argument("Aircraft capacity must be positive.");
    }
    if (capacity % numOfRowSeats != 0) {
        throw std::invalid_argument("Aircraft capacity must be a multiple of the number of seats per row.");
    }
    this -> capacity = capacity;
    this -> numOfRows = capacity / numOfRowSeats;
}
void AircraftModel::setNumOfRowSeats(int numOfRowSeats) {
    if (numOfRowSeats <= 0) {
        throw std::invalid_argument("Number of row seats must be positive.");
    }
    if (numOfRowSeats > MAX_SEATS_PER_ROW) {
        throw std::invalid_argument("Number of row seats cannot be greater than " + std::to_string(MAX_SEATS_PER_ROW) + ".");
    }
    if (capacity % numOfRowSeats != 0) {
        throw std::invalid_argument("Aircraft capacity must be a multiple of the number of seats per row.");
    }
    this -> numOfRowSeats = numOfRowSeats;
    this -> numOfRows = capacity / numOfRowSeats;
}