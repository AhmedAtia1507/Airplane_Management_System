#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "../../Model/include/AircraftModel.hpp"

/**
 * @class AircraftRepository
 * @brief Singleton repository for managing AircraftModel instances.
 *
 * This class provides thread-safe access and management of aircraft models,
 * including adding, updating, deleting, and searching by aircraft ID.
 * It enforces the singleton pattern to ensure a single instance throughout the application.
 *
 * Usage:
 *   - Use getInstance() to obtain the singleton instance.
 *   - Use addAircraft(), updateAircraft(), deleteAircraft() to modify the repository.
 *   - Use findAircraftById() to retrieve aircraft models by their unique ID.
 *
 * Note:
 *   - Copy and move operations are disabled to preserve singleton integrity.
 */
class AircraftRepository {
    std::unordered_map<std::string, std::shared_ptr<AircraftModel>> aircrafts;

    AircraftRepository();
    AircraftRepository(const AircraftRepository&) = delete;
    AircraftRepository& operator=(const AircraftRepository&) = delete;
    AircraftRepository(AircraftRepository&&) = delete;
    AircraftRepository& operator=(AircraftRepository&&) = delete;

    public:
        static std::shared_ptr<AircraftRepository> getInstance();
        std::optional<std::shared_ptr<AircraftModel>> findAircraftById(const std::string& aircraftId) const;
        std::vector<std::shared_ptr<AircraftModel>> getAllAircrafts() const;
        bool addAircraft(const AircraftModel& newAircraft);
        bool updateAircraft(const AircraftModel& aircraft);
        bool deleteAircraft(const std::string& aircraftId);

        ~AircraftRepository();
};