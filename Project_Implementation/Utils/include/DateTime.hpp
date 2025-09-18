#pragma once

#include <string>
/**
 * @brief Represents a date and time.
 *
 * The DateTime struct encapsulates year, month, day, hour, minute, and second components.
 * It provides constructors for initialization, conversion from string, and formatting to string.
 * Utility methods include obtaining the current date and time, comparison, and validity checking.
 */
struct DateTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;

    DateTime() = default;
    DateTime& operator=(const DateTime&) = default;
    DateTime(int y, int m, int d, int h = 0, int min = 0);
    DateTime(const std::string& dateTimeStr);
    std::string toString() const;

    static DateTime now();
    bool operator<=(const DateTime& other) const;
    bool sameDay(const DateTime& other) const;
    bool isValid() const;

    private:
        void parseDateOnly(const std::string& dateStr);
        void parseTimeOnly(const std::string& timeStr);
};