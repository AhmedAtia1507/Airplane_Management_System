#pragma once

#include <memory>
#include "../../Model/include/BookingManager.hpp"

/**
 * @brief Interface class for managing booking operations through a command-line interface.
 * 
 * The BookingManagerInterface class provides a user-friendly CLI for booking managers
 * to interact with the airplane booking system. It handles user input, displays menus,
 * and manages various booking-related operations such as searching flights, making
 * reservations, and managing existing bookings.
 * 
 * @details This class serves as a wrapper around the BookingManager functionality,
 * providing a clean separation between business logic and user interface. It includes
 * methods for displaying seat maps, handling user input validation, and managing
 * the overall user experience for booking managers.
 * 
 * Key features:
 * - Flight search and booking capabilities
 * - Booking modification and cancellation
 * - Passenger and flight information display
 * - Interactive seat map visualization
 * - Input validation and error handling
 * 
 * @note This class is designed for booking manager users and provides administrative
 * level access to the booking system.
 */
class BookingManagerInterface {
    std::shared_ptr<BookingManager> currentUser;
    void clearInputBuffer();

    void displayBookingManagerMenu();
    void displaySeatMap(const std::vector<std::vector<bool>>& seatMap);
    void searchFlights();
    bool viewBookings();
    void bookFlight();
    void modifyBooking();
    void cancelBooking();
    void displayAllPassengers();
    void displayAllFlights();
    constexpr static int SEARCH_FLIGHTS_OPTION = 1;
    constexpr static int VIEW_BOOKINGS_OPTION = 2;
    constexpr static int BOOK_FLIGHT_OPTION = 3;
    constexpr static int MODIFY_BOOKING_OPTION = 4;
    constexpr static int CANCEL_BOOKING_OPTION = 5;
    constexpr static int LOGOUT_OPTION = 6;

    public:
        BookingManagerInterface(const std::shared_ptr<BookingManager>& bookingManager);
        void startInterface();
};