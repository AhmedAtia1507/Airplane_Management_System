#include "../include/JSONManager.hpp"


/**
 * @brief Parses a JSON file and populates a map of UserModel objects.
 *
 * This template specialization reads a JSON file from the specified file path,
 * validates that each JSON object contains an "id" field, and uses the UserFactory
 * to create UserModel instances. The created UserModel objects are stored in the
 * provided unordered_map, keyed by their "id".
 *
 * @param members Reference to an unordered_map where the parsed UserModel objects will be stored.
 * @param filePath Path to the JSON file to be parsed.
 *
 * @throws std::runtime_error If the JSON file cannot be opened for reading.
 * @throws std::invalid_argument If any JSON object does not contain an "id" field.
 */
template<>
void JSONManager::parseJSON<UserModel>(std::unordered_map<std::string, std::shared_ptr<UserModel>>& members, const std::string& filePath) {
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
    
        std::shared_ptr<UserModel> newElement = UserFactory::createUser(element);
        std::string id = element["id"].get<std::string>();
        members.insert({id, newElement});
    }
}