#include "../include/AircraftRepository.hpp"
#include "../../Utils/include/JSONManager.hpp"
#include "../../Utils/include/DatabasePathResolver.hpp"
#include <stdexcept>

/**
 * @brief Path to the aircraft database JSON file.
 *
 * This constant defines the relative file path to the JSON file
 * containing aircraft data used by the repository.
 */
const std::string AIRCRAFT_DATABASE_PATH = DatabasePathResolver::getDatabasePath() + "aircrafts.json";
/**
 * @brief Returns the singleton instance of AircraftRepository.
 *
 * Implements Meyers' Singleton pattern using a static local variable,
 * ensuring thread-safe and lazy initialization of the AircraftRepository instance.
 *
 * @return std::shared_ptr<AircraftRepository> Shared pointer to the singleton instance.
 */
std::shared_ptr<AircraftRepository> AircraftRepository::getInstance() {
    // Meyers' Singleton: thread-safe and lazy-initialized.
    static std::shared_ptr<AircraftRepository> instance(new AircraftRepository());
    return instance;
}
/**
 * @brief Finds an aircraft by its unique identifier.
 *
 * Searches the repository for an aircraft with the specified ID.
 * If found, returns a shared pointer to the AircraftModel wrapped in std::optional.
 * If not found, returns std::nullopt.
 *
 * @param aircraftId The unique identifier of the aircraft to find.
 * @return std::optional<std::shared_ptr<AircraftModel>> Shared pointer to the AircraftModel if found, std::nullopt otherwise.
 */
std::optional<std::shared_ptr<AircraftModel>> AircraftRepository::findAircraftById(const std::string& aircraftId) const {
    if(aircrafts.find(aircraftId) == aircrafts.end()) {
        return std::nullopt;
    }

    return aircrafts.at(aircraftId);
}
/**
 * @brief Retrieves all aircraft models stored in the repository.
 *
 * This function returns a vector containing shared pointers to all
 * AircraftModel instances currently managed by the repository.
 *
 * @return std::vector<std::shared_ptr<AircraftModel>> 
 *         A vector of shared pointers to AircraftModel objects.
 */
std::vector<std::shared_ptr<AircraftModel>> AircraftRepository::getAllAircrafts() const {
    std::vector<std::shared_ptr<AircraftModel>> ExistingAircrafts;
    for (const auto& [id, aircraft] : aircrafts) {
        ExistingAircrafts.push_back(aircraft);
    }
    return ExistingAircrafts;
}
/**
 * @brief Constructs an AircraftRepository object and initializes the aircrafts data.
 *
 * This constructor parses the aircraft database JSON file located at AIRCRAFT_DATABASE_PATH
 * and populates the aircrafts collection using JSONManager::parseJSON.
 */
AircraftRepository::AircraftRepository() {
    JSONManager::parseJSON(aircrafts, AIRCRAFT_DATABASE_PATH);
}
/**
 * @brief Adds a new aircraft to the repository.
 *
 * This method attempts to add the provided AircraftModel to the repository.
 * If an aircraft with the same ID already exists, the method returns false
 * and does not add the aircraft. Otherwise, the aircraft is added and the
 * method returns true.
 *
 * @param newAircraft The AircraftModel object to be added.
 * @return true if the aircraft was successfully added; false if an aircraft
 *         with the same ID already exists.
 */
bool AircraftRepository::addAircraft(const AircraftModel& newAircraft) {
    if(aircrafts.find(newAircraft.getAircraftId()) != aircrafts.end()) {
        return false;
    }
    aircrafts[newAircraft.getAircraftId()] = std::make_shared<AircraftModel>(newAircraft);
    return true;
}
/**
 * @brief Updates an existing aircraft in the repository.
 *
 * This function searches for an aircraft with the specified ID in the repository.
 * If the aircraft exists, it updates the entry with the provided aircraft model.
 * If the aircraft does not exist, the function returns false.
 *
 * @param aircraft The AircraftModel object containing updated aircraft information.
 * @return true if the aircraft was found and updated; false otherwise.
 */
bool AircraftRepository::updateAircraft(const AircraftModel& aircraft) {
    if(aircrafts.find(aircraft.getAircraftId()) == aircrafts.end()) {
        return false;
    }
    aircrafts[aircraft.getAircraftId()] = std::make_shared<AircraftModel>(aircraft);
    return true;
}
/**
 * @brief Deletes an aircraft from the repository by its ID.
 *
 * This method searches for an aircraft with the specified ID in the repository.
 * If the aircraft is found, it is removed from the repository and the method returns true.
 * If the aircraft is not found, the method returns false.
 *
 * @param aircraftId The unique identifier of the aircraft to be deleted.
 * @return true if the aircraft was found and deleted; false otherwise.
 */
bool AircraftRepository::deleteAircraft(const std::string& aircraftId) {
    if(aircrafts.find(aircraftId) == aircrafts.end()) {
        return false;
    }
    aircrafts.erase(aircraftId);
    return true;
}

/**
 * @brief Destructor for the AircraftRepository class.
 *
 * This destructor is responsible for saving the current state of the aircrafts
 * collection to a JSON file specified by AIRCRAFT_DATABASE_PATH using the JSONManager.
 * Ensures that any changes made to the aircrafts are persisted before the repository
 * object is destroyed.
 */
AircraftRepository::~AircraftRepository() {
    JSONManager::saveToJSON(aircrafts, AIRCRAFT_DATABASE_PATH);
    aircrafts.clear();
}