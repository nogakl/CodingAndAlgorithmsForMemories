#pragma once

#include "json11.hpp"
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace json11;

std::string CreateJsonHeader(uint8_t filters);
std::string CreateJsonRecord(std::string x, std::string y, std::string z);
void ParseJsonHeader(std::string jsonHeader, int& filters);
void ParseJsonRecord(std::string jsonRecord, std::string& x, std::string& y, std::string& z);
void ParseLogFile(std::string logFilePath);
