#include "JsonParser.h"

std::string CreateJsonHeader(uint8_t filters) {
    Json my_json = Json::object{
   { "filters", filters } };
    return my_json.dump();
}

std::string CreateJsonRecord(std::string x, std::string y, std::string z) {
    Json my_json = Json::object{
    { "sequences", Json::array { x, y, z } },
    };
    return my_json.dump();  
}

void ParseJsonHeader(std::string jsonHeader, int& filters) {
    std::string err;
    Json jsonObj = Json::parse(jsonHeader, err);
    filters = jsonObj["filters"].int_value();
}

void ParseJsonRecord(std::string jsonRecord, std::string& x, std::string& y, std::string& z) {
    std::string err;
    Json jsonObj = Json::parse(jsonRecord, err);
    x = jsonObj["sequences"].array_items()[0].string_value();
    y = jsonObj["sequences"].array_items()[1].string_value();
    z = jsonObj["sequences"].array_items()[2].string_value();
}

void ParseLogFile(std::string logFilePath) {
    std::string x = "";
    std::string y = "";
    std::string z = "";
    auto filters = 0;

    std::string header;
    std::string record;
    std::ifstream myfile;
    myfile.open(logFilePath);
    
    getline(myfile, header);
    ParseJsonHeader(header, filters);

    while (getline(myfile, record)) {
        ParseJsonRecord(record, x, y, z);
        //TODO:: print/garph the result..
    }
}
