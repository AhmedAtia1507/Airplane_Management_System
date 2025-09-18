#pragma once

/**
 * @class UserInterface
 * @brief Provides the command-line user interface for the application.
 *
 * This class manages the main menu display, user login handling, and program flow control.
 * It defines constants for menu options and exposes a method to start the program.
 *
 * @method displayMainMenu Displays the main menu to the user.
 * @method handleLogin Handles user login functionality.
 * @method startProgram Starts the main program loop and manages user interactions.
 *
 * @var LOGIN_OPTION Constant representing the login menu option.
 * @var EXIT_OPTION Constant representing the exit menu option.
 */
class UserInterface {
    void displayMainMenu();
    void handleLogin();
    constexpr static int LOGIN_OPTION = 1;
    constexpr static int EXIT_OPTION = 2;
    
    public:
        void startProgram();
};