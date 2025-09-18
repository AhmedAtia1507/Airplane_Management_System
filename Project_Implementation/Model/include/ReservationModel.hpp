#pragma once

#include <string>
#include "../../Third_Party/json.hpp"

using JSON = nlohmann::json;

/**
 * @class ReservationModel
 * @brief Represents a reservation for a flight, including passenger, seat, status, and payment information.
 *
 * This model encapsulates all relevant data for a flight reservation, such as the flight ID,
 * passenger ID, seat number, reservation status, and payment ID. It provides constructors for
 * initialization from parameters or a JSON object, as well as methods for serialization and
 * accessors/mutators for each field.
 *
 * @enum ReservationStatus
 *      CONFIRMED - The reservation is confirmed.
 *      CANCELLED - The reservation has been cancelled.
 *
 * @constructor ReservationModel()
 *      Default constructor.
 * @constructor ReservationModel(const std::string& flightId, const std::string& passengerId, 
 *      const std::string& seatNumber, const ReservationStatus& status, const std::string& paymentId)
 *      Constructs a ReservationModel with the specified details.
 * @constructor ReservationModel(const JSON& json)
 *      Constructs a ReservationModel from a JSON object.
 *
 * @method void to_json(JSON& json) const
 *      Serializes the reservation model to a JSON object.
 *
 * @method std::string getReservationId() const
 *      Gets the reservation ID.
 * @method std::string getFlightId() const
 *      Gets the flight ID.
 * @method std::string getPassengerId() const
 *      Gets the passenger ID.
 * @method std::string getSeatNumber() const
 *      Gets the seat number.
 * @method ReservationStatus getStatus() const
 *      Gets the reservation status.
 * @method std::string getPaymentId() const
 *      Gets the payment ID.
 *
 * @method void setReservationId(const std::string& reservationId)
 *      Sets the reservation ID.
 * @method void setFlightId(const std::string& flightId)
 *      Sets the flight ID.
 * @method void setPassengerId(const std::string& passengerId)
 *      Sets the passenger ID.
 * @method void setSeatNumber(const std::string& seatNumber)
 *      Sets the seat number.
 * @method void setStatus(const ReservationStatus& status)
 *      Sets the reservation status.
 * @method void setPaymentId(const std::string& paymentId)
 *      Sets the payment ID.
 *
 * @destructor ~ReservationModel()
 *      Default destructor.
 */
class ReservationModel {
public:
    enum class ReservationStatus {
        CONFIRMED,
        CANCELLED
    };
private:
    std::string reservationId;
    std::string flightId;
    std::string passengerId;
    std::string seatNumber;
    ReservationStatus status;
    std::string paymentId;

public:
    ReservationModel() = default;
    ReservationModel(const std::string& flightId, const std::string& passengerId, 
        const std::string& seatNumber, const ReservationStatus& status, const std::string& paymentId);
    ReservationModel(const JSON& json);

    void to_json(JSON& json) const;
    
    inline std::string getReservationId() const                     { return reservationId; }
    inline std::string getFlightId() const                          { return flightId; } 
    inline std::string getPassengerId() const                       { return passengerId; }
    inline std::string getSeatNumber() const                        { return seatNumber; }
    inline ReservationStatus getStatus() const                      { return status; }
    inline std::string getPaymentId() const                         { return paymentId; }


    inline void setReservationId(const std::string& reservationId)  { this->reservationId = reservationId; }
    inline void setFlightId(const std::string& flightId)            { this->flightId = flightId; }
    inline void setPassengerId(const std::string& passengerId)      { this->passengerId = passengerId; }
    void setSeatNumber(const std::string& seatNumber);
    inline void setStatus(const ReservationStatus& status)          { this->status = status; }
    inline void setPaymentId(const std::string& paymentId)          { this->paymentId = paymentId; }


    ~ReservationModel() = default;
};