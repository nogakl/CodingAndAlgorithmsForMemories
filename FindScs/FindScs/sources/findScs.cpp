//#include <bits/stdc++.h> 
#include "JsonParser.h"
#include "TestUtils.h"
#include <direct.h>

static constexpr int SIZE = 1;
static std::string outputsDir = "\\outputs_size_";
#define GetCurrentDir _getcwd

// returns shortest supersequence of X and Y and Z

#define ALL_FILTERS 0xFD
#define NO_FILTERS 0x00
#define MASK_A 0x80
#define MASK_B 0x40
#define MASK_C 0x20
#define MASK_AB 0x10
#define MASK_AC 0x08
#define MASK_BC 0x04

#define RETURN_ERR(msg)\
    /*printf(msg);*/\
    return "ERROR";

#define USE_FILTER(MASK, FILTER) (MASK & FILTER)


std::string findScs(std::string X, std::string Y, std::string Z, uint32_t filter_mask)
{
    int dp[SIZE + 1][SIZE + 1][SIZE + 1];
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
                int min = INT_MAX;
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
                if (j != 0 && k != 0 && USE_FILTER(filter_mask, MASK_AC) && Y[j - 1] == Z[k - 1])
                    min = min > (res = 1 + dp[i][j - 1][k - 1]) ? res : min;
                if (i != 0 && j != 0 && k != 0 && X[i - 1] == Y[j - 1] && X[i - 1] == Z[k - 1])
                    min = min > (res = 1 + dp[i - 1][j - 1][k - 1]) ? res : min;

                if (min == INT_MAX)
                {
                    RETURN_ERR("need more filters\n")
                }

                dp[i][j][k] = min;
            }
        }
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
    return scs;
}


std::string GetLogFilePath(std::string current_working_dir, int filterCounter, int size, int errorsCount) {
    std::string logFilePath = current_working_dir;
    logFilePath.append(std::to_string(errorsCount));
    logFilePath.append("_errors_TN_");
    logFilePath.append(std::to_string(filterCounter));
    logFilePath.append(".json");
    return logFilePath;
}


// Driver program to test above function 
int main()
{
    outputsDir.append(std::to_string(SIZE)).append("\\");
    char buff[FILENAME_MAX]; 
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    current_working_dir.append(outputsDir);
    _mkdir(current_working_dir.c_str());
    auto tmpLogFilePath = current_working_dir;
    tmpLogFilePath.append("tmp.json");

    std::ofstream outfile;
    std::vector<int> allFilters { MASK_A , MASK_B, MASK_C, MASK_AB, MASK_AC, MASK_BC };
    auto filters_combinations = powerSet(allFilters);
    auto filterCounter = 0;
    for (auto filterVec: filters_combinations)
    {
        uint32_t currentFilter = 0;
        for (auto filter : filterVec) {
            currentFilter |= filter;
        }
        outfile.open(tmpLogFilePath, std::ios_base::out); 

        outfile << CreatJsonHeader(currentFilter)<< std::endl;
        std::list<std::string> all_words;
        char arr[] = { 'A', 'C', 'G', 'T' };
        int n = sizeof(arr) / sizeof(arr[0]);
        int w_size = SIZE;
        auto errorsCount = 0;
        CombinationRepetition(arr, n, w_size, &all_words);

        std::string scs;

        for (std::string x : all_words) {
            {
                for (std::string y : all_words)
                {
                    if (x.compare(y))
                    {
                        for (std::string z : all_words)
                        {
                            if (x.compare(z) && y.compare(z))
                            {
                                /*currentFilter = MASK_C | MASK_AB| MASK_A | MASK_C;
                                x = "A";
                                y = "A";
                                z = "G";*/
                                scs = findScs(x, y, z, currentFilter);
                                if (!scs.compare("ERROR")) {  //we need more filters//
                                    std::string tmp = CreateJsonRecord(x, y, z);
                                    outfile << tmp << std::endl;
                                    errorsCount++;
                                }
                            }
                        }
                    }
                }
            }
        }

        outfile.close();
        auto logFilePath = GetLogFilePath(current_working_dir, filterCounter, SIZE, errorsCount);
        rename(tmpLogFilePath.c_str(), logFilePath.c_str());
        filterCounter++;
    }
    remove(tmpLogFilePath.c_str());
    return 0; 
} 