#include "JsonParser.h"

std::string CreateJsonRecord(std::string x, std::string y, std::string z, int8_t filters) {
    Json my_json = Json::object{
    { "filters", filters },
    { "sequences", Json::array { x, y, z } },
    };
    return my_json.dump();  
}

void ParseJsonRecord(std::string jsonRecord, std::string& x, std::string& y, std::string& z, int8_t& filters) {
    std::string err;
    Json jsonObj = Json::parse(jsonRecord, err);
    filters = jsonObj["filters"].int_value();
    x = jsonObj["sequences"].array_items()[0].string_value();
    y = jsonObj["sequences"].array_items()[1].string_value();
    z = jsonObj["sequences"].array_items()[2].string_value();

}