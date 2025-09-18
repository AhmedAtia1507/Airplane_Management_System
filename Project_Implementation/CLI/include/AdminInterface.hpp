#pragma once

#include <memory>
#include "../../Model/include/Admin.hpp"
#include "../../Model/include/CrewMemberModel.hpp"

/**
 * @brief Interface class for administrator operations in the Airplane Management System
 * 
 * The AdminInterface class provides a command-line interface for administrators to manage
 * flights, aircraft, and users within the system. It handles user interactions through
 * menu-driven operations and maintains the current admin user session.
 * 
 * @details This class supports three main management areas:
 * - Flight Management: Create, update, remove flights and manage crew assignments
 * - Aircraft Management: Add, modify, and remove aircraft from the system
 * - User Management: Handle user accounts and their associated data
 * 
 * The interface uses a hierarchical menu system with predefined option constants
 * for consistent navigation and operation selection.
 */
class AdminInterface {
    std::shared_ptr<Admin> currentUser;
    
    void displayAdminMenu();

    // Flight Management
    void displayManageFlightsMenu();
    void handleFlights();
    void addFlightOption();
    bool displayAllAircrafts();
    void displayCrewMembers(const CrewMemberModel::CrewType& role = CrewMemberModel::CrewType::Pilot);
    bool displayExistingFlights();
    void updateExistingFlight();
    void removeExistingFlight();
    void assignCrewToFlight();
    void removeCrewMemberFromFlight();
    void displayCrewMembersOfFlight();

    // Aircraft Management
    void displayManageAircraftsMenu();
    void handleAircrafts();
    void addAircraft();
    bool displayExistingAircrafts();
    void updateExistingAircraft();
    void removeExistingAircraft();
    
    // User Management
    void displayManageUsersMenu();
    void handleUsers();
    void addUser();
    bool displayExistingUsers();
    void updateExistingUser();
    void removeExistingUser();

    constexpr static int MANAGE_FLIGHTS_OPTION = 1;
    constexpr static int MANAGE_AIRCRAFTS_OPTION = 2;
    constexpr static int MANAGE_USERS_OPTION = 3;
    constexpr static int LOGOUT_OPTION = 4;

    constexpr static int ADD_FLIGHT_OPTION = 1;
    constexpr static int UPDATE_FLIGHT_OPTION = 2;
    constexpr static int REMOVE_FLIGHT_OPTION = 3;
    constexpr static int VIEW_FLIGHTS_OPTION = 4;
    constexpr static int ASSIGN_CREW_OPTION = 5;
    constexpr static int REMOVE_CREW_OPTION = 6;
    constexpr static int FLIGHT_BACK_OPTION = 7;

    constexpr static int BACK_OPTION = 5;

    constexpr static int ADD_AIRCRAFT_OPTION = 1;
    constexpr static int UPDATE_AIRCRAFT_OPTION = 2;
    constexpr static int REMOVE_AIRCRAFT_OPTION = 3;
    constexpr static int VIEW_AIRCRAFTS_OPTION = 4;
    
    constexpr static int ADD_USER_OPTION = 1;
    constexpr static int UPDATE_USER_OPTION = 2;
    constexpr static int REMOVE_USER_OPTION = 3;
    constexpr static int VIEW_USERS_OPTION = 4;

    public:
        AdminInterface(const std::shared_ptr<Admin>& admin);
        void startInterface();
};