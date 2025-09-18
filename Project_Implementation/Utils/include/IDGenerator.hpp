#pragma once

#include <string>

/**
 * @class IDGenerator
 * @brief Utility class for generating unique string-based IDs.
 *
 * This class provides a static method to generate unique identifiers.
 * Instantiation of this class is disabled.
 *
 * @note All methods are static; do not instantiate this class.
 */
class IDGenerator {
    public:
        IDGenerator() = delete;
        static std::string generateUniqueID();
};