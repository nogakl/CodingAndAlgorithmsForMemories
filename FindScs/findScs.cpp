//#include <bits/stdc++.h> 
#include <iostream>
#include <string>
#include <list>
#include <stdio.h>
#include <limits.h>

using namespace std; 
  
static constexpr int SIZE = 4;
// returns shortest supersequence of X and Y 

/*
    mask : 0xabcdefXX
    a - use filter a 0x80
    b - use filter b 0x40
    c - use filter c 0x20
    d - use filter ab 0x10
    e - use filter ac 0x08
    f - use filter bc 0x04
*/

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

string findScs(string X, string Y, string Z, int8_t filter_mask)
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
                    min = min >  (res = !i && !k ? j : 1 + dp[i][j - 1][k]) ? res : min;
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
    string scs;
    while (index < dp[SIZE][SIZE][SIZE])
    {
        int result = dp[i][j][k];
        if (i && j && k && X[i - 1] == Y[j - 1] && Z[k - 1] == X[i - 1] && result == 1 + dp[i - 1][j - 1][k - 1])
        {
            scs.push_back(X[i - 1]);
            --i, --j, --k, ++index;
            continue;
        }
        if(i && j && X[i - 1] == Y[j - 1] && USE_FILTER(filter_mask, MASK_AB) && result == 1 + dp[i - 1][j - 1][k])
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
    CombinationRepetitionUtil(chosen, arr, 0, w_size, n-1, all_words); 

    delete chosen;
} 

// Driver program to test above function 
int main() 
{ 
    std::list<string> all_words;
    char arr[] = {'A', 'C', 'G', 'T'}; 
    int n = sizeof(arr)/sizeof(arr[0]); 
    int w_size = SIZE; 
    CombinationRepetition(arr, n, w_size, &all_words);

    string scs;

    int max_cyc = 0;
    string max_word;
    string a;
    string b;
    string c;

    for (string x : all_words) {
        {
            for (string y : all_words)
            {
                if (x.compare(y))
                {
                    for(string z : all_words)
                    {
                        if (x.compare(z) && y.compare(z))
                        {

                            printf("WORDS: %s, %s, %s\n", x.c_str(), y.c_str(), z.c_str());
                            scs = findScs(x, y, z, MASK_A | MASK_AB | MASK_BC);
                            printf("takes %d cycles\n", scs.size());
                            if (max_cyc < scs.size())
                            {
                                max_cyc = scs.size();
                                max_word = scs;
                                a = x;
                                b = y;
                                c = z;
                            }

                           
                        }
                    }
                }
            }
        }
    }

    printf("MAX: %s of %d cyc, %s\n", max_word.c_str(), max_cyc);
   /* 
    string X = "abc"; 
    string Y = "abc"; 
    string Z = "cbc";
  
    printf("%s\n",printShortestSuperSeq(X, Y, Z).c_str()); 
  */
    return 0; 
} 