#include "../include/PassengerInterface.hpp"
#include "../../Controller/include/PassengerController.hpp"
#include <iostream>

PassengerInterface::PassengerInterface(const std::shared_ptr<Passenger>& passenger) : currentUser(passenger) {}

void PassengerInterface::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void PassengerInterface::startInterface() {
    int choice;
    do {
        displayPassengerMenu();
        std::cin >> choice;

        switch (choice) {
            case SEARCH_FLIGHTS_OPTION:
                // Search Flights
                searchFlights();
                break;
            case VIEW_RESERVATIONS_OPTION:
                // View Reservations
                viewReservations();
                break;
            case LOGOUT_OPTION:
                std::cout << "Logging out..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != LOGOUT_OPTION);
}

void PassengerInterface::displayPassengerMenu() {
    std::cout << "Passenger Menu - Please choose an option:" << std::endl;
    std::cout << "1. Search Flights" << std::endl;
    std::cout << "2. View Reservations" << std::endl;
    std::cout << "3. Logout" << std::endl;
    std::cout << "Choice: ";
}

void PassengerInterface::searchFlights() {
    std::string origin;
    std::string destination;
    std::string departureDateStr;
    std::string flightChoice;
    std::string seatNumber;
    
    clearInputBuffer();
    std::cout << " ----- Search Flights ----- " << std::endl;
    std::cout << "Please enter the origin of the flight: ";
    std::getline(std::cin, origin);
    std::cout << "Please enter the destination of the flight: ";
    std::getline(std::cin, destination);
    std::cout << "Please enter the departure date (YYYY-MM-DD): ";
    std::getline(std::cin, departureDateStr);

    DateTime departureDate(departureDateStr);
    auto flights = PassengerController::getFlightsByRouteAndDate(currentUser->getUserId(), origin, destination, departureDate);
    
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
    
    std::cout << "Enter the Flight Number you wish to book (or '0' to cancel): ";
    std::getline(std::cin, flightChoice);
    if (flightChoice == "0") {
        std::cout << "Booking cancelled." << std::endl;
        return;
    }
    
    auto flight = PassengerController::getFlightDetails(currentUser->getUserId(), flightChoice);
    if (!flight.has_value()) {
        std::cout << "Invalid Flight ID." << std::endl;
        return;
    }
    
    displaySeatMap(flight.value() -> getSeatMap());
    std::cout << "Enter the Seat Number you wish to book (e.g., 12A): ";
    std::getline(std::cin, seatNumber);
    
    std::cout << "Please choose the payment method:" << std::endl;
    std::cout << "1. Credit Card" << std::endl;
    std::cout << "2. PayPal" << std::endl;
    std::cout << "Choice: ";
    int paymentChoice;
    std::cin >> paymentChoice;
    std::string paymentType;
    JSON paymentDetails;
    
    while (!(std::cin >> paymentChoice)) {
        std::cin.clear(); // clear error state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
        std::cout << "Invalid input. Please enter a number (1-2): ";
    }
    clearInputBuffer();
    switch (paymentChoice) {
        case 1:
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
        case 2:
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
        auto reservationOpt = PassengerController::bookFlight(
            currentUser->getUserId(), 
            flightChoice, 
            seatNumber, 
            paymentType, 
            paymentDetails
        );
        if (reservationOpt.has_value()) {
            auto reservation = reservationOpt.value();
            std::cout << "Flight booked successfully! Reservation ID: " << reservation->getReservationId() << std::endl;
            if (reservation && !reservation->getPaymentId().empty()) {
                try {
                    std::string paymentResult = PassengerController::processPayment(
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

void PassengerInterface::viewReservations() {
    clearInputBuffer();
    std::cout << " ----- View Reservations ----- " << std::endl;
    auto reservations = PassengerController::getPassengerReservations(currentUser->getUserId());
    if (reservations.empty()) {
        std::cout << "No reservations found." << std::endl;
        return;
    }
    
    std::cout << "Your Reservations:" << std::endl;
    int index = 1;
    for (const auto& reservation : reservations) {
        std::cout << index << ". Reservation ID: " << reservation->getReservationId() << std::endl;
        std::cout << "   Flight ID: " << reservation->getFlightId() <<  std::endl;
        std::cout << "   Seat Number: " << reservation->getSeatNumber() << std::endl;
        std::cout << "   Status: " << (reservation->getStatus() == ReservationModel::ReservationStatus::CONFIRMED ? "Confirmed" : "Cancelled") << std::endl;
        std::cout << "------------------------" << std::endl;
        index++;
    }
}

void PassengerInterface::displaySeatMap(const std::vector<std::vector<bool>>& seatMap) {
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