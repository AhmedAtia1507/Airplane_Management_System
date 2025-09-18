#pragma once

#include "../../Model/include/ReservationModel.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

/**
 * @class ReservationRepository
 * @brief Singleton repository for managing ReservationModel objects.
 *
 * This class provides methods to add, update, delete, and query reservations.
 * It ensures only one instance exists using the singleton pattern.
 * Reservations are stored in an unordered map, indexed by reservation ID.
 *
 * Copy and move operations are deleted to enforce singleton behavior.
 *
 * Public Methods:
 * - getInstance(): Returns the singleton instance of the repository.
 * - findReservationById(): Finds a reservation by its ID.
 * - findReservationsByPassenger(): Finds all reservations for a given passenger ID.
 * - addReservation(): Adds a new reservation.
 * - updateReservation(): Updates an existing reservation.
 * - deleteReservation(): Deletes a reservation by its ID.
 */
class ReservationRepository {
    std::unordered_map<std::string, std::shared_ptr<ReservationModel>> reservations;

    ReservationRepository();
    ReservationRepository(const ReservationRepository&) = delete;
    ReservationRepository& operator=(const ReservationRepository&) = delete;
    ReservationRepository(ReservationRepository&&) = delete;
    ReservationRepository& operator=(ReservationRepository&&) = delete;

    public:
        static std::shared_ptr<ReservationRepository> getInstance();
        std::optional<std::shared_ptr<ReservationModel>> findReservationById(const std::string& reservationId) const;
        std::vector<std::shared_ptr<ReservationModel>> findReservationsByPassenger(const std::string& passengerId) const;
        std::vector<std::shared_ptr<ReservationModel>> getAllReservations() const;
        bool addReservation(const ReservationModel& newReservation);
        bool updateReservation(const ReservationModel& reservation);
        bool deleteReservation(const std::string& reservationId);

        ~ReservationRepository();
};