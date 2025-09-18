#include "CLI/include/UserInterface.hpp"


#include <csignal>
#include <iostream>

// Global cleanup function
void cleanup() {
    std::cout << "\nSaving data before exit..." << std::endl;
}

// Signal handler
void signalHandler(int signal) {
    std::string signalName;
    switch(signal) {
        case SIGINT: signalName = "CTRL+C"; break;
        case SIGTERM: signalName = "terminate"; break;
        #ifdef SIGBREAK
            case SIGBREAK: signalName = "CTRL+BREAK"; break;
        #endif
        default: signalName = "unknown"; break;
    }
    
    std::cout << "\nReceived " << signalName << " signal..." << std::endl;
    cleanup();
    std::exit(0);
}
int main() {
    // Register signal handler
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    #ifdef SIGBREAK
        std::signal(SIGBREAK, signalHandler);
    #endif
    try {
        UserInterface ui;
        ui.startProgram();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
    }
    return 0;
}