#pragma once
#include <string>

/**
 * @brief Utility class for resolving database file paths in the application.
 * 
 * This class provides static methods to locate and retrieve the database path
 * for the Airplane Management System. It automatically searches for the database
 * file in predefined locations and caches the result for efficient access.
 * 
 * The class is designed as a utility class with deleted constructor to prevent
 * instantiation, as all functionality is provided through static methods.
 * 
 * @note This class cannot be instantiated as the constructor is explicitly deleted.
 */
class DatabasePathResolver {
    static std::string findDatabasePath();    
    public:
        static std::string getDatabasePath();
        DatabasePathResolver() = delete;
};