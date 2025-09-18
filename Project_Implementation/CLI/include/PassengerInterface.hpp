#pragma once

#include <memory>
#include "../../Model/include/Passenger.hpp"

/**
 * @brief Interface class for passenger operations in the airline management system.
 * 
 * This class provides a command-line interface for passengers to interact with
 * the airline system. It allows passengers to search for flights, view their
 * reservations, and manage their booking activities.
 * 
 * The interface maintains a reference to the current passenger user and provides
 * menu-driven navigation for various passenger operations.
 */
class PassengerInterface {
    std::shared_ptr<Passenger> currentUser;
    
    void clearInputBuffer();
    void displayPassengerMenu();
    void searchFlights();
    void viewReservations();
    void displayExistingFlights();
    void displaySeatMap(const std::vector<std::vector<bool>>& seatMap);

    constexpr static int SEARCH_FLIGHTS_OPTION = 1;
    constexpr static int VIEW_RESERVATIONS_OPTION = 2;
    constexpr static int LOGOUT_OPTION = 3;
    public:
        PassengerInterface(const std::shared_ptr<Passenger>& passenger);
        void startInterface();
};