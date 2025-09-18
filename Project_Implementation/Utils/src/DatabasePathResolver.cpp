#include "../include/DatabasePathResolver.hpp"
#include <filesystem>
#include <vector>
#include <stdexcept>

/**
 * @brief Searches for the database directory in predefined locations and returns its canonical path.
 * 
 * This method attempts to locate the database directory by checking multiple potential paths
 * in a specific order, starting from the current directory and moving up the directory hierarchy.
 * The search includes both relative paths and project-specific paths.
 * 
 * Search order:
 * 1. ./Database/ (current directory)
 * 2. ../Database/ (parent directory)
 * 3. ../../Database/ (grandparent directory)
 * 4. ./Project_Implementation/Database/ (project subdirectory)
 * 5. ../Project_Implementation/Database/ (project in parent directory)
 * 
 * @return std::string The canonical absolute path to the database directory with trailing slash
 * @throws std::runtime_error If none of the predefined paths exist or are accessible
 * 
 * @note The returned path is guaranteed to be canonical (absolute, resolved symlinks, normalized)
 * @note The returned path always ends with a directory separator "/"
 */
std::string DatabasePathResolver::findDatabasePath() {
    std::vector<std::string> searchPaths = {
        "./Database/",                    // Current directory
        "../Database/",                   // Parent directory
        "../../Database/",                // Grandparent directory
        "./Project_Implementation/Database/",
        "../Project_Implementation/Database/"
    };
    
    for (const auto& path : searchPaths) {
        if (std::filesystem::exists(path)) {
            return std::filesystem::canonical(path).string() + "/";
        }
    }
    
    throw std::runtime_error("Database directory not found!");
}

/**
 * @brief Retrieves the database path for the application.
 * 
 * This function returns the path to the database directory. If the DATABASE_PATH
 * macro is defined at compile time, it uses that predefined path with a trailing
 * slash appended. Otherwise, it dynamically finds the database path using the
 * findDatabasePath() method.
 * 
 * @return std::string The database path with a trailing slash, either from
 *         the compile-time DATABASE_PATH macro or dynamically resolved.
 * 
 * @note The returned path always includes a trailing slash for consistency
 *       in path concatenation operations.
 */
std::string DatabasePathResolver::getDatabasePath() {
    #ifdef DATABASE_PATH
        return std::string(DATABASE_PATH) + "/";
    #else
        return findDatabasePath();
    #endif
}
