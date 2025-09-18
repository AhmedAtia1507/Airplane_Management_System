#include "../include/BookingManagerInterface.hpp"
#include "../../Controller/include/BookingManagerController.hpp"
#include "../../Model/include/Passenger.hpp"
#include <iostream>
#include <limits>

BookingManagerInterface::BookingManagerInterface(const std::shared_ptr<BookingManager>& bookingManager)
    : currentUser(bookingManager) {}

void BookingManagerInterface::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void BookingManagerInterface::displayBookingManagerMenu() {
    std::cout << "Booking Manager Interface - Please choose an option:" << std::endl;
    std::cout << "1. Search Flights" << std::endl;
    std::cout << "2. View Bookings" << std::endl;
    std::cout << "3. Book a Flight" << std::endl;
    std::cout << "4. Modify a Booking" << std::endl;
    std::cout << "5. Cancel a Booking" << std::endl;
    std::cout << "6. Logout" << std::endl;
    std::cout << "Choice: ";
}

void BookingManagerInterface::startInterface() {
    int choice;
    do {
        displayBookingManagerMenu();
        std::cin >> choice;

        switch (choice) {
            case SEARCH_FLIGHTS_OPTION:
                searchFlights();
                break;
            case VIEW_BOOKINGS_OPTION:
                std::cout << " ----- View All Bookings ----- " << std::endl;
                viewBookings();
                break;
            case BOOK_FLIGHT_OPTION:
                bookFlight();
                break;
            case MODIFY_BOOKING_OPTION:
                modifyBooking();
                break;
            case CANCEL_BOOKING_OPTION:
                cancelBooking();
                break;
            case LOGOUT_OPTION:
                std::cout << "Logging out..." << std::endl;
                break;
                default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != LOGOUT_OPTION);
}

void BookingManagerInterface::searchFlights() {
    std::string origin;
    std::string destination;
    std::string departureDateStr;
    
    clearInputBuffer();
    std::cout << "Please enter the origin of the flight: ";
    std::getline(std::cin, origin);
    std::cout << "Please enter the destination of the flight: ";
    std::getline(std::cin, destination);
    std::cout << "Please enter the departure date (YYYY-MM-DD): ";
    std::getline(std::cin, departureDateStr);   
    
    DateTime departureDate(departureDateStr);
    auto flights = BookingManagerController::getFlightsByRouteAndDate(currentUser->getUserId(), origin, destination, departureDate);
    
    if (flights.empty()) {
        std::cout << "No flights found for the specified criteria." << std::endl;
        return;
    }
    std::cout << "Available Flights:" << std::endl;
    int index = 1;
    for (const auto& flight : flights) {
        std::cout << index << ". Flight ID: " << flight->getFlightId() << std::endl;
        std::cout << "   Origin: " << flight->getOrigin() << std::endl;
        std::cout << "   Destination: " << flight->getDestination() << std::endl;
        std::cout << "   Departure Time: " << flight->getDepartureTime().toString() << std::endl;
        std::cout << "   Arrival Time: " << flight->getArrivalTime().toString() << std::endl;
        std::cout << "------------------------" << std::endl;
        index++;
    }
}

bool BookingManagerInterface::viewBookings() {
    // Implement view bookings logic here
    auto reservations = BookingManagerController::getAllReservations(currentUser->getUserId());
    if (reservations.empty()) {
        std::cout << "No reservations found." << std::endl;
        return false;
    }

    std::cout << "Available Reservations:" << std::endl;
    int index = 1;
    for (const auto& reservation : reservations) {
        std::cout << index << ". Reservation ID: " << reservation->getReservationId() << std::endl;
        std::cout << "   Flight ID: " << reservation->getFlightId() <<  std::endl;
        std::cout << "   Seat Number: " << reservation->getSeatNumber() << std::endl;
        std::cout << "   Status: " << (reservation->getStatus() == ReservationModel::ReservationStatus::CONFIRMED ? "Confirmed" : "Cancelled") << std::endl;
        std::cout << "   Passenger ID: " << reservation->getPassengerId() << std::endl;
        std::cout << "------------------------" << std::endl;
        index++;
    }
    return true;
}
void BookingManagerInterface::displayAllPassengers() {
    auto passengers = BookingManagerController::getAllPassengers(currentUser->getUserId());
    if (passengers.empty()) {
        std::cout << "No passengers found." << std::endl;
        return;
    }
    std::cout << "Available Passengers:" << std::endl;
    int index = 1;
    for (const auto& user : passengers) {
        auto passenger = std::dynamic_pointer_cast<Passenger>(user);
        std::cout << index << ". Passenger ID: " << passenger -> getUserId() << std::endl;
        std::cout << "   Name: " << passenger -> getUsername() << std::endl;
        std::cout << "   Loyalty Points: " << passenger -> getLoyaltyPoints() << std::endl;
        std::cout << "------------------------" << std::endl;
        index++;
    }
}
void BookingManagerInterface::displayAllFlights() {
    auto flights = BookingManagerController::getAllFlights(currentUser->getUserId());
    if (flights.empty()) {
        std::cout << "No flights found." << std::endl;
        return;
    }
    std::cout << "Available Flights:" << std::endl;
    int index = 1;
    for (const auto& flight : flights) {
        std::cout << index << ". Flight ID: " << flight->getFlightId() << std::endl;
        std::cout << "   Origin: " << flight->getOrigin() << std::endl;
        std::cout << "   Destination: " << flight->getDestination() << std::endl;
        std::cout << "   Departure Time: " << flight->getDepartureTime().toString() << std::endl;
        std::cout << "   Arrival Time: " << flight->getArrivalTime().toString() << std::endl;
        std::cout << "------------------------" << std::endl;
        index++;
    }
}
void BookingManagerInterface::displaySeatMap(const std::vector<std::vector<bool>>& seatMap) {
    if (seatMap.empty()) {
        std::cout << "No seat map available for this flight." << std::endl;
        return;
    }

    std::cout << "Legend: [O] = Available, [X] = Occupied" << std::endl;
    for(std::size_t index = 0; index < seatMap.size(); index++) {
        std::cout << "Row " << (index + 1) << "\t";
        for(std::size_t j = 0; j < seatMap[index].size(); j++) {
            if(seatMap[index][j]) {
                std::cout << "[X]\t"; // Occupied
            } else {
                std::cout << "[O]\t"; // Available
            }
        }
        std::cout << std::endl;
    }
}
void BookingManagerInterface::bookFlight() {
    std::string passengerId;
    std::string flightId;
    std::string seatNumber;
    int paymentTypeChoice;
    std::string paymentType;
    JSON paymentDetails;

    clearInputBuffer();
    std::cout << " ----- Book a Flight ----- " << std::endl;
    displayAllPassengers();

    int maxAttempts = 3;
    int attempts = 0;
    
    std::optional<std::shared_ptr<UserModel>> passengerOpt;
    do {
        attempts++;
        std::cout << "Please enter the Passenger ID to book a flight for: ";
        std::getline(std::cin, passengerId);
        
        if (passengerId.empty()) {
            std::cout << "Passenger ID cannot be empty." << std::endl;
            continue;
        }
        
        passengerOpt = BookingManagerController::getPassengerDetails(currentUser->getUserId(), passengerId);
        if (!passengerOpt.has_value()) {
            std::cout << "Invalid Passenger ID." << std::endl;
        }
    } while ((attempts < maxAttempts) && !passengerOpt.has_value());
    if (!passengerOpt.has_value()) {
        std::cout << "Maximum attempts reached. Aborting booking." << std::endl;
        return;
    }
    
    std::cout << "Passenger selected: " << passengerOpt.value()->getUsername() << std::endl;
    
    displayAllFlights();
    attempts = 0;
    std::optional<std::shared_ptr<FlightModel>> flightOpt;

    do {
        attempts++;
        std::cout << "Please enter the Flight ID to book: ";
        std::getline(std::cin, flightId);

        if (flightId.empty()) {
            std::cout << "Flight ID cannot be empty." << std::endl;
            continue;
        }

        flightOpt = BookingManagerController::getFlightDetails(currentUser->getUserId(), flightId);
        if (!flightOpt.has_value()) {
            std::cout << "Invalid Flight ID." << std::endl;
        }
    } while ((attempts < maxAttempts) && !flightOpt.has_value());
    if (!flightOpt.has_value()) {
        std::cout << "Maximum attempts reached. Aborting booking." << std::endl;
        return;
    }

    std::cout << "Flight selected: " << flightOpt.value() -> getFlightId() << std::endl;

    auto flight = flightOpt.value();
    displaySeatMap(flight -> getSeatMap());
    attempts = 0;

    do {
        attempts++;
        std::cout << "Please enter the Seat Number to book (e.g., 12A): ";
        std::getline(std::cin, seatNumber);
        if (seatNumber.empty()) {
            std::cout << "Seat Number cannot be empty." << std::endl;
            continue;
        }
        try {
            if (!flight -> isValidSeat(seatNumber)) {
                std::cout << "Invalid seat number format. Please try again." << std::endl;
                seatNumber.clear();
                continue;
            }
            if (flight -> getSeatStatus(seatNumber)) {
                std::cout << "Seat is already occupied or invalid. Please choose another seat." << std::endl;
                seatNumber.clear();
                continue;
            }
        }
        catch(const std::exception& e) {
            std::cout << "Error checking seat status: " << e.what() << std::endl;
            seatNumber.clear();
        }
        catch(...) {
            std::cout << "Unknown error occurred while checking seat status." << std::endl;
            seatNumber.clear();
        }

    } while ((attempts < maxAttempts) && seatNumber.empty());
    if (attempts >= maxAttempts || seatNumber.empty()) {
        std::cout << "Maximum attempts reached or invalid seat. Aborting booking." << std::endl;
        return;
    }
    
    std::cout << "Please Select Payment Type: " << std::endl;
    std::cout << "1. Cash" << std::endl;
    std::cout << "2. Credit Card" << std::endl;
    std::cout << "3. PayPal" << std::endl;
    std::cout << "Choice: ";

    while (!(std::cin >> paymentTypeChoice)) {
        std::cin.clear(); // clear error state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
        std::cout << "Invalid input. Please enter a number (1-3): ";
    }
    clearInputBuffer();
    switch (paymentTypeChoice) {
        case 1:
            paymentType = "cash";
            // No additional details needed for cash
            break;
        case 2:
            paymentType = "credit";
            {
                std::string cardNumber, expiryDate, cvv;
                std::cout << "Enter Card Number: ";
                std::getline(std::cin, cardNumber);
                std::cout << "Enter Expiry Date (MM/YY): ";
                std::getline(std::cin, expiryDate);
                std::cout << "Enter CVV: ";
                std::getline(std::cin, cvv);
                paymentDetails["cardNumber"] = cardNumber;
                paymentDetails["expirationDate"] = expiryDate;
                paymentDetails["cvv"] = cvv;
            }
            break;
        case 3:
            paymentType = "paypal";
            {
                std::string paypalEmail;
                std::cout << "Enter PayPal Email: ";
                std::getline(std::cin, paypalEmail);
                paymentDetails["email"] = paypalEmail;
            }
            break;
        default:
            std::cout << "Invalid payment type selected." << std::endl;
            return;
    }
    try {
        auto reservationOpt = BookingManagerController::createReservation(
                currentUser->getUserId(), 
                passengerId, 
                flightId, 
                seatNumber, 
                paymentType, 
                paymentDetails
        );
        if (reservationOpt.has_value()) {
            auto reservation = reservationOpt.value();
            std::cout << "Flight booked successfully! Reservation ID: " << reservation->getReservationId() << std::endl;
            if (reservation && !reservation->getPaymentId().empty()) {
                try {
                    std::string paymentResult = BookingManagerController::processPayment(
                        currentUser->getUserId(), 
                        reservation->getPaymentId()
                    );
                    std::cout << "Payment Status: " << paymentResult << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Payment processing failed: " << e.what() << std::endl;
                    std::cout << "Reservation created but payment needs manual processing." << std::endl;
                }
            } else {
                std::cout << "Warning: No payment ID generated. Manual payment processing required." << std::endl;
            }
        } else {
            std::cout << "Failed to book flight. Please check the details and try again." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "An error occurred while booking the flight: " << e.what() << std::endl;
    }
}
void BookingManagerInterface::modifyBooking() {
    std::string reservationId;
    if(!viewBookings()) {
        return;
    }
    clearInputBuffer();
    std::cout << "Please enter the Reservation ID to modify: ";
    std::getline(std::cin, reservationId);

    auto reservationOpt = BookingManagerController::getReservationDetails(currentUser->getUserId(), reservationId);
    if (!reservationOpt.has_value()) {
        std::cout << "Reservation not found." << std::endl;
        return;
    }
    auto reservation = reservationOpt.value();

    std::cout << "Current Reservation Details:" << std::endl;
    std::cout << "Reservation ID: " << reservation->getReservationId() << std::endl;
    std::cout << "Flight ID: " << reservation->getFlightId() << std::endl;
    std::cout << "Seat Number: " << reservation->getSeatNumber() << std::endl;
    
    auto flightOpt = BookingManagerController::getFlightDetails(currentUser->getUserId(), reservation->getFlightId());
    if (!flightOpt.has_value()) {
        std::cout << "Associated flight not found." << std::endl;
        return;
    }
    auto flight = flightOpt.value();
    displaySeatMap(flight -> getSeatMap());
    std::string newSeatNumber;
    std::cout << "Enter new Seat Number (or press Enter to keep current): ";
    std::getline(std::cin, newSeatNumber);


    if (!newSeatNumber.empty()) {
        reservation->setSeatNumber(newSeatNumber);
        if (!BookingManagerController::updateReservation(currentUser->getUserId(), *reservation)) {
            std::cout << "Failed to update reservation." << std::endl;
            return;
        }
        std::cout << "Reservation modified successfully!" << std::endl;
    }
    else {
        std::cout << "No changes made to the reservation." << std::endl;
    }
}
void BookingManagerInterface::cancelBooking() {
    std::string reservationId;
    if(!viewBookings()) {
        return;
    }
    clearInputBuffer();
    std::cout << "Please enter the Reservation ID to cancel: ";
    std::getline(std::cin, reservationId);

    auto reservationOpt = BookingManagerController::getReservationDetails(currentUser->getUserId(), reservationId);
    if (!reservationOpt.has_value()) {
        std::cout << "Reservation not found." << std::endl;
        return;
    }
    auto reservation = reservationOpt.value();

    std::cout << "Current Reservation Details:" << std::endl;
    std::cout << "Reservation ID: " << reservation->getReservationId() << std::endl;
    std::cout << "Flight ID: " << reservation->getFlightId() << std::endl;
    std::cout << "Seat Number: " << reservation->getSeatNumber() << std::endl;
    
    char confirm;
    std::cout << "Are you sure you want to cancel this reservation? (y/n): ";
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        if (!BookingManagerController::cancelReservation(currentUser->getUserId(), reservationId)) {
            std::cout << "Failed to cancel reservation." << std::endl;
            return;
        }
        std::cout << BookingManagerController::refundPayment(currentUser -> getUserId(), reservation -> getPaymentId()) << std::endl;
        std::cout << "Reservation cancelled successfully!" << std::endl;
    } else {
        std::cout << "Cancellation aborted." << std::endl;
    }
}