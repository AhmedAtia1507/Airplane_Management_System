#include "../include/UserInterface.hpp"
#include "../include/AdminInterface.hpp"
#include "../include/BookingManagerInterface.hpp"
#include "../include/PassengerInterface.hpp"
#include "../../Controller/include/AuthController.hpp"
#include <iostream>

/**
 * @brief Displays the main menu of the Airline Management System to the user.
 *
 * This function outputs the main menu options to the console, allowing the user
 * to choose between logging into the system or exiting the application.
 */
void UserInterface::displayMainMenu() {
    std::cout << "Welcome to the Airline Management System" << std::endl;
    std::cout << "Please enter your choice:" << std::endl;
    std::cout << "1. Login to the system" << std::endl;
    std::cout << "2. Exit" << std::endl;
}
/**
 * @brief Handles the user login process.
 *
 * Prompts the user to enter their username and password, then attempts to authenticate
 * using the AuthController. If authentication fails, displays an error message.
 * On successful login, greets the user and redirects them to the appropriate interface
 * based on their role (Passenger, Admin, or Booking Manager). If the user role is unknown,
 * access is denied.
 */
void UserInterface::handleLogin() {
    std::string username;
    std::string password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    auto userOpt = AuthController::login(username, password);
    if (!userOpt.has_value()) {
        std::cout << "Login failed. Please try again." << std::endl;
        return;
    }
    auto user = userOpt.value();
    std::cout << "Login successful! Welcome, " << user -> getUsername() << "." << std::endl;

    switch (user -> getRole()) {
        case UserModel::UserType::Passenger:
            std::cout << "Redirecting to Passenger Interface..." << std::endl;
            {
                PassengerInterface passengerInterface(std::dynamic_pointer_cast<Passenger>(user));
                passengerInterface.startInterface();
            }
            break;
        case UserModel::UserType::Admin:
            std::cout << "Redirecting to Admin Interface..." << std::endl;
            {
                AdminInterface adminInterface(std::dynamic_pointer_cast<Admin>(user));
                adminInterface.startInterface();
            }
            break;
        case UserModel::UserType::BookingManager:
            std::cout << "Redirecting to Booking Manager Interface..." << std::endl;
            {
                BookingManagerInterface bookingManagerInterface(std::dynamic_pointer_cast<BookingManager>(user));
                bookingManagerInterface.startInterface();
            }
            break;
        default:
            std::cout << "Unknown user role. Access denied." << std::endl;
            break;
    }
}
/**
 * @brief Starts the main program loop for the user interface.
 *
 * Displays the main menu and processes user input until the exit option is selected.
 * Handles user login and program termination based on user choices.
 * If an invalid choice is entered, prompts the user to try again.
 */
void UserInterface::startProgram() {
    int choice = 0;
    while (choice != EXIT_OPTION) {
        displayMainMenu();
        std::cin >> choice;
    
        switch (choice) {
            case LOGIN_OPTION:
                handleLogin();
                break;
            case EXIT_OPTION:
                std::cout << "Exiting the program. Goodbye!" << std::endl;
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
}