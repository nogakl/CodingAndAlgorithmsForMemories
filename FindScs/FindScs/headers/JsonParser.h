#pragma once

#include "json11.hpp"
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace json11;

std::string CreateJsonFlilter(uint8_t filterCounter, std::string filters);

std::string CreateJsonHeader(std::string filtersStr);
std::string CreateJsonRecord(std::string x, std::string y, std::string z, std::string scs);
std::string CreateComplexJsonRecord(std::string filtersStr, std::string calculated, int diff);
void ParseJsonHeader(std::string jsonHeader, int& filters);
void ParseJsonRecord(std::string jsonRecord, std::string& x, std::string& y, std::string& z);
void ParseLogFile(std::string logFilePath);


std::string CreateResultJsonHeader(std::vector<int> filtersResult);

std::string CreateResultJsonRecord(std::string sequences);
