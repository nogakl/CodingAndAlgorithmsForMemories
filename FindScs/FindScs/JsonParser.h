#pragma once

#include "headers/json11.hpp"
using namespace json11;

std::string CreateJsonRecord(std::string x, std::string y, std::string z, int8_t filters);
void ParseJsonRecord(std::string jsonRecord, std::string& x, std::string& y, std::string& z, int8_t& filters);
