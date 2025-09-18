#pragma once

#include <memory>
#include "ReservationModel.hpp"

/**
 * @class ReservationModelBuilder
 * @brief Builder class for constructing ReservationModel objects.
 *
 * This class provides a fluent interface for setting various properties
 * of a ReservationModel, such as flight ID, passenger ID, seat number,
 * reservation status, and payment ID. After setting the desired properties,
 * use the build() method to create a shared pointer to a ReservationModel
 * instance with the specified configuration.
 *
 * Usage example:
 * @code
 * auto reservation = ReservationModelBuilder()
 *     .setFlightId("FL-123")
 *     .setPassengerId("PAS-456")
 *     .setSeatNumber("12A")
 *     .setStatus(ReservationModel::ReservationStatus::CONFIRMED)
 *     .setPaymentId("PAY-789")
 *     .build();
 * @endcode
 */
class ReservationModelBuilder {
    std::string flightId = "";
    std::string passengerId = "";
    std::string seatNumber = "";
    ReservationModel::ReservationStatus status = ReservationModel::ReservationStatus::CONFIRMED;
    std::string paymentId = "";

    public:
        ReservationModelBuilder() = default;
        ReservationModelBuilder& setFlightId(const std::string& flightId);
        ReservationModelBuilder& setPassengerId(const std::string& passengerId);
        ReservationModelBuilder& setSeatNumber(const std::string& seatNumber);
        ReservationModelBuilder& setStatus(const ReservationModel::ReservationStatus& status);
        ReservationModelBuilder& setPaymentId(const std::string& paymentId);

        std::shared_ptr<ReservationModel> build() const;

        ~ReservationModelBuilder() = default;
};