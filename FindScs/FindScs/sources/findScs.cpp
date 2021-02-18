//#include <bits/stdc++.h>
#include <algorithm>  
#include <chrono>
#include "JsonParser.h"
#include "TestUtils.h"
#include <direct.h>
#include <random>
#include <thread>



//#define CREATE_FILTERS_MAP
//#define PLOT_ALL_WORDS
static constexpr int SIZE = 12;
static constexpr int POOL_SIZE = 2000000;


static std::string OutputsDir = "\\no_MASK_C_results_size_";
#define GetCurrentDir _getcwd

#define ALL_FILTERS 0xFD
#define NO_FILTERS 0x00
#define MASK_A 0x80
#define MASK_B 0x40
#define MASK_C 0x20
#define MASK_AB 0x10
#define MASK_AC 0x08
#define MASK_BC 0x04

auto currentFilters =  MASK_A | MASK_B | MASK_AC |MASK_AB | MASK_BC;
#define RETURN_ERR(msg)\
    /*printf(msg);*/\
    return "ERROR";

#define USE_FILTER(MASK, FILTER) (MASK & FILTER)


// returns shortest supersequence of X and Y and Z
static int dp[SIZE + 1][SIZE + 1][SIZE + 1];

std::string findScs(std::string X, std::string Y, std::string Z, int8_t filter_mask)
{
    std::fill_n(&dp[0][0][0], (SIZE +1) * (SIZE + 1) * (SIZE + 1), 0);
    for (int i = 0; i <= SIZE; ++i)
    {
        for (int j = 0; j <= SIZE; ++j)
        {
            for (int k = 0; k <= SIZE; ++k)
            {
                dp[i][j][k] = INT_MAX;
            }
        }
    }

    for (int i = 0; i <= SIZE; ++i)
    {
        for (int j = 0; j <= SIZE; ++j)
        {
            for (int k = 0; k <= SIZE; ++k)
            {
                if (!i && !j && !k)
                {
                    dp[0][0][0] = 0;
                    continue;
                }

                int res;
                int min = INT_MAX - 1;
                if (i != 0 && USE_FILTER(filter_mask, MASK_A))
                    min = min > (res = !k && !j ? i : 1 + dp[i - 1][j][k]) ? res : min;
                if (j != 0 && USE_FILTER(filter_mask, MASK_B))
                    min = min > (res = !i && !k ? j : 1 + dp[i][j - 1][k]) ? res : min;
                if (k != 0 && USE_FILTER(filter_mask, MASK_C))
                    min = min > (res = !i && !j ? k : 1 + dp[i][j][k - 1]) ? res : min;
                if (i != 0 && j != 0 && USE_FILTER(filter_mask, MASK_AB) && X[i - 1] == Y[j - 1])
                    min = min > (res = 1 + dp[i - 1][j - 1][k]) ? res : min;
                if (i != 0 && k != 0 && USE_FILTER(filter_mask, MASK_AC) && X[i - 1] == Z[k - 1])
                    min = min > (res = 1 + dp[i - 1][j][k - 1]) ? res : min;
                if (j != 0 && k != 0 && USE_FILTER(filter_mask, MASK_BC) && Y[j - 1] == Z[k - 1])
                    min = min > (res = 1 + dp[i][j - 1][k - 1]) ? res : min;
                if (i != 0 && j != 0 && k != 0 && X[i - 1] == Y[j - 1] && X[i - 1] == Z[k - 1])
                    min = min > (res = 1 + dp[i - 1][j - 1][k - 1]) ? res : min;

                dp[i][j][k] = min;
            }
        }
    }

    if (dp[SIZE][SIZE][SIZE] == INT_MAX - 1)
    {
        RETURN_ERR("need more filters");
    }

    int i = SIZE, j = SIZE, k = SIZE;
    int index = 0;
    std::string scs;
    while (index < dp[SIZE][SIZE][SIZE])
    {
        int result = dp[i][j][k];
        if (i && j && k && X[i - 1] == Y[j - 1] && Z[k - 1] == X[i - 1] && result == 1 + dp[i - 1][j - 1][k - 1])
        {
            scs.push_back(X[i - 1]);
            --i, --j, --k, ++index;
            continue;
        }
        if (i && j && X[i - 1] == Y[j - 1] && USE_FILTER(filter_mask, MASK_AB) && result == 1 + dp[i - 1][j - 1][k])
        {
            scs.push_back(X[i - 1]);
            --i, --j, ++index;
            continue;
        }
        if (i && k && X[i - 1] == Z[k - 1] && USE_FILTER(filter_mask, MASK_AC) && result == 1 + dp[i - 1][j][k - 1])
        {
            scs.push_back(X[i - 1]);
            --i, --k, ++index;
            continue;
        }
        if (j && k && Y[j - 1] == Z[k - 1] && USE_FILTER(filter_mask, MASK_BC) && result == 1 + dp[i][j - 1][k - 1])
        {
            scs.push_back(Y[j - 1]);
            --j, --k, ++index;
            continue;
        }
        if (i && USE_FILTER(filter_mask, MASK_A) && result == 1 + dp[i - 1][j][k])
        {
            scs.push_back(X[i - 1]);
            --i, ++index;
            continue;
        }
        if (j && USE_FILTER(filter_mask, MASK_B) && result == 1 + dp[i][j - 1][k])
        {
            scs.push_back(Y[j - 1]);
            --j, ++index;
            continue;
        }
        if (k && USE_FILTER(filter_mask, MASK_C) && result == 1 + dp[i][j][k - 1])
        {
            scs.push_back(Z[k - 1]);
            --k, ++index;
            continue;
        }
    }
    std::reverse(scs.begin(), scs.end());
    return scs;
}


std::string ConvertFilterToStr(uint32_t filter) {
    switch (filter)
    {
    case MASK_A:
        return "MASK_A";
    case MASK_B:
        return "MASK_B";
    case MASK_C:
        return "MASK_C";
    case MASK_AB:
        return "MASK_AB";
    case MASK_AC:
        return "MASK_AC";
    case MASK_BC:
        return "MASK_BC";
    default:
        return "";
    }
}

// Driver program to test above function 
int main()
{
#ifdef CREATE_FILTERS_MAP
    std::ofstream filtersMapFile("new_filtersMap.json");
#endif //CREATE_FILTERS_MAP

    OutputsDir.append(std::to_string(SIZE)).append("\\");

    char buff[FILENAME_MAX]; 
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);

    current_working_dir.append(OutputsDir);


    _mkdir(current_working_dir.c_str());

    std::map<int, unsigned long long> sequencesMap;

    auto tmpLogFilePath1 = current_working_dir;
    auto tmpLogFilePath2 = current_working_dir;

    //std::vector<int> allFilters { MASK_A , MASK_B, MASK_C, MASK_AB, MASK_AC, MASK_BC };

    std::list<std::string> all_words;
    char arr[] = { 'A', 'C', 'G', 'T' };
    int n = sizeof(arr) / sizeof(arr[0]);
    int w_size = SIZE;


    CombinationRepetition(arr, n, w_size, &all_words);
    std::vector<std::string> allWordsVec;
    std::copy(all_words.begin(), all_words.end(), std::back_inserter(allWordsVec));
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    std::shuffle(allWordsVec.begin(), allWordsVec.end(),e);

    auto sequencesCounter = 0;
    std::string origScs = "";
    std::string scs = "";
    std::string sequences = "";
    auto allFilters = MASK_A | MASK_B | MASK_C | MASK_AB | MASK_AC | MASK_BC;

    std::ofstream outfileDiffs(tmpLogFilePath1.append("outfileDiffs.txt"));
    std::ofstream outfileCounts(tmpLogFilePath2.append("outfileCounts.txt"));

    std::cout << "Starting..\n" << std::endl;

    for (size_t i = 0; i < POOL_SIZE; i++)
    {
        srand(std::chrono::system_clock::now().time_since_epoch().count());
        auto x = allWordsVec.at(rand() % all_words.size());
        auto y = allWordsVec.at(rand() % all_words.size());
        auto z = allWordsVec.at(rand() % all_words.size());
        origScs = findScs(x, y, z, allFilters);
        scs = findScs(x, y, z, currentFilters);
        if (!scs.compare("ERROR")) {
            sequencesMap[-1]++;
        }
        else {
            sequencesMap[scs.size() - origScs.size()]++;//.push_back(sequences);
        }
    }
  
     

// simulates into groups
    std::map<int, unsigned long long>::iterator it;
    for (it = sequencesMap.begin(); it != sequencesMap.end(); it++) {
        outfileDiffs << it->first << std::endl;
        outfileCounts << it->second << std::endl;
    }
    outfileDiffs.close();
    outfileCounts.close();

    return 0; 
} 