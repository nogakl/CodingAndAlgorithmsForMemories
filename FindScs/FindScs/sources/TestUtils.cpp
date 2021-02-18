#include "TestUtils.h"

std::vector<int> getOnLocations(int a) {
    std::vector<int> result;
    int place = 0;
    while (a != 0) {
        if (a & 1) {
            result.push_back(place);
        }
        ++place;
        a >>= 1;
    }
    return result;
}

std::vector<std::vector<int>> powerSet(const std::vector<int>& set) {
    std::vector<std::vector<int>> result;
    int numPowerSets = static_cast<int>(pow(2.0, static_cast<double>(set.size())));
    for (size_t i = 0; i < numPowerSets; ++i) {
        std::vector<int> onLocations = getOnLocations(i);
        std::vector<int> subSet;
        for (size_t j = 0; j < onLocations.size(); ++j) {
            subSet.push_back(set.at(onLocations.at(j)));
        }
        result.push_back(subSet);
    }
    return result;
}

void CombinationRepetitionUtil(char chosen[], char arr[], 
    uint16_t index, uint16_t r, uint16_t end, std::list<std::string>* all_words)
{
    // Since index has become r, current combination is 
    // ready to be printed, print 
    std::string str;
    if (index == r)
    {
        for (uint16_t i = 0; i < r; i++)
            str.push_back(arr[chosen[i]]);
        reverse(str.begin(), str.end());
        all_words->push_front(str);
        return;
    }

    // One by one choose all elements (without considering 
    // the fact whether element is already chosen or not) 
    // and recur 
    for (uint16_t i = 0; i <= end; i++)
    {
        chosen[index] = i;
        CombinationRepetitionUtil(chosen, arr, index + 1, r, end, all_words);
    }
    return;
}

// The main function that prints all combinations of size r 
// in arr[] of size n with repitions. This function mainly 
// uses CombinationRepetitionUtil() 
void CombinationRepetition(char arr[], uint16_t n, uint16_t w_size, std::list<std::string>* all_words)
{
    // Allocate memory 
    char* chosen{ new char[w_size + 1] {} };

    // Call the recursice tfunction 
    CombinationRepetitionUtil(chosen, arr, 0, w_size, n - 1, all_words);

    delete chosen;
}
