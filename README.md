# Airline Reservation and Management System

## Overview

This project is a modern C++ Airline Reservation and Management System designed for robust, scalable, and maintainable airline operations. It features a layered architecture, clean separation of concerns, and leverages modern C++ best practices including smart pointers, RAII, and comprehensive error handling.

The system supports user authentication, flight management, crew assignment, reservation processing, and payment handling. Data persistence is achieved using JSON files, and the project is fully cross-platform, with build and analysis support for both Windows and Linux (WSL).

---

## Features

- **User Roles:** Admin, Booking Manager, Passenger, Crew
- **Authentication:** Secure login and role-based access
- **Flight Management:** Create, update, search, and view flights
- **Crew Management:** Assign and remove crew members from flights
- **Reservation System:** Book, modify, and cancel reservations
- **Payment Processing:** Supports Credit Card, PayPal, and Cash payments
- **Data Persistence:** All data stored in JSON files for easy portability
- **Robust Error Handling:** Defensive programming and input validation
- **Memory Safety:** No leaks (Valgrind and AddressSanitizer clean)
- **Extensible Design:** Layered architecture for easy feature addition

---

## Project Structure

```
Project_Implementation/
├── CMakeLists.txt
├── main.cpp
├── CLI/                # Command-line interfaces for each user role
├── Controller/         # Business logic controllers
├── Model/              # Data models and business entities
├── Repositories/       # Data access and persistence
├── Services/           # Service layer for business operations
├── Utils/              # Utility functions and helpers
├── Third_Party/        # External libraries (e.g., nlohmann/json)
├── Database/           # JSON data files
└── build/              # Build artifacts
```

---

## Technologies Used

- **C++17** (Modern features: smart pointers, RAII, std::optional, etc.)
- **CMake** (Cross-platform build system)
- **nlohmann/json** (JSON parsing and serialization)
- **Valgrind & AddressSanitizer** (Memory analysis and debugging)
- **Windows & WSL/Linux** (Cross-platform compatibility)

---

## How to Build & Run

### Prerequisites

- C++17 compatible compiler (g++, clang++, MSVC)
- CMake >= 3.10
- [nlohmann/json](https://github.com/nlohmann/json) (included in Third_Party)

### Build (Linux/WSL)

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build
./build/AirlineManagementSystem
```

### Build (Windows)

```powershell
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build
.\build\AirlineManagementSystem.exe
```

### Run Memory Analysis

```bash
valgrind --tool=memcheck --leak-check=full ./build/AirlineManagementSystem
```

---

## Example Use Cases

- **Admin:** Manage flights, assign crew, view reports
- **Booking Manager:** Search flights, create reservations, process payments
- **Passenger:** View flights, book tickets, manage reservations
- **Crew:** View assigned flights and schedules

---

## Why This Project Stands Out

- **Professional architecture:** Layered, modular, and extensible
- **Modern C++:** Uses best practices and advanced features
- **Memory safe:** Verified with Valgrind and sanitizers
- **Real-world complexity:** Simulates actual airline operations
- **Portfolio-ready:** Demonstrates advanced C++ and software engineering skills

---

## Author

**Ahmed Atia**  
*Software Developer | Modern C++ Enthusiast*  
LinkedIn | GitHub

---

## License

This project is released under the MIT License.
