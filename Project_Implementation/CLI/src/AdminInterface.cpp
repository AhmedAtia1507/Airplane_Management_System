#include "../include/AdminInterface.hpp"
#include "../../Controller/include/AdminController.hpp"
#include <iostream>
#include <stdexcept>
#include <functional>

AdminInterface::AdminInterface(const std::shared_ptr<Admin>& admin) : currentUser(admin) {}


void AdminInterface::displayAdminMenu() {
    std::cout << "Admin Interface - Please choose an option:" << std::endl;
    std::cout << "1. Manage Flights" << std::endl;
    std::cout << "2. Manage Aircrafts" << std::endl;
    std::cout << "3. Manage Users" << std::endl;
    std::cout << "4. Logout" << std::endl;
    std::cout << "Choice: ";
}

void AdminInterface::startInterface() {
    int choice = 0;
    while (choice != LOGOUT_OPTION) {
        displayAdminMenu();
        std::cin >> choice;
        
        switch (choice) {
            case MANAGE_FLIGHTS_OPTION:
                // Manage Flights
                handleFlights();
                break;
            case MANAGE_AIRCRAFTS_OPTION:
                // Manage Aircrafts
                handleAircrafts();
                break;
            case MANAGE_USERS_OPTION:
                // Manage Users
                handleUsers();
                break;
            case LOGOUT_OPTION:
                std::cout << "Logging out..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
}
/****************************************************** Flight Management ******************************************** */

void AdminInterface::displayManageFlightsMenu() {
    std::cout << "Manage Flights Menu - Please choose an option:" << std::endl;
    std::cout << "1. Add Flight" << std::endl;
    std::cout << "2. Update Flight" << std::endl;
    std::cout << "3. Remove Flight" << std::endl;
    std::cout << "4. View Flights" << std::endl;
    std::cout << "5. Assign Crew to Flight" << std::endl;
    std::cout << "6. Remove Crew from Flight" << std::endl;
    std::cout << "7. Back to Admin Menu" << std::endl;
    std::cout << "Choice: ";
}

void AdminInterface::handleFlights() {
    int choice = 0;
    while (choice != FLIGHT_BACK_OPTION) {
        displayManageFlightsMenu();
        std::cin >> choice;

        switch (choice) {
            case ADD_FLIGHT_OPTION:
                addFlightOption();
                break;
            case UPDATE_FLIGHT_OPTION:
                // Update Flight
                updateExistingFlight();
                break;
            case REMOVE_FLIGHT_OPTION:
                // Remove Flight
                removeExistingFlight();
                break;
                case VIEW_FLIGHTS_OPTION:
                // View Flights
                std::cout << " ----- View All Flights ----- " << std::endl;
                displayExistingFlights();
                break;
            case ASSIGN_CREW_OPTION:
                // Assign Crew to Flight
                assignCrewToFlight();
                break;
            case REMOVE_CREW_OPTION:
                // Remove Crew from Flight
                removeCrewMemberFromFlight();
                break;
            case FLIGHT_BACK_OPTION:
                std::cout << "Going back to Admin Menu..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
}

void AdminInterface::addFlightOption() {
    std::string origin;
    std::string destination;
    std::string departureTimeStr;
    std::string arrivalTimeStr;
    std::string aircraftId;
    std::string crewIdsStr;
    std::vector<std::string> crewMemberIds;
    int attempts = 0;

    std::cout << " ----- Add New Flight ----- " << std::endl;
    do {
        attempts++;
        std::cout << "Enter Origin: ";
        std::getline(std::cin, origin);
        if (origin.empty()) {
            std::cout << "Origin cannot be empty. Please try again." << std::endl;
        }
    } while (attempts < 3 && origin.empty());
    if (origin.empty()) {
        std::cout << "Maximum number of attempts reached due to empty or invalid Crew IDs. Exiting..." << std::endl;
        return;
    }

    attempts = 0;
    do {
        attempts++;
        std::cout << "Enter Destination: ";
        std::getline(std::cin, destination);
        if (destination.empty()) {
            std::cout << "Destination cannot be empty. Please try again." << std::endl;
        }
    } while (attempts < 3 && destination.empty());
    if (destination.empty()) {
        std::cout << "Maximum number of attempts reached. Exiting..." << std::endl;
        return;
    }

    attempts = 0;
    do {
        attempts++;
        std::cout << "Enter Departure Date & Time (YYYY-MM-DD HH:MM): ";
        std::getline(std::cin, departureTimeStr);
        if (departureTimeStr.empty()) {
            std::cout << "Departure Date & Time cannot be empty. Please try again." << std::endl;
        }
    } while (attempts < 3 && departureTimeStr.empty());
    if (departureTimeStr.empty()) {
        std::cout << "Maximum number of attempts reached. Exiting..." << std::endl;
        return;
    }

    attempts = 0;
    do {
        attempts++;
        std::cout << "Enter Arrival Date & Time (YYYY-MM-DD HH:MM): ";
        std::getline(std::cin, arrivalTimeStr);
        if (arrivalTimeStr.empty()) {
            std::cout << "Arrival Date & Time cannot be empty. Please try again." << std::endl;
        }
    } while (attempts < 3 && arrivalTimeStr.empty());
    if (arrivalTimeStr.empty()) {
        std::cout << "Maximum number of attempts reached. Exiting..." << std::endl;
        return;
    }

    attempts = 0;
    if(!displayAllAircrafts()) {
        return;
    }
    do {
        attempts++;
        std::cout << "Enter Aircraft ID: ";
        std::getline(std::cin, aircraftId);
        if (aircraftId.empty()) {
            std::cout << "Aircraft ID cannot be empty. Please try again." << std::endl;
        }
        else if (!AdminController::getAircraftById(currentUser -> getUserId(), aircraftId).has_value()) {
            std::cout << "Invalid Aircraft ID. Please try again." << std::endl;
            aircraftId.clear();
        }
    } while (attempts < 3 && aircraftId.empty());
    if (aircraftId.empty()) {
        std::cout << "Maximum number of attempts reached. Exiting..." << std::endl;
        return;
    }
    if (!AdminController::getAircraftById(currentUser -> getUserId(), aircraftId).has_value()) {
        std::cout << "Invalid Aircraft ID. Exiting..." << std::endl;
        return;
    }


    displayCrewMembers(CrewMemberModel::CrewType::Pilot);
    std::cout << "Enter Pilot Crew IDs (comma separated) or leave empty if you want to assign it later: ";
    std::getline(std::cin >> std::ws, crewIdsStr);
    if (!crewIdsStr.empty()) {
        std::stringstream ss(crewIdsStr);
        std::string crewId;
        while (std::getline(ss, crewId, ',')) {
            crewId.erase(0, crewId.find_first_not_of(" \t\n\r\f\v"));
            crewId.erase(crewId.find_last_not_of(" \t\n\r\f\v") + 1);
            if (!AdminController::getCrewMemberById(currentUser -> getUserId(), crewId).has_value()) {
                std::cout << "Warning: Crew ID " << crewId << " does not exist and will be ignored." << std::endl;
                continue;
            }
            crewMemberIds.push_back(crewId);
        }
    }

    displayCrewMembers(CrewMemberModel::CrewType::FlightAttendant);
    std::cout << "Enter Flight Attendant Crew IDs (comma separated) or leave empty if you want to assign it later: ";
    std::getline(std::cin >> std::ws, crewIdsStr);
    if (!crewIdsStr.empty()) {
        std::stringstream ss(crewIdsStr);
        std::string crewId;
        while (std::getline(ss, crewId, ',')) {
            crewId.erase(0, crewId.find_first_not_of(" \t\n\r\f\v"));
            crewId.erase(crewId.find_last_not_of(" \t\n\r\f\v") + 1);
            if (!AdminController::getCrewMemberById(currentUser -> getUserId(), crewId).has_value()) {
                std::cout << "Warning: Crew ID " << crewId << " does not exist and will be ignored." << std::endl;
                continue;
            }
            crewMemberIds.push_back(crewId);
        }
    }

    try {
        // Call the controller to add the flight
        auto newFlight = AdminController::addFlight(
            currentUser -> getUserId(), 
            origin, 
            destination, 
            departureTimeStr, 
            arrivalTimeStr, 
            aircraftId,
            crewMemberIds
        );
        
        if (newFlight.has_value()) {
            std::cout << "Flight added successfully! Flight ID: " << newFlight.value() -> getFlightId() << std::endl;
        } else {
            std::cout << "Failed to add flight. Please check the details and try again." << std::endl;
        }
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void AdminInterface::displayCrewMembers(const CrewMemberModel::CrewType& role) {
    auto crewMembers = AdminController::getAllCrewMembers(currentUser -> getUserId(), role);
    if (crewMembers.empty()) {
        std::cout << "No crew members available." << std::endl;
        return;
    }
    switch(role) {
        case CrewMemberModel::CrewType::Pilot:
            std::cout << "Available Pilots:" << std::endl;
            break;
        case CrewMemberModel::CrewType::FlightAttendant:
            std::cout << "Available Flight Attendants:" << std::endl;
            break;
        default:
            std::cout << "Available Crew Members:" << std::endl;
            break;
    }
    int index = 1;
    for (const auto& crewMember : crewMembers) {
        std::cout << index << ". Crew ID: " << crewMember -> getCrewId() << ", Name: " << crewMember -> getName() << std::endl;
        index++;
    }
}
void AdminInterface::assignCrewToFlight() {
    std::cout << " ----- Assign Crew to Flight ----- " << std::endl;
    displayExistingFlights();
    std::string flightId;
    std::cout << "Please enter the Flight ID to assign crew: ";
    while((std::cin >> flightId).fail() || flightId.empty()) {
        std::cin.clear(); // clear the fail state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
        std::cout << "Flight ID cannot be empty. Please enter a valid Flight ID: ";
    }

    std::vector<std::string> crewMemberIds;
    std::string crewIdsStr;
    int attempts = 0;
    displayCrewMembers(CrewMemberModel::CrewType::Pilot);
    do {
        attempts++;
        std::cout << "Enter Pilot Crew IDs (comma separated): ";
        if (!std::getline(std::cin >> std::ws, crewIdsStr) || crewIdsStr.empty()) {
            std::cout << "Crew IDs cannot be empty. Please try again." << std::endl;
            crewIdsStr.clear();
            continue;
        }
        std::stringstream ss(crewIdsStr);
        std::string crewId;
        while (std::getline(ss, crewId, ',')) {
            crewId.erase(0, crewId.find_first_not_of(" \t\n\r\f\v"));
            crewId.erase(crewId.find_last_not_of(" \t\n\r\f\v") + 1);
            if (!AdminController::getCrewMemberById(currentUser -> getUserId(), crewId).has_value()) {
                std::cout << "Error: Crew ID " << crewId << " does not exist." << std::endl;
                crewMemberIds.clear();
                crewIdsStr.clear();
                break;
            }
            crewMemberIds.push_back(crewId);
        }
    } while (attempts < 3 && crewIdsStr.empty());
    if (crewIdsStr.empty()) {
        std::cout << "Maximum number of attempts reached. Exiting..." << std::endl;
        return;
    }
    
    attempts = 0;
    displayCrewMembers(CrewMemberModel::CrewType::FlightAttendant);
    do {
        attempts++;
        std::cout << "Enter Flight Attendant Crew IDs (comma separated): ";
        if (!std::getline(std::cin >> std::ws, crewIdsStr) || crewIdsStr.empty()) {
            std::cout << "Crew IDs cannot be empty. Please try again." << std::endl;
            crewIdsStr.clear();
            continue;
        }
        std::stringstream ss(crewIdsStr);
        std::string crewId;
        while (std::getline(ss, crewId, ',')) {
            crewId.erase(0, crewId.find_first_not_of(" \t\n\r\f\v"));
            crewId.erase(crewId.find_last_not_of(" \t\n\r\f\v") + 1);
            if (!AdminController::getCrewMemberById(currentUser -> getUserId(), crewId).has_value()) {
                std::cout << "Error: Crew ID " << crewId << " does not exist." << std::endl;
                crewIdsStr.clear();
                break;
            }
            crewMemberIds.push_back(crewId);
        }
    } while (attempts < 3 && crewIdsStr.empty());
    if (crewIdsStr.empty()) {
        std::cout << "Maximum number of attempts reached. Exiting..." << std::endl;
        return;
    }

    try {
        if (AdminController::assignCrewToFlight(currentUser -> getUserId(), flightId, crewMemberIds)) {
            std::cout << "Crew assigned successfully to flight " << flightId << "!" << std::endl;
        } else {
            std::cout << "Failed to assign crew. Please check the details and try again." << std::endl;
        }
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

// Option 1: Return void (recommended for display function)
void AdminInterface::displayCrewMembersOfFlight() {
    std::cout << " ----- View Crew Members of Flight ----- " << std::endl;
    displayExistingFlights();
    std::string flightId;
    std::cout << "Please enter the Flight ID to view assigned crew: ";
    while((std::cin >> flightId).fail() || flightId.empty()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Flight ID cannot be empty. Please enter a valid Flight ID: ";
    }
    std::cin.ignore(); // Clear buffer
    
    auto crewMembers = AdminController::getCrewMembersOfFlight(currentUser->getUserId(), flightId);
    if (crewMembers.empty()) {
        std::cout << "No crew members assigned to this flight." << std::endl;
        return;
    }
    
    std::cout << "Crew Members of Flight " << flightId << ":" << std::endl;
    int index = 1;
    for (const auto& crewMember : crewMembers) {
        std::cout << index << ". Crew ID: " << crewMember->getCrewId() << std::endl;
        std::cout << "   Name: " << crewMember->getName() << std::endl;
        std::cout << "   Role: " << (crewMember->getRole() == CrewMemberModel::CrewType::Pilot ? "Pilot" : "Flight Attendant") << std::endl;
        index++;
    }
}
void AdminInterface::removeCrewMemberFromFlight() {
    std::cout << " ----- Remove Crew Member from Flight ----- " << std::endl;
    
    // Step 1: Select flight
    displayExistingFlights();
    std::string flightId;
    std::cout << "Please enter the Flight ID to remove crew from: ";
    while((std::cin >> flightId).fail() || flightId.empty()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Flight ID cannot be empty. Please enter a valid Flight ID: ";
    }
    std::cin.ignore(); // Clear the input buffer
    
    // Step 2: Display crew members for the selected flight
    auto crewMembers = AdminController::getCrewMembersOfFlight(currentUser->getUserId(), flightId);
    if (crewMembers.empty()) {
        std::cout << "No crew members assigned to this flight." << std::endl;
        return;
    }
    
    std::cout << "Crew Members of Flight " << flightId << ":" << std::endl;
    int index = 1;
    for (const auto& crewMember : crewMembers) {
        std::cout << index << ". Crew ID: " << crewMember->getCrewId() << std::endl;
        std::cout << "   Name: " << crewMember->getName() << std::endl;
        std::cout << "   Role: " << (crewMember->getRole() == CrewMemberModel::CrewType::Pilot ? "Pilot" : "Flight Attendant") << std::endl;
        index++;
    }
    
    // Step 3: Select crew member to remove
    std::string crewMemberId;
    std::cout << "Please enter the Crew ID to remove: ";
    while((std::cin >> crewMemberId).fail() || crewMemberId.empty()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Crew ID cannot be empty. Please enter a valid Crew ID: ";
    }
    
    // Step 4: Confirm removal
    std::cout << "Are you sure you want to remove crew member " << crewMemberId 
              << " from flight " << flightId << "? (y/n): ";
    char confirmation;
    std::cin >> confirmation;
    
    if (confirmation != 'y' && confirmation != 'Y') {
        std::cout << "Operation cancelled." << std::endl;
        return;
    }
    
    // Step 5: Call controller to remove crew member
    try {
        if (AdminController::removeCrewMemberFromFlight(currentUser->getUserId(), flightId, crewMemberId)) {
            std::cout << "Crew member removed successfully from flight " << flightId << "!" << std::endl;
        } else {
            std::cout << "Failed to remove crew member. Please check the details and try again." << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
bool AdminInterface::displayAllAircrafts() {
    auto aircrafts = AdminController::getAllAircrafts(currentUser -> getUserId());
    if (aircrafts.empty()) {
        std::cout << "No aircrafts available. Please add an aircraft first." << std::endl;
        return false;
    }
    std::cout << "Here is all the aircrafts available:" << std::endl;
    int index = 1;
    for (const auto& aircraft : aircrafts) {
        std::cout << index << ". Aircraft ID: " << aircraft -> getAircraftId() << std::endl;
        std::cout << "   Model: " << aircraft -> getModel() << std::endl;
        std::cout << "   Capacity: " << aircraft -> getCapacity() << std::endl;
        index++;
    }
    return true;
}
bool AdminInterface::displayExistingFlights() {
    auto flights = AdminController::getAllFlights(currentUser -> getUserId());
    if (flights.empty()) {
        std::cout << "No flights available." << std::endl;
        return false;
    }
    std::cout << "Here are the existing flights:" << std::endl;
    int index = 1;
    for (const auto& flight : flights) {
        std::cout << index << ". Flight ID: " << flight -> getFlightId() << std::endl;
        std::cout << "   Origin: " << flight -> getOrigin() << std::endl;
        std::cout << "   Destination: " << flight -> getDestination() << std::endl;
        std::cout << "   Departure: " << flight -> getDepartureTime().toString() << std::endl;
        std::cout << "   Arrival: " << flight -> getArrivalTime().toString() << std::endl;
        std::cout << "   Aircraft ID: " << flight -> getAircraftId() << std::endl;
        
        // get crew members for the flight
        auto crewMembers = AdminController::getCrewMembersOfFlight(currentUser->getUserId(), flight->getFlightId());
        if (!crewMembers.empty()) {
            std::cout << "   Crew Members: ";
            for (std::size_t index = 0; index < crewMembers.size(); index++) {
                std::cout << crewMembers[index]->getName() << " (" 
                          << (crewMembers[index]->getRole() == CrewMemberModel::CrewType::Pilot ? "Pilot" : "Flight Attendant") 
                          << ")";
                if (index < crewMembers.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
        index++;
    }
    return true;
}

void AdminInterface::updateExistingFlight() {
    std::cout << " ----- Update Existing Flight ----- " << std::endl;
    if (!displayExistingFlights()) {
        return;
    }
    std::string flightId = "";
    std::cout << "Please enter the Flight ID to update: ";
    while((std::cin >> flightId).fail() || flightId.empty()) {
        std::cin.clear(); // clear the fail state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
        std::cout << "Flight ID cannot be empty. Please enter a valid Flight ID: ";
    }
    
    // Get the flight details
    auto flightOpt = AdminController::getFlightById(currentUser -> getUserId(), flightId);
    if (!flightOpt.has_value()) {
        std::cout << "Invalid flight selection. Please try again." << std::endl;
        return;
    }
    
    auto flight = flightOpt.value();
    
    // Display current flight details
    std::cout << "Current Flight Details:" << std::endl;
    std::cout << "   Flight ID: " << flight -> getFlightId() << std::endl;
    std::cout << "   Origin: " << flight -> getOrigin() << std::endl;
    std::cout << "   Destination: " << flight -> getDestination() << std::endl;
    std::cout << "   Departure: " << flight -> getDepartureTime().toString() << std::endl;
    std::cout << "   Arrival: " << flight -> getArrivalTime().toString() << std::endl;
    std::cout << "   Aircraft ID: " << flight -> getAircraftId() << std::endl;
    
    // Get new details
    std::string newOrigin, newDestination, newDepartureTime, newArrivalTime, newAircraftId;
    std::cout << "Enter new Origin (leave blank to keep current): ";
    std::cin.ignore();
    std::getline(std::cin, newOrigin);
    std::cout << "Enter new Destination (leave blank to keep current): ";
    std::getline(std::cin, newDestination);
    std::cout << "Enter new Departure Time (leave blank to keep current): ";
    std::getline(std::cin, newDepartureTime);
    std::cout << "Enter new Arrival Time (leave blank to keep current): ";
    std::getline(std::cin, newArrivalTime);
    std::cout << "Enter new Aircraft ID (leave blank to keep current): ";
    std::getline(std::cin, newAircraftId);
    
    try {
        // Call the controller to update the flight
        if (AdminController::updateFlight(
            currentUser -> getUserId(),
            flight -> getFlightId(),
            newOrigin.empty() ? flight -> getOrigin() : newOrigin,
            newDestination.empty() ? flight -> getDestination() : newDestination,
            newDepartureTime.empty() ? flight -> getDepartureTime() : newDepartureTime,
            newArrivalTime.empty() ? flight -> getArrivalTime() : newArrivalTime,
            newAircraftId.empty() ? flight -> getAircraftId() : newAircraftId
        )) {
            std::cout << "Flight updated successfully!" << std::endl;
        } else {
            std::cout << "Failed to update flight. Please check the details and try again." << std::endl;
        }
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void AdminInterface::removeExistingFlight() {
    std::cout << " ----- Remove Existing Flight ----- " << std::endl;
    if(!displayExistingFlights()) {
        return;
    }
    std::string flightId = "";
    std::cout << "Please enter the Flight ID to remove: ";
    while((std::cin >> flightId).fail() || flightId.empty()) {
        std::cin.clear(); // clear the fail state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
        std::cout << "Flight ID cannot be empty. Please enter a valid Flight ID: ";
    }

    // Call the controller to remove the flight
    if (AdminController::removeFlight(currentUser -> getUserId(), flightId)) {
        std::cout << "Flight removed successfully!" << std::endl;
    } else {
        std::cout << "Failed to remove flight. Please check the details and try again." << std::endl;
    }
}

/********************************************** Aircraft Management **********************************************/

void AdminInterface::displayManageAircraftsMenu() {
    std::cout << "Manage Aircrafts Menu - Please choose an option:" << std::endl;
    std::cout << "1. Add Aircraft" << std::endl;
    std::cout << "2. Update Aircraft" << std::endl;
    std::cout << "3. Remove Aircraft" << std::endl;
    std::cout << "4. View Aircrafts" << std::endl;
    std::cout << "5. Back to Admin Menu" << std::endl;
}

void AdminInterface::handleAircrafts() {
    int choice = 0;
    while (choice != BACK_OPTION) {
        displayManageAircraftsMenu();
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case ADD_AIRCRAFT_OPTION:
                // Add Aircraft
                addAircraft();
                break;
            case UPDATE_AIRCRAFT_OPTION:
                // Update Aircraft
                updateExistingAircraft();
                break;
            case REMOVE_AIRCRAFT_OPTION:
                // Remove Aircraft
                removeExistingAircraft();
                break;
            case VIEW_AIRCRAFTS_OPTION:
                std::cout << " ----- View All Aircrafts ----- " << std::endl;
                // View Aircrafts
                displayExistingAircrafts();
                break;
            case BACK_OPTION:
                // Back to Admin Menu
                std::cout << "Going back to Admin Menu..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
            
        }
    }
}
void AdminInterface::addAircraft() {
    std::string model;
    int capacity;
    int numOfRowSeats;
    
    std::cout << " ----- Add New Aircraft ----- " << std::endl;
    std::cout << "Enter Model: ";
    getline(std::cin >> std::ws, model); // Use std::ws to consume any leading whitespace
    std::cout << "Enter Capacity: ";
    while (!(std::cin >> capacity) || capacity <= 0) {
        std::cout << "Invalid input. Please enter a positive number: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cout << "Enter Number of Row Seats: ";
    std::cin >> numOfRowSeats;
    
    try {
        // Call the controller to add the aircraft
        auto newAircraft = AdminController::addAircraft(
            currentUser -> getUserId(), 
            model, 
            capacity, 
            numOfRowSeats
        );
        
        if (newAircraft.has_value()) {
            std::cout << "Aircraft added successfully! Aircraft ID: " << newAircraft.value() -> getAircraftId() << std::endl;
        } else {
            std::cout << "Failed to add aircraft. Please check the details and try again." << std::endl;
        }
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

bool AdminInterface::displayExistingAircrafts() {
    auto aircrafts = AdminController::getAllAircrafts(currentUser -> getUserId());
    if (aircrafts.empty()) {
        std::cout << "No aircrafts available." << std::endl;
        return false;
    }
    std::cout << "Here are the existing aircrafts:" << std::endl;
    int index = 1;
    for (const auto& aircraft : aircrafts) {
        std::cout << index << ". Aircraft ID: " << aircraft -> getAircraftId() << std::endl;
            std::cout << "   Model: " << aircraft -> getModel() << std::endl;
            std::cout << "   Capacity: " << aircraft -> getCapacity() << std::endl;
            std::cout << "   Number of Seats in each row: " << aircraft -> getNumOfRowSeats() << std::endl;
            index++;
    }
    return true;
}

void AdminInterface::updateExistingAircraft() {
    std::cout << " ----- Update Existing Aircraft ----- " << std::endl;
    if(!displayExistingAircrafts()) {
        return;
    }
    std::string aircraftId;
    std::cout << "Please enter the Aircraft ID to update: ";
    std::cin >> aircraftId;
        
    // Fetch the existing aircraft details
    auto existingAircraftOpt = AdminController::getAircraftById(currentUser -> getUserId(), aircraftId);
    if (!existingAircraftOpt.has_value()) {
        std::cout << "Aircraft not found." << std::endl;
        return;
    }
    auto existingAircraft = existingAircraftOpt.value();
    // Display current details
    std::cout << "Current Aircraft Details:" << std::endl;
    std::cout << "Model: " << existingAircraft -> getModel() << std::endl;
    std::cout << "Capacity: " << existingAircraft -> getCapacity() << std::endl;
    std::cout << "Number of Seats in each row: " << existingAircraft -> getNumOfRowSeats() << std::endl;
    
    // Get new details
    std::string newModel;
    int newCapacity;
    int newNumOfRowSeats;
    
    std::cout << "Enter new Model (leave empty to keep current): ";
    std::cin.ignore();
    std::getline(std::cin, newModel);
    std::cout << "Enter new Capacity (leave empty to keep current): ";
    std::cin >> newCapacity;
    std::cout << "Enter new number of seats in each row (leave empty to keep current): ";
    std::cin >> newNumOfRowSeats;
    
    // Call the controller to update the aircraft
    try {
        if (AdminController::updateAircraft(
            currentUser -> getUserId(),
            aircraftId,
            newModel.empty() ? existingAircraft -> getModel() : newModel,
            newCapacity == 0 ? existingAircraft -> getCapacity() : newCapacity,
            newNumOfRowSeats == 0 ? existingAircraft -> getNumOfRowSeats() : newNumOfRowSeats
        )) {
            std::cout << "Aircraft updated successfully!" << std::endl;
        } else {
            std::cout << "Failed to update aircraft. Please check the details and try again." << std::endl;
        }
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
void AdminInterface::removeExistingAircraft() {
    std::cout << " ----- Remove Existing Aircraft ----- " << std::endl;
    if(!displayExistingAircrafts()) {
        return;
    }
    std::string aircraftId;
    std::cout << "Please enter the Aircraft ID to remove: ";
    std::cin >> aircraftId;
        
    // Call the controller to remove the aircraft
    if (AdminController::removeAircraft(currentUser -> getUserId(), aircraftId)) {
        std::cout << "Aircraft removed successfully!" << std::endl;
    } else {
        std::cout << "Failed to remove aircraft. Please check the details and try again." << std::endl;
    }
}

/*********************************************** Manage Users ***********************************************/

void AdminInterface::displayManageUsersMenu() {
    std::cout << "Manage Users Menu:" << std::endl;
    std::cout << "1. Add User" << std::endl;
    std::cout << "2. Update User Password" << std::endl;
    std::cout << "3. Remove User" << std::endl;
    std::cout << "4. View Users" << std::endl;
    std::cout << "5. Back to Admin Menu" << std::endl;
}

void AdminInterface::handleUsers() {
    int choice = 0;
    while (choice != BACK_OPTION) {
        displayManageUsersMenu();
        std::cout << "Choice: ";
        std::cin >> choice;
        switch (choice) {
            case ADD_USER_OPTION:
                addUser();
                break;
            case UPDATE_USER_OPTION:
                updateExistingUser();
                break;
            case REMOVE_USER_OPTION:
                removeExistingUser();
                break;
            case VIEW_USERS_OPTION:
                displayExistingUsers();
                break;
            case BACK_OPTION:
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

void AdminInterface::addUser() {
    std::string username;
    std::string password;
    int roleChoice;

    std::cout << " ----- Add New User ----- " << std::endl;
    std::cout << "Enter Username: ";
    std::cin >> username;
    std::cout << "Enter Password: ";
    std::cin >> password;
    std::cout << "Select User Role:" << std::endl;
    std::cout << "1. Passenger" << std::endl;
    std::cout << "2. Admin" << std::endl;
    std::cout << "3. Booking Manager" << std::endl;
    std::cout << "Choice: ";
    std::cin >> roleChoice;

    UserModel::UserType role;
    switch (roleChoice) {
        case 1:
            role = UserModel::UserType::Passenger;
            break;
        case 2:
            role = UserModel::UserType::Admin;
            break;
        case 3:
            role = UserModel::UserType::BookingManager;
            break;
        default:
            std::cout << "Invalid role. User will be added as Passenger by default." << std::endl;
            role = UserModel::UserType::Passenger;
            break;
    }

    // Call the controller to add the user
    auto newUser = AdminController::createUser(currentUser -> getUserId(), username, password, role);
    if (newUser.has_value()) {
        std::cout << "User added successfully! User ID: " << newUser.value() -> getUserId() << std::endl;
    } else {
        std::cout << "Failed to add user. Please check the details and try again." << std::endl;
    }
}

bool AdminInterface::displayExistingUsers() {
    auto users = AdminController::getAllUsers(currentUser -> getUserId());
    if (users.size() <= 1) { // Only the current admin exists
        std::cout << "No other users available." << std::endl;
        return false;
    }
    std::cout << "Here are the existing users:" << std::endl;
    int index = 1;
    for (const auto& user : users) {
        if (user -> getUserId() == currentUser -> getUserId()) {
            continue; // Skip displaying the current admin user
        }
        std::cout << index << ". User ID: " << user -> getUserId() << std::endl;
        std::cout << "   Username: " << user -> getUsername() << std::endl;
        std::cout << "   Role: ";
        switch (user -> getRole()) {
            case UserModel::UserType::Passenger:
                std::cout << "Passenger" << std::endl;
                break;
            case UserModel::UserType::Admin:
                std::cout << "Admin" << std::endl;
                break;
            case UserModel::UserType::BookingManager:
                std::cout << "Booking Manager" << std::endl;
                break;
            default:
                std::cout << "Unknown" << std::endl;
                break;
        }
        index++;
    }
    return true;
}

void AdminInterface::updateExistingUser() {
    std::cout << " ----- Update Existing User ----- " << std::endl;
    if(!displayExistingUsers()) {
        return;
    }
    std::string userId;
    std::cout << "Please enter the User ID to update: ";
    std::cin >> userId;
    
    // Fetch the existing user details
    auto existingUserOpt = AdminController::getUserById(currentUser -> getUserId(), userId);
    if (!existingUserOpt.has_value()) {
        std::cout << "User not found." << std::endl;
        return;
    }
    auto existingUser = existingUserOpt.value();
    
    // Display current details
    std::cout << "Current User Details:" << std::endl;
    std::cout << "User ID: " << existingUser -> getUserId() << std::endl;
    std::cout << "Username: " << existingUser -> getUsername() << std::endl;
    std::cout << "Role: ";
    switch (existingUser -> getRole()) {
        case UserModel::UserType::Passenger:
            std::cout << "Passenger" << std::endl;
            break;
        case UserModel::UserType::Admin:
            std::cout << "Admin" << std::endl;
            break;
        case UserModel::UserType::BookingManager:
            std::cout << "Booking Manager" << std::endl;
            break;
        default:
            std::cout << "Unknown" << std::endl;
            break;
    }

    // Get new details
    std::string newPassword;
    std::cout << "Enter new Password (leave empty to keep current): ";
    std::getline(std::cin, newPassword);
    if (newPassword.empty()) {
        std::cout << "No changes made to the password." << std::endl;
        return;
    }

    // Call the controller to update the user
    if (AdminController::updateUserPassword(currentUser -> getUserId(), userId, newPassword)) {
        std::cout << "User password updated successfully!" << std::endl;
    } else {
        std::cout << "Failed to update user password. Please check the details and try again." << std::endl;
    }
}

void AdminInterface::removeExistingUser() {
    std::cout << " ----- Remove Existing User ----- " << std::endl;
    if(!displayExistingUsers()) {
        return;
    }
    std::string userId;
    std::cout << "Please enter the User ID to remove: ";
    std::cin >> userId;
    
    // Call the controller to remove the user
    if (AdminController::deleteUser(currentUser -> getUserId(), userId)) {
        std::cout << "User removed successfully!" << std::endl;
    } else {
        std::cout << "Failed to remove user. Please check the details and try again." << std::endl;
    }
}