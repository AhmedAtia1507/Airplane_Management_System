# =============================================================================
# PROJECT CONFIGURATION
# =============================================================================

# Minimum CMake version requirement
cmake_minimum_required(VERSION 3.10)

# Project declaration
project(AirlineManagementSystem
    VERSION 1.0.0
    DESCRIPTION "A comprehensive airline management system"
    LANGUAGES CXX
)

# =============================================================================
# COMPILER SETTINGS
# =============================================================================

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =============================================================================
# BUILD CONFIGURATION OPTIONS
# =============================================================================

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message(STATUS "No build type selected. Defaulting to Debug.")
endif()

# Build options
option(ENABLE_ASAN "Enable AddressSanitizer for Debug builds" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer for Debug builds" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer for Debug builds" OFF)
option(ENABLE_MSAN "Enable MemorySanitizer for Debug builds" OFF)
option(VALGRIND_BUILD "Build for Valgrind analysis (disables sanitizers)" OFF)

# =============================================================================
# SOURCE FILES ORGANIZATION
# =============================================================================

# Model layer sources
set(MODEL_SOURCES
    Model/src/Admin.cpp
    Model/src/AircraftModel.cpp
    Model/src/BookingManager.cpp
    Model/src/CashPayment.cpp
    Model/src/CreditPayment.cpp
    Model/src/CrewMemberModel.cpp
    Model/src/FlightModel.cpp
    Model/src/FlightModelBuilder.cpp
    Model/src/Passenger.cpp
    Model/src/PaymentModel.cpp
    Model/src/PaymentStrategyFactory.cpp
    Model/src/PaypalPayment.cpp
    Model/src/ReservationModel.cpp
    Model/src/ReservationModelBuilder.cpp
    Model/src/UserFactory.cpp
    Model/src/UserModel.cpp
)

# Repository layer sources
set(REPOSITORY_SOURCES
    Repositories/src/AircraftRepository.cpp
    Repositories/src/CrewMemberRepository.cpp
    Repositories/src/FlightRepository.cpp
    Repositories/src/PaymentRepository.cpp
    Repositories/src/ReservationRepository.cpp
    Repositories/src/UserRepository.cpp
)

# Service layer sources
set(SERVICE_SOURCES
    Services/src/AircraftService.cpp
    Services/src/CrewMemberService.cpp
    Services/src/FlightService.cpp
    Services/src/PaymentService.cpp
    Services/src/ReservationService.cpp
    Services/src/UserManagementService.cpp
)

# Utility sources
set(UTILS_SOURCES
    Utils/src/DateTime.cpp
    Utils/src/IDGenerator.cpp
    Utils/src/JSONManager.cpp
    Utils/src/DatabasePathResolver.cpp
)

# Controller layer sources
set(CONTROLLER_SOURCES
    Controller/src/AdminController.cpp
    Controller/src/AuthController.cpp
    Controller/src/BookingManagerController.cpp
    Controller/src/PassengerController.cpp
)

# CLI interface sources
set(CLI_SOURCES
    CLI/src/AdminInterface.cpp
    CLI/src/BookingManagerInterface.cpp
    CLI/src/PassengerInterface.cpp
    CLI/src/UserInterface.cpp
)

# All source files
set(SOURCE_FILES
    main.cpp
    ${MODEL_SOURCES}
    ${REPOSITORY_SOURCES}
    ${SERVICE_SOURCES}
    ${UTILS_SOURCES}
    ${CONTROLLER_SOURCES}
    ${CLI_SOURCES}
)
# =============================================================================
# EXECUTABLE TARGET
# =============================================================================

add_executable(AirlineManagementSystem ${SOURCE_FILES})

target_compile_definitions(AirlineManagementSystem PRIVATE
    DATABASE_PATH="${CMAKE_SOURCE_DIR}/Database"
)
# Base compiler warnings (applied to all build types)
target_compile_options(AirlineManagementSystem PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wpedantic
    #-Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wconversion
    #-Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
)

# Build-type specific flags using generator expressions
target_compile_options(AirlineManagementSystem PRIVATE
    $<$<CONFIG:Debug>:-g -O0 -fno-omit-frame-pointer>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
    $<$<CONFIG:RelWithDebInfo>:-O2 -g -DNDEBUG>
    $<$<CONFIG:MinSizeRel>:-Os -DNDEBUG>
)

# Sanitizer flags (only for Debug builds)
target_compile_options(AirlineManagementSystem PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<BOOL:${ENABLE_ASAN}>>:-fsanitize=address>
    $<$<AND:$<CONFIG:Debug>,$<BOOL:${ENABLE_TSAN}>>:-fsanitize=thread>
    $<$<AND:$<CONFIG:Debug>,$<BOOL:${ENABLE_UBSAN}>>:-fsanitize=undefined>
    $<$<AND:$<CONFIG:Debug>,$<BOOL:${ENABLE_MSAN}>>:-fsanitize=memory>
)

# Valgrind-specific flags
target_compile_options(AirlineManagementSystem PRIVATE
    $<$<BOOL:${VALGRIND_BUILD}>:-fno-optimize-sibling-calls>
)

# Additional sanitizer link flags
target_link_options(AirlineManagementSystem PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<BOOL:${ENABLE_ASAN}>>:-fsanitize=address>
    $<$<AND:$<CONFIG:Debug>,$<BOOL:${ENABLE_TSAN}>>:-fsanitize=thread>
    $<$<AND:$<CONFIG:Debug>,$<BOOL:${ENABLE_UBSAN}>>:-fsanitize=undefined>
    $<$<AND:$<CONFIG:Debug>,$<BOOL:${ENABLE_MSAN}>>:-fsanitize=memory>
)

target_include_directories(AirlineManagementSystem
    PUBLIC
        Third_Party
    PRIVATE
        Controller/include
        Model/include
        Repositories/include
        Utils/include
        Services/include
        CLI/include
)

# =============================================================================
# CUSTOM TARGETS FOR ANALYSIS TOOLS
# =============================================================================

# Valgrind targets
add_custom_target(valgrind
    COMMAND ${CMAKE_COMMAND} -E echo "Starting Valgrind analysis..."
    COMMAND valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all 
            --track-origins=yes --verbose $<TARGET_FILE:AirlineManagementSystem>
    COMMAND ${CMAKE_COMMAND} -E echo "Valgrind analysis completed."
    DEPENDS AirlineManagementSystem
    COMMENT "Running Valgrind memory check"
    VERBATIM
)

add_custom_target(valgrind-detailed
    COMMAND ${CMAKE_COMMAND} -E echo "Starting detailed Valgrind analysis..."
    COMMAND valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all 
            --track-origins=yes --verbose --num-callers=20 
            $<TARGET_FILE:AirlineManagementSystem>
    COMMAND ${CMAKE_COMMAND} -E echo "Detailed Valgrind analysis completed."
    DEPENDS AirlineManagementSystem
    COMMENT "Running detailed Valgrind memory check"
    VERBATIM
)

# =============================================================================
# USAGE EXAMPLES AND HELP
# =============================================================================

# Print configuration summary
message(STATUS "=== Build Configuration Summary ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "AddressSanitizer: ${ENABLE_ASAN}")
message(STATUS "ThreadSanitizer: ${ENABLE_TSAN}")
message(STATUS "UBSanitizer: ${ENABLE_UBSAN}")
message(STATUS "MemorySanitizer: ${ENABLE_MSAN}")
message(STATUS "Valgrind build: ${VALGRIND_BUILD}")
message(STATUS "===================================")

# Example build commands
message(STATUS "To build the project, use the following commands:")
message(STATUS "Debug build:")
message(STATUS "  cmake -DCMAKE_BUILD_TYPE=Debug -B build")
message(STATUS "  cmake --build build --config Debug")
message(STATUS "Release build:")
message(STATUS "  cmake -DCMAKE_BUILD_TYPE=Release -B build")
message(STATUS "  cmake --build build --config Release")
message(STATUS "RelWithDebInfo build:")
message(STATUS "  cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -B build")
message(STATUS "  cmake --build build --config RelWithDebInfo")
message(STATUS "MinSizeRel build:")
message(STATUS "  cmake -DCMAKE_BUILD_TYPE=MinSizeRel -B build")
message(STATUS "  cmake --build build --config MinSizeRel")
message(STATUS "To run Valgrind analysis (after building):")
message(STATUS "  cmake --build build --config Debug --target valgrind")
message(STATUS "To run detailed Valgrind analysis (after building):")
message(STATUS "  cmake --build build --config Debug --target valgrind-detailed")
message(STATUS "===================================")