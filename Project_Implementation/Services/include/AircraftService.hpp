#pragma once

#include <string>
#include <optional>
#include <memory>
#include <vector>
#include "../../Model/include/AircraftModel.hpp"


/**
 * @brief Service class for managing aircraft operations in the airplane management system.
 * 
 * This class provides static methods for CRUD operations on aircraft data.
 * It serves as a service layer between the application logic and data storage,
 * handling aircraft management operations such as creation, retrieval, updating, and deletion.
 * 
 * @note This class cannot be instantiated as the default constructor is deleted.
 *       All operations are performed through static methods.
 */
class AircraftService {
    public:
        AircraftService() = delete;

        static std::vector<std::shared_ptr<AircraftModel>> getAllAircrafts();
        static std::optional<std::shared_ptr<AircraftModel>> getAircraftById(const std::string& aircraftId);
        static std::optional<std::shared_ptr<AircraftModel>> addAircraft(
            const std::string& model,
            int capacity,
            int numOfRowSeats
        );
        static bool updateAircraft(const AircraftModel& aircraft);
        static bool deleteAircraft(const std::string& aircraftId);
};