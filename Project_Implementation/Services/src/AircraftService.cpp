#include "../include/AircraftService.hpp"
#include "../../Repositories/include/AircraftRepository.hpp"

/**
 * @brief Retrieves all aircraft models from the repository.
 *
 * This method returns a vector containing shared pointers to all
 * AircraftModel instances currently stored in the AircraftRepository.
 *
 * @return std::vector<std::shared_ptr<AircraftModel>> 
 *         A vector of shared pointers to AircraftModel objects.
 */
std::vector<std::shared_ptr<AircraftModel>> AircraftService::getAllAircrafts() {
    return AircraftRepository::getInstance() -> getAllAircrafts();
}
/**
 * @brief Retrieves an aircraft model by its unique identifier.
 *
 * This method queries the aircraft repository for an aircraft with the specified ID.
 * If found, returns a shared pointer to the AircraftModel wrapped in an std::optional.
 * If not found, returns std::nullopt.
 *
 * @param aircraftId The unique identifier of the aircraft to retrieve.
 * @return std::optional<std::shared_ptr<AircraftModel>> An optional containing a shared pointer to the AircraftModel if found, or std::nullopt otherwise.
 */
std::optional<std::shared_ptr<AircraftModel>> AircraftService::getAircraftById(const std::string& aircraftId) {
    return AircraftRepository::getInstance() -> findAircraftById(aircraftId);
}
/**
 * @brief Adds a new aircraft to the repository.
 *
 * Creates an AircraftModel instance with the specified model name, capacity, and number of row seats.
 * Attempts to add the aircraft to the AircraftRepository. If successful, returns a shared pointer
 * to the newly created AircraftModel; otherwise, returns nullptr.
 *
 * @param model The name or type of the aircraft model.
 * @param capacity The seating capacity of the aircraft.
 * @param numOfRowSeats The number of seats per row in the aircraft.
 * @return std::optional<std::shared_ptr<AircraftModel>> An optional containing a shared pointer to the newly added AircraftModel if successful, otherwise std::nullopt.
 */
std::optional<std::shared_ptr<AircraftModel>> AircraftService::addAircraft (
    const std::string& model,
    int capacity,
    int numOfRowSeats
) {
    AircraftModel newAircraft(model, capacity, numOfRowSeats);
    if ( AircraftRepository::getInstance() -> addAircraft(newAircraft) ) {
        return AircraftRepository::getInstance() -> findAircraftById(newAircraft.getAircraftId());
    }
    return std::nullopt;
}
/**
 * @brief Updates the information of an existing aircraft.
 *
 * This method attempts to update the details of the specified aircraft model
 * in the underlying repository. It delegates the update operation to the
 * AircraftRepository singleton instance.
 *
 * @param aircraft The AircraftModel object containing updated aircraft information.
 * @return true if the update was successful; false otherwise.
 */
bool AircraftService::updateAircraft(const AircraftModel& aircraft) {
    return AircraftRepository::getInstance() -> updateAircraft(aircraft);
}
/**
 * @brief Deletes an aircraft with the specified ID.
 *
 * This method attempts to remove the aircraft identified by the given aircraftId
 * from the repository. It delegates the deletion operation to the AircraftRepository.
 *
 * @param aircraftId The unique identifier of the aircraft to be deleted.
 * @return true if the aircraft was successfully deleted; false otherwise.
 */
bool AircraftService::deleteAircraft(const std::string& aircraftId) {
    return AircraftRepository::getInstance() -> deleteAircraft(aircraftId);
}