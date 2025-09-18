#include "../include/IDGenerator.hpp"
#include <random>


/**
 * @brief Generates a unique ID as a string.
 *
 * This function creates a random integer between 10000 and 99999
 * using a random number generator and returns it as a string.
 * Each call is likely to produce a different value, suitable for
 * simple unique identification purposes.
 *
 * @return A randomly generated unique ID as a std::string.
 */
std::string IDGenerator::generateUniqueID() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(10000, 99999);

    return std::to_string(distribution(generator));
}