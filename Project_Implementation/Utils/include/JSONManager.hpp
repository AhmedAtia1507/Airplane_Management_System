#pragma once

#include <unordered_map>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <string>
#include "../../Model/include/UserFactory.hpp"
#include "../../Third_Party/json.hpp"
#include "../../Model/include/UserModel.hpp"

using JSON = nlohmann::json;


/**
 * @class JSONManager
 * @brief Utility class for parsing and saving objects to and from JSON files.
 *
 * Provides static template methods to parse JSON files into containers of objects,
 * and to serialize containers of objects back to JSON files. Requires that the object
 * type T is constructible from a const JSON& and provides a to_json method.
 *
 * @tparam T Type of object to be managed. Must be constructible from const JSON& and have a to_json(JSON&) method.
 *
 * @note Uses nlohmann::json for JSON parsing and serialization.
 */
class JSONManager {
    public:
        JSONManager() = delete; // Prevent instantiation of this utility class
        template<typename T>
        static void parseJSON(std::unordered_map<std::string, std::shared_ptr<T>>& members, const std::string& filePath) {
            static_assert(std::is_constructible<T, const JSON&>::value, "T must be constructible from const JSON&");
            std::ifstream inputJSON(filePath);
            if (!inputJSON.is_open()) {
                throw std::runtime_error("JSON File \"" + filePath + "\" could not be opened for reading.");
            }
        
            JSON json;
            inputJSON >> json;
        
            for(auto& element : json) {
                if(element.is_null()|| !element.contains("id")) {
                    throw std::invalid_argument("Error: Invalid JSON Format. JSON Object doesn't contain id");
                }
            
                std::shared_ptr<T> newElement = std::make_shared<T>(element);
                std::string id = element["id"].get<std::string>();
                members.insert({id, newElement});
            }
        }
        template<typename T>
        static void saveToJSON(const std::unordered_map<std::string, std::shared_ptr<T>>& members, const std::string& filePath) {
            std::ofstream outputJSON(filePath);
            if(!outputJSON.is_open()) {
                throw std::runtime_error("JSON File \"" + filePath + "\" could not be opened for writing.");
            }
            JSON json = nlohmann::json::array();
            for(const auto& member : members) {
                JSON j;
                member.second->to_json(j);
                json.push_back(j);
            }
            outputJSON << std::setw(4) << json << std::endl;
        }
};

/**
 * @brief Parses a JSON file and populates a map of UserModel objects.
 *
 * Specialization of JSONManager::parseJSON for UserModel. Reads the JSON data from the specified file path,
 * deserializes each user entry, and inserts them into the provided unordered_map, using the user's identifier
 * as the key and a shared pointer to the UserModel as the value.
 *
 * @param members Reference to an unordered_map where the parsed UserModel objects will be stored.
 * @param filePath Path to the JSON file containing user data.
 *
 * @throws std::runtime_error If the file cannot be opened or the JSON is invalid.
 */
template<>
void JSONManager::parseJSON<UserModel>(std::unordered_map<std::string, std::shared_ptr<UserModel>>& members, const std::string& filePath);