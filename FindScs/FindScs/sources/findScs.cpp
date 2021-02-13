//#include <bits/stdc++.h> 
#include "JsonParser.h"
#include "TestUtils.h"
#include <direct.h>

//#define CREATE_FILTERS_MAP
#define PLOT_ALL_WORDS
static constexpr int SIZE = 3;

static std::string OutputsDir = "\\outputs_size_";
//static std::string failedOutputsDir = "\\failed_outputs_size_";
//static std::string partialSuccessOutputsDir = "\\partial_success_outputs_size_";
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

std::string findScs(std::string X, std::string Y, std::string Z, int8_t filter_mask)
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


std::string GetLogFilePath(std::string current_working_dir, int sequencesCounter,std::string type) {
    std::string logFilePath = current_working_dir;
    logFilePath.append(std::to_string(sequencesCounter));
    logFilePath.append("_");
    logFilePath.append(type);
    logFilePath.append(".json");
    return logFilePath;
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

    //failedOutputsDir.append(std::to_string(SIZE)).append("\\");
    //partialSuccessOutputsDir.append(std::to_string(SIZE)).append("\\");

    OutputsDir.append(std::to_string(SIZE)).append("\\");

    char buff[FILENAME_MAX]; 
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);

    //auto current_working_dir_cpy = current_working_dir;

    //auto workingDirFailed = current_working_dir.append(failedOutputsDir);
    //auto workingDirParSec = current_working_dir_cpy.append(partialSuccessOutputsDir);

    current_working_dir.append(OutputsDir);

    //_mkdir(workingDirFailed.c_str());
    //_mkdir(workingDirParSec.c_str());


    _mkdir(current_working_dir.c_str());


    /*auto tmpFailedLogFilePath = workingDirFailed;
    auto tmpPsLogFilePath = workingDirParSec;

    tmpFailedLogFilePath.append("tmp.json");
    tmpPsLogFilePath.append("tmp.json");*/



    //std::ofstream failedOutfile;
    //std::ofstream psOutfile;

    std::map<std::vector<int>, std::vector<std::string>> sequencesMap;

    auto tmpLogFilePath = current_working_dir;
    tmpLogFilePath.append("tmp.json");
    std::ofstream outfile;

    std::vector<int> allFilters { MASK_A , MASK_B, MASK_C, MASK_AB, MASK_AC, MASK_BC };
    auto filters_combinations = powerSet(allFilters);
    //auto filterCounter = 0;
#ifdef CREATE_FILTERS_MAP
        filtersMapFile << CreateJsonFlilter(filterCounter, filtersStr) << std::endl;
#endif // CREATE_FILTERS_MAP


        /*failedOutfile.open(tmpFailedLogFilePath, std::ios_base::out);
        psOutfile.open(tmpPsLogFilePath, std::ios_base::out); 

        failedOutfile << CreateJsonHeader(filtersStr)<< std::endl;
        psOutfile << CreateJsonHeader(filtersStr) << std::endl;*/


        std::list<std::string> all_words;
        char arr[] = { 'A', 'C', 'G', 'T' };
        int n = sizeof(arr) / sizeof(arr[0]);
        int w_size = SIZE;
        auto failedErrorsCount = 0;
        auto psErrorsCount = 0;

        CombinationRepetition(arr, n, w_size, &all_words);
        auto sequencesCounter = 0;
        std::string scs;

        for (std::string x : all_words)
        {
            for (std::string y : all_words)
            {
                for (std::string z : all_words)
                {
                    auto origScs = findScs(x, y, z, MASK_A | MASK_B | MASK_C | MASK_AB | MASK_AC | MASK_BC);
#ifdef PLOT_ALL_WORDS
                    outfile.open(tmpLogFilePath, std::ios_base::out);
                    outfile << CreateJsonRecord(x, y, z, origScs) << std::endl;
#endif //PLOT_ALL_WORDS

                    std::vector<int> filtersResults;
                    for (auto filterVec : filters_combinations)
                    {
                        std::string filtersStr = " ";
                        if (filterVec.size() <= 2) {
                            continue;
                        }
                        uint32_t currentFilter = 0;
                        for (auto filter : filterVec) {
                            currentFilter |= filter;
                            filtersStr.append(ConvertFilterToStr(filter));
                            filtersStr.append(" | ");
                        }
                        //TODO!!
                        //outfile << CreateJsonRecord(x, y, z) << std::endl;
                        //if (x.compare(z) && y.compare(z))
                        //{
                            /*currentFilter = MASK_AC | MASK_AB | MASK_BC  /*| MASK_AC | MASK_A; /* MASK_AC | MASK_AB | MASK_BC;
                            x = "CT";
                            y = "GT";
                            z = "GC";*/
                        scs = findScs(x, y, z, currentFilter);
                        auto diff = 0;
                        if (!scs.compare("ERROR")) {  //we need more filters//
                            diff = -1;
                        }
                        else {
                            diff = scs.size() - origScs.size();
                        }
                        filtersResults.push_back(diff);
#ifdef PLOT_ALL_WORDS
                        outfile << CreateComplexJsonRecord(filtersStr, scs, diff) << std::endl;
#endif //PLOT_ALL_WORDS
                    }
                    std::string sequences = x;
                    sequences.append(",");
                    sequences.append(y);
                    sequences.append(",");
                    sequences.append(z);
                    sequencesMap[filtersResults].push_back(sequences);

#ifdef PLOT_ALL_WORDS
                    outfile.close();
                    auto logFilePath = GetLogFilePath(current_working_dir, sequencesCounter, "seq_triple"/*, failedErrorsCount*/);
                    rename(tmpLogFilePath.c_str(), logFilePath.c_str());
#endif // PLOT_ALL_WORDS

                    sequencesCounter++;
                }
                           
            }
        }
        remove(tmpLogFilePath.c_str());


    // sumulates into groups
        std::map<std::vector<int>, std::vector<std::string>>::iterator it;
        auto resultsCounter = 0;
        for (it = sequencesMap.begin(); it != sequencesMap.end(); it++)
        {
            outfile.open(tmpLogFilePath, std::ios_base::out);
            outfile << CreateResultJsonHeader(it->first) <<std:: endl;
            for (auto sequences : it->second) {
                outfile << CreateResultJsonRecord(sequences) << std::endl;
            }
            outfile.close();
            auto logFilePath = GetLogFilePath(current_working_dir, resultsCounter, "results_group");
            rename(tmpLogFilePath.c_str(), logFilePath.c_str());
            resultsCounter++;
        }
    return 0; 
} 