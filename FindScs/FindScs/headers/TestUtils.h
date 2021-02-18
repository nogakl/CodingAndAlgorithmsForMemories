#pragma once
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>

std::vector<std::vector<int> > powerSet(const std::vector<int>& set);
void CombinationRepetition(char arr[], uint16_t n, uint16_t w_size, std::list<std::string>* all_words);