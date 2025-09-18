#include "../include/DateTime.hpp"
#include <stdexcept>
#include <chrono>
#include <ctime>

/**
 * @brief Constructs a DateTime object with specified date and time components.
 * 
 * @param y    Year component.
 * @param m    Month component (1-12).
 * @param d    Day component (1-31).
 * @param h    Hour component (0-23).
 * @param min  Minute component (0-59).
 */
DateTime::DateTime(int y, int m, int d, int h, int min) 
    : year(y), month(m), day(d), hour(h), minute(min) {}

/**
 * @brief Constructs a DateTime object from a string representation.
 *
 * Accepts input strings in either "YYYY-MM-DD" or "YYYY-MM-DD HH:MM" format.
 * If only the date is provided ("YYYY-MM-DD"), the time is set to 00:00.
 * Parses the year, month, day, hour, and minute from the string.
 *
 * @param dateTimeStr The date and time string in the format "YYYY-MM-DD" or "YYYY-MM-DD HH:MM".
 * @throws std::invalid_argument if the input string does not match the expected format.
 *         Specifically, throws "Invalid date time string format" if the string does not match either format.
 */
DateTime::DateTime(const std::string& dateTimeStr) {
    // Remove leading/trailing whitespace
    std::string trimmed = dateTimeStr;
    trimmed.erase(0, trimmed.find_first_not_of(" \t"));
    trimmed.erase(trimmed.find_last_not_of(" \t") + 1);
    
    // Find the space separator
    size_t spacePos = trimmed.find(' ');
    
    if (spacePos == std::string::npos) {
        // Only date provided: "YYYY-MM-DD" or "YYYY-M-D"
        parseDateOnly(trimmed);
        hour = 0;
        minute = 0;
    } else {
        // Date and time provided: "YYYY-MM-DD HH:MM" or "YYYY-M-D H:MM"
        std::string datePart = trimmed.substr(0, spacePos);
        std::string timePart = trimmed.substr(spacePos + 1);
        
        parseDateOnly(datePart);
        parseTimeOnly(timePart);
    }
    
    if (!isValid()) {
        throw std::invalid_argument("Invalid date time value: " + dateTimeStr);
    }
}

/**
 * @brief Parses a date string and extracts year, month, and day components.
 * 
 * This function parses a date string in the format YYYY-MM-DD or YYYY-M-D
 * and sets the corresponding year, month, and day member variables.
 * 
 * @param dateStr The date string to parse. Must be in format YYYY-MM-DD or YYYY-M-D
 *                where YYYY is a 4-digit year, MM/M is month (1-12), and DD/D is day (1-31)
 * 
 * @throws std::invalid_argument If the date format is invalid (missing dashes) or
 *                               if any numeric component cannot be parsed as an integer
 * 
 * @note This function only parses the date portion and does not handle time components.
 *       The parsed values are stored in the class member variables year, month, and day.
 * 
 * @example
 * parseDateOnly("2023-12-25");  // Valid: December 25, 2023
 * parseDateOnly("2023-1-5");    // Valid: January 5, 2023
 */
void DateTime::parseDateOnly(const std::string& dateStr) {
    // Split by '-'
    size_t firstDash = dateStr.find('-');
    size_t secondDash = dateStr.find('-', firstDash + 1);
    
    if (firstDash == std::string::npos || secondDash == std::string::npos) {
        throw std::invalid_argument("Invalid date format. Expected YYYY-MM-DD or YYYY-M-D");
    }
    
    try {
        year = std::stoi(dateStr.substr(0, firstDash));
        month = std::stoi(dateStr.substr(firstDash + 1, secondDash - firstDash - 1));
        day = std::stoi(dateStr.substr(secondDash + 1));
    } catch (const std::exception& e) {
        throw std::invalid_argument("Invalid numeric value in date: " + dateStr);
    }
}
/**
 * @brief Parses a time string and extracts hour and minute components.
 * 
 * This function parses a time string in the format "HH:MM" or "H:MM" and sets
 * the hour and minute member variables of the DateTime object.
 * 
 * @param timeStr The time string to parse, expected in format "HH:MM" or "H:MM"
 * 
 * @throws std::invalid_argument If the time format is invalid (missing colon separator)
 * @throws std::invalid_argument If the hour or minute values are not valid numeric values
 * 
 * @note This function only parses time components (hour and minute) and does not
 *       affect date components of the DateTime object.
 * 
 * @example
 * DateTime dt;
 * dt.parseTimeOnly("14:30");  // Sets hour to 14, minute to 30
 * dt.parseTimeOnly("9:05");   // Sets hour to 9, minute to 5
 */
void DateTime::parseTimeOnly(const std::string& timeStr) {
    // Split by ':'
    size_t colonPos = timeStr.find(':');
    
    if (colonPos == std::string::npos) {
        throw std::invalid_argument("Invalid time format. Expected HH:MM or H:MM");
    }
    
    try {
        hour = std::stoi(timeStr.substr(0, colonPos));
        minute = std::stoi(timeStr.substr(colonPos + 1));
    } catch (const std::exception& e) {
        throw std::invalid_argument("Invalid numeric value in time: " + timeStr);
    }
}
/**
 * @brief Converts the DateTime object to a formatted string.
 *
 * The returned string is in the format "YYYY-MM-DD HH:MM",
 * with leading zeros for single-digit month, day, hour, and minute values.
 *
 * @return A string representing the date and time.
 */
std::string DateTime::toString() const {
    return std::to_string(year) + "-" +
           (month < 10 ? "0" : "") + std::to_string(month) + "-" +
           (day < 10 ? "0" : "") + std::to_string(day) + " " +
           (hour < 10 ? "0" : "") + std::to_string(hour) + ":" +
           (minute < 10 ? "0" : "") + std::to_string(minute);
}

/**
 * @brief Compares two DateTime objects to determine if this object is less than or equal to another.
 *
 * Performs a sequential comparison of year, month, day, hour, and minute fields.
 * Returns true if this DateTime is earlier than or equal to the specified DateTime.
 *
 * @param other The DateTime object to compare against.
 * @return true if this DateTime is less than or equal to other, false otherwise.
 */
bool DateTime::operator<=(const DateTime& other) const {
    if (year != other.year)     return year < other.year;
    if (month != other.month)   return month < other.month;
    if (day != other.day)       return day < other.day;
    if (hour != other.hour)     return hour < other.hour;
    return minute < other.minute;
}
/**
 * @brief Checks if this DateTime object represents the same calendar day as another.
 * 
 * Compares the year, month, and day fields of both DateTime objects.
 *
 * @param other The DateTime object to compare with.
 * @return true if both DateTime objects have the same year, month, and day; false otherwise.
 */
bool DateTime::sameDay(const DateTime& other) const {
    return year == other.year && month == other.month && day == other.day;
}

/**
 * @brief Creates a DateTime object representing the current local date and time.
 * 
 * This static method retrieves the current system time and converts it to the local
 * timezone, creating a DateTime object with the current year, month, day, hour, and minute.
 * The implementation uses platform-specific functions for thread-safe time conversion.
 * 
 * @return DateTime object representing the current local date and time (seconds are not included)
 * 
 * @note This method uses localtime_s() on Windows and localtime_r() on other platforms
 *       for thread-safe local time conversion.
 * @note The returned DateTime object does not include seconds precision.
 */
DateTime DateTime::now() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &t);
#else
    localtime_r(&t, &local_tm);
#endif
    return DateTime(
        local_tm.tm_year + 1900,
        local_tm.tm_mon + 1,
        local_tm.tm_mday,
        local_tm.tm_hour,
        local_tm.tm_min
    );
}


/**
 * @brief Validates the DateTime object's date and time components.
 * 
 * Checks if the current DateTime object represents a valid date and time by verifying:
 * - Year is non-negative
 * - Month is between 1-12
 * - Day is between 1-31 and valid for the specific month
 * - Hour is between 0-23
 * - Minute is between 0-59
 * - Accounts for leap years when validating February dates
 * - Handles months with 30 days (April, June, September, November)
 * 
 * @return true if the DateTime object represents a valid date and time, false otherwise
 */
bool DateTime::isValid() const {
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31 ||
        hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        return false;
    }
    // Check for valid days in month
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if ((isLeap && day > 29) || (!isLeap && day > 28)) return false;
    }
    return true;
}