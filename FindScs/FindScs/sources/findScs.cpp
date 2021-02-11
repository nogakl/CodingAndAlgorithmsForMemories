//#include <bits/stdc++.h> 
#include <iostream>
#include <string>
#include <list>
#include <stdio.h>
#include <fstream>
#include "../JsonParser.h"

using namespace std; 
  
static constexpr int SIZE = 2;
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
    printf(msg);\
    return "ERROR";

#define USE_FILTER(MASK, FILTER) (MASK & FILTER)


string printShortestSuperSeq(string X, string Y, string Z, int8_t filter_mask)
{

    int dp[SIZE + 1][SIZE + 1][SIZE + 1];
    // dp[i][j] contains length of shortest supersequence 
    // for X[0..i-1] and Y[0..j-1] and Z[0..k-1]

    // Fill table in bottom up manner 
    for (int i = 0; i <= SIZE; i++)
    {
        for (int j = 0; j <= SIZE; j++)
        {
            for (int k = 0; k <= SIZE; ++k)
            {
                // Below steps follow recurrence relation 

             //step 1: at least one of those words ended before the call
                if (i == 0) {
                    if (j == 0) {
                        if (USE_FILTER(filter_mask, MASK_C))
                        {
                            dp[i][j][k] = k; //filter c
                            continue;
                        }
                        RETURN_ERR("need filter c\n");
                    }
                    if (k == 0) {
                        if (USE_FILTER(filter_mask, MASK_B))
                        {
                            dp[i][j][k] = j; //filter b
                            continue;
                        }
                        RETURN_ERR("need filter b\n");
                    }
                    if (Y[SIZE - j] == Z[SIZE - k])
                    {
                        if (USE_FILTER(filter_mask, MASK_BC))
                        {
                            dp[i][j][k] = 1 + dp[i][j - 1][k - 1]; //filter bc
                            continue;
                        }
                    }
                    if (USE_FILTER(filter_mask, MASK_B) && USE_FILTER(filter_mask, MASK_C))
                    {
                        dp[i][j][k] = 1 + min(dp[i][j - 1][k], dp[i][j][k - 1]); //filter b or c 
                        continue;
                    }
                    RETURN_ERR("need filter b and c or bc\n");
                }
                else if (j == 0)
                {
                    if (k == 0)
                    {
                        if (USE_FILTER(filter_mask, MASK_A))
                        {
                            dp[i][j][k] = i; //filter a
                            continue;
                        }
                        RETURN_ERR("need filter a\n");
                    }
                    if (X[SIZE - i] == Z[SIZE - k])
                    {
                        if (USE_FILTER(filter_mask, MASK_AC))
                        {
                            dp[i][j][k] = 1 + dp[i - 1][j][k - 1]; //filter ac
                            continue;
                        }
                    }

                    if (USE_FILTER(filter_mask, MASK_A) && USE_FILTER(filter_mask, MASK_C))
                    {
                        dp[i][j][k] = 1 + min(dp[i - 1][j][k], dp[i][j][k - 1]); //filter a or c
                        continue;
                    }
                    RETURN_ERR("need filter a and c or ac\n");
                }
                if (k == 0)
                {
                    if (X[SIZE - i] == Y[SIZE - j])
                    {
                        if (USE_FILTER(filter_mask, MASK_AB))
                        {
                            dp[i][j][k] = 1 + dp[i - 1][j - 1][k]; //filter ab
                            continue;
                        }
                    }
                    if (USE_FILTER(filter_mask, MASK_A) && USE_FILTER(filter_mask, MASK_B))
                    {
                        dp[i][j][k] = 1 + min(dp[i - 1][j][k], dp[i][j - 1][k]); //filter a or b
                        continue;
                    }
                    RETURN_ERR("need filter a and b or ab\n");
                }

                //step 2: all the words include at least one char
                if (X[SIZE - i] == Y[SIZE - j] && X[SIZE - i] == Z[SIZE - k])
                {
                    dp[i][j][k] = 1 + dp[i - 1][j - 1][k - 1]; //filter abc
                    continue;
                }
                if (X[SIZE - i] == Y[SIZE - j])
                {
                    int tmp = INT_MAX;
                    tmp = USE_FILTER(filter_mask, MASK_AB) ? 1 + dp[i - 1][j - 1][k] : tmp;
                    tmp = USE_FILTER(filter_mask, MASK_C) ? min(tmp, 1 + dp[i][j][k - 1]) : tmp;
                    tmp = USE_FILTER(filter_mask, MASK_A) ? min(tmp, 1 + dp[i - 1][j][k]) : tmp;
                    tmp = USE_FILTER(filter_mask, MASK_B) ? min(tmp, 1 + dp[i][j - 1][k]) : tmp;

                    if (tmp == INT_MAX)
                    {
                        RETURN_ERR("need filter a and b and c or ab\n");
                    }
                    dp[i][j][k] = tmp;
                    continue;
                }
                if (X[SIZE - i] == Z[SIZE - k])
                {
                    int tmp = INT_MAX;
                    tmp = USE_FILTER(filter_mask, MASK_AC) ? 1 + dp[i - 1][j][k - 1] : tmp;
                    tmp = USE_FILTER(filter_mask, MASK_C) ? min(tmp, 1 + dp[i][j][k - 1]) : tmp;
                    tmp = USE_FILTER(filter_mask, MASK_A) ? min(tmp, 1 + dp[i - 1][j][k]) : tmp;
                    tmp = USE_FILTER(filter_mask, MASK_B) ? min(tmp, 1 + dp[i][j - 1][k]) : tmp;

                    if (tmp == INT_MAX)
                    {
                        RETURN_ERR("need filter a and b and c or ab\n");
                    }
                    dp[i][j][k] = tmp;
                    continue;
                }
                if (Y[SIZE - j] == Z[SIZE - k])
                {
                    int tmp = INT_MAX;
                    tmp = USE_FILTER(filter_mask, MASK_BC) ? 1 + dp[i][j - 1][k - 1] : tmp;
                    tmp = USE_FILTER(filter_mask, MASK_C) ? min(tmp, 1 + dp[i][j][k - 1]) : tmp;
                    tmp = USE_FILTER(filter_mask, MASK_A) ? min(tmp, 1 + dp[i - 1][j][k]) : tmp;
                    tmp = USE_FILTER(filter_mask, MASK_B) ? min(tmp, 1 + dp[i][j - 1][k]) : tmp;

                    if (tmp == INT_MAX)
                    {
                        RETURN_ERR("need filter a and b and c or ab\n");
                    }
                    dp[i][j][k] = tmp;
                    continue;
                }
                int tmp = INT_MAX;
                tmp = USE_FILTER(filter_mask, MASK_C) ? min(tmp, 1 + dp[i][j][k - 1]) : tmp;
                tmp = USE_FILTER(filter_mask, MASK_A) ? min(tmp, 1 + dp[i - 1][j][k]) : tmp;
                tmp = USE_FILTER(filter_mask, MASK_B) ? min(tmp, 1 + dp[i][j - 1][k]) : tmp;

                if (tmp == INT_MAX)
                {
                    RETURN_ERR("need filter a and b and c \n");
                }
                dp[i][j][k] = tmp;
                continue;
            }
        }
    }

    // Following code is used to print shortest supersequence 

    // dp[m][n] stores the length of the shortest supersequence 
    // of X and Y 
    int index = dp[SIZE][SIZE][SIZE];

    // string to store the shortest supersequence 
    string scs;

    // Start from the bottom right corner and one by one 
    // push characters in output string 
    int i = SIZE, j = SIZE, k = SIZE;

    while (i > 0 || j > 0 || k > 0)
    {
        if (i && j && k && X[SIZE - i] == Y[SIZE - j] && X[SIZE - i] == Z[SIZE - k]) //filter abc
        {
            scs.push_back(X[SIZE - i]);
            --i, --j, --k, --index;
            continue;
        }
        if (i && j && X[SIZE - i] == Y[SIZE - j]) //filter ab or c
        {
            if (k && USE_FILTER(filter_mask, MASK_C) && dp[i - 1][j - 1][k] >= dp[i][j][k - 1] && dp[i - 1][j][k] >= dp[i][j][k - 1] && dp[i][j - 1][k] >= dp[i][j][k - 1])
            {
                scs.push_back(Z[SIZE - k]);
                --k, --index;
                continue;
            }
            if (USE_FILTER(filter_mask, MASK_B) && dp[i - 1][j - 1][k] >= dp[i][j - 1][k] && dp[i - 1][j][k] >= dp[i][j - 1][k] && (!k || dp[i][j][k - 1] >= dp[i][j - 1][k]))
            {
                scs.push_back(Y[SIZE - j]);
                --j, --index;
                continue;
            }
            if (USE_FILTER(filter_mask, MASK_A) && dp[i - 1][j - 1][k] >= dp[i - 1][j][k] && dp[i][j - 1][k] >= dp[i - 1][j][k] && (!k || dp[i][j][k - 1] >= dp[i - 1][j][k]))
            {
                scs.push_back(X[SIZE - i]);
                --i, --index;
                continue;
            }
            if (USE_FILTER(filter_mask, MASK_AB) && dp[i - 1][j][k] >= dp[i - 1][j - 1][k] && dp[i][j - 1][k] >= dp[i - 1][j - 1][k] && dp[i][j][k - 1] >= dp[i - 1][j - 1][k])
            { //filter ab
                scs.push_back(X[SIZE - i]);
                --i, --j, --index;
                continue;
            }
            RETURN_ERR("shouldn't get here\n");
        }
        if (i && k && X[SIZE - i] == Z[SIZE - k]) //filter ac or b
        {
            if (USE_FILTER(filter_mask, MASK_C) && dp[i - 1][j][k - 1] >= dp[i][j][k - 1] && dp[i - 1][j][k] >= dp[i][j][k - 1] && (!j || dp[i][j - 1][k] >= dp[i][j][k - 1]))
            {
                scs.push_back(Z[SIZE - k]);
                --k, --index;
                continue;
            }
            if (j && USE_FILTER(filter_mask, MASK_B) && dp[i - 1][j][k - 1] >= dp[i][j - 1][k] && dp[i - 1][j][k] >= dp[i][j - 1][k] && dp[i][j][k - 1] >= dp[i][j - 1][k])
            {
                scs.push_back(Y[SIZE - j]);
                --j, --index;
                continue;
            }
            if (USE_FILTER(filter_mask, MASK_A) && dp[i - 1][j][k - 1] >= dp[i - 1][j][k] && (!j || dp[i][j - 1][k] >= dp[i - 1][j][k]) && dp[i][j][k - 1] >= dp[i - 1][j][k])
            {
                scs.push_back(X[SIZE - i]);
                --i, --index;
                continue;
            }
            if (USE_FILTER(filter_mask, MASK_AC) && dp[i - 1][j][k] >= dp[i - 1][j][k - 1] && (!j || dp[i][j - 1][k] >= dp[i - 1][j][k - 1]) && dp[i][j][k - 1] >= dp[i - 1][j][k - 1])
            { //filter ac
                scs.push_back(X[SIZE - i]);
                --i, --k, --index;
                continue;
            }
            RETURN_ERR("shouldn't get here\n");
        }
        if (j && k && Z[SIZE - k] == Y[SIZE - j]) //filter bc or a
        {
            if (USE_FILTER(filter_mask, MASK_C) && dp[i][j - 1][k - 1] >= dp[i][j][k - 1] && (!i || dp[i - 1][j][k] >= dp[i][j][k - 1]) && dp[i][j - 1][k] >= dp[i][j][k - 1])
            {
                scs.push_back(Z[SIZE - k]);
                --k, --index;
                continue;
            }
            if (USE_FILTER(filter_mask, MASK_B) && dp[i][j - 1][k - 1] >= dp[i][j - 1][k] && (!i || dp[i - 1][j][k] >= dp[i][j - 1][k]) && dp[i][j][k - 1] >= dp[i][j - 1][k])
            {
                scs.push_back(Y[SIZE - j]);
                --j, --index;
                continue;
            }
            if (i && USE_FILTER(filter_mask, MASK_A) && dp[i][j - 1][k - 1] >= dp[i - 1][j][k] && dp[i][j - 1][k] >= dp[i - 1][j][k] && dp[i][j][k - 1] >= dp[i - 1][j][k])
            {
                scs.push_back(X[SIZE - i]);
                --i, --index;
                continue;
            }
            if (USE_FILTER(filter_mask, MASK_AC) && (!i || dp[i - 1][j][k] >= dp[i][j - 1][k - 1]) && dp[i][j - 1][k] >= dp[i][j - 1][k - 1] && dp[i][j][k - 1] >= dp[i][j - 1][k - 1])
            { //filter bc
                scs.push_back(Y[SIZE - j]);
                --j, --k, --index;
                continue;
            }
            RETURN_ERR("shouldn't get here\n");
        }

        if (i && USE_FILTER(filter_mask, MASK_A) && (!j || dp[i - 1][j][k] <= dp[i][j - 1][k]))
        {
            if (!k || dp[i - 1][j][k] <= dp[i][j][k - 1]) //filter a
            {
                scs.push_back(X[SIZE - i]);
                --i, --index;
                continue;
            }
        }
        if (j && USE_FILTER(filter_mask, MASK_B) && (!i || dp[i][j - 1][k] <= dp[i - 1][j][k]))
        {
            if (!k || dp[i][j - 1][k] <= dp[i][j][k - 1]) //filter b
            {
                scs.push_back(Y[SIZE - j]);
                --j, --index;
                continue;
            }
        }
        if (k && USE_FILTER(filter_mask, MASK_C) && (!i || dp[i][j][k - 1] <= dp[i - 1][j][k]))
        {
            if (!j || dp[i][j][k - 1] <= dp[i][j - 1][k]) //filter c
            {
                scs.push_back(Z[SIZE - k]);
                --k, --index;
                continue;
            }
        }
    }


    // reverse the string and return it 
   // reverse(scs.begin(), scs.end()); 
    return scs;
}

void CombinationRepetitionUtil(char chosen[], char arr[],
    int index, int r, int end, std::list<string> *all_words)
{
    // Since index has become r, current combination is 
    // ready to be printed, print 
    string str;
    if (index == r)
    {
        for (int i = 0; i < r; i++)
            str.push_back(arr[chosen[i]]);
        reverse(str.begin(), str.end());
        all_words->push_front(str);
        return;
    }

    // One by one choose all elements (without considering 
    // the fact whether element is already chosen or not) 
    // and recur 
    for (int i = 0; i <= end; i++)
    {
        chosen[index] = i;
        CombinationRepetitionUtil(chosen, arr, index + 1,
            r, end, all_words);
    }
    return;
}

// The main function that prints all combinations of size r 
// in arr[] of size n with repitions. This function mainly 
// uses CombinationRepetitionUtil() 
void CombinationRepetition(char arr[], int n, int w_size, std::list<string> *all_words)
{
    // Allocate memory 
    char* chosen{ new char[w_size + 1] {} };

    // Call the recursice tfunction 
    CombinationRepetitionUtil(chosen, arr, 0, w_size, n - 1, all_words);

    delete chosen;
}


void ParseLogFile(std::string path) {
    std::string x = "";
    std::string y = "";
    std::string z = "";
    int8_t filters = 0;

    string line;
    ifstream myfile;
    myfile.open(path);

    while (getline(myfile, line)) {
        ParseJsonRecord(line, x, y, z, filters);
        cout << line << endl;
        //TODO:: print/garph the result..
    }
}

vector<int> getOnLocations(int a) {
    vector<int> result;
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

template<typename T>
vector<vector<T> > powerSet(const vector<T>& set) {
    vector<vector<T> > result;
    int numPowerSets = static_cast<int>(pow(2.0, static_cast<double>(set.size())));
    for (size_t i = 0; i < numPowerSets; ++i) {
        vector<int> onLocations = getOnLocations(i);
        vector<T> subSet;
        for (size_t j = 0; j < onLocations.size(); ++j) {
            subSet.push_back(set.at(onLocations.at(j)));
        }
        result.push_back(subSet);
    }
    return result;
}

// Driver program to test above function 
int main()
{
    std::ofstream outfile;
    std::string filename = "tmp.json";
    std::vector<int> allFilters { MASK_A , MASK_B, MASK_C, MASK_AB, MASK_AC, MASK_BC };
    auto filters_combinations = powerSet(allFilters);
    auto i = 0;
    for (auto filterVec: filters_combinations)
    {
        auto currentFilter = 0;
        for (auto filter : filterVec) {
            currentFilter |= filter;
        }
        outfile.open(filename, std::ios_base::app | std::ios_base::out); // append instead of overwrite

        std::list<string> all_words;
        char arr[] = { 'A', 'C', 'G', 'T' };
        int n = sizeof(arr) / sizeof(arr[0]);
        int w_size = SIZE;
        auto count = 0;
        CombinationRepetition(arr, n, w_size, &all_words);

        string scs;

        for (string x : all_words) {
            {
                for (string y : all_words)
                {
                    if (x.compare(y))
                    {
                        for (string z : all_words)
                        {
                            if (x.compare(z) && y.compare(z))
                            {
                                /*currentFilter = MASK_AB | MASK_C| MASK_B | MASK_BC | MASK_AC | MASK_A;
                                x = "A";
                                y = "A";
                                z = "G";*/
                                scs = printShortestSuperSeq(x, y, z, currentFilter);
                                if (!scs.compare("ERROR")) {  //we need more filters//
                                    std::string tmp = CreateJsonRecord(x, y, z, currentFilter);
                                    outfile << tmp << std::endl;
                                    count++;
                                }
                            }
                        }
                    }
                }
            }
        }

        std::string new_filename = std::to_string(i);
        new_filename.append("_failures_size_");
        new_filename.append(std::to_string(SIZE));
        new_filename.append("_errors_count_");
        new_filename.append(std::to_string(count));
        new_filename.append(".json");
        outfile.close();
        rename(filename.c_str(), new_filename.c_str());
        i++;
    }
    return 0; 
} 