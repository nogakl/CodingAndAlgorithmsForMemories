//#include <bits/stdc++.h> 
#include <iostream>
#include <string>
#include <list>
using namespace std; 
  
static constexpr int SIZE = 5;
// returns shortest supersequence of X and Y 
string printShortestSuperSeq(const string X, const  string Y, const  string Z)
{ 
  
    int dp[SIZE][SIZE][SIZE];
    // dp[i][j] contains length of shortest supersequence 
    // for X[0..i-1] and Y[0..j-1] and Z[0..k-1]
  
    // Fill table in bottom up manner 
    for (int i = 0; i < SIZE; i++) 
    { 
        for (int j = 0; j < SIZE; j++)
        {
            for (int k = 0; k < SIZE; ++k)
            {
                // Below steps follow recurrence relation 

             //step 1: at least one of those words ended before the call
                if (i == 0) {
                    if (j == 0) {
                        dp[i][j][k] = k; //filter c
                        continue;
                    }
                    if (k == 0) {
                        dp[i][j][k] = j; //filter b
                        continue;
                    }
                    if (Y[j - 1] == Z[k - 1])
                    {
                        dp[i][j][k] = 1 + dp[i][j - 1][k - 1]; //filter bc
                        continue;
                    }

                    dp[i][j][k] = 1 + min(dp[i][j - 1][k], dp[i][j][k - 1]); //filter b or c 
                    continue;
                }
                else if (j == 0)
                {
                    if (k == 0)
                    {
                        dp[i][j][k] = i; //filter a
                        continue;
                    }
                    if (X[i - 1] == Z[k - 1])
                    {
                        dp[i][j][k] = 1 + dp[i - 1][j][k - 1]; //filter ac
                        continue;
                    }

                    dp[i][j][k] = 1 + min(dp[i - 1][j][k], dp[i][j][k - 1]); //filter a or c
                    continue;
                }
                if (k == 0)
                {
                    if (X[i - 1] == Y[j - 1])
                    {
                        dp[i][j][k] = 1 + dp[i - 1][j - 1][k]; //filter ab
                        continue;
                    }
                    dp[i][j][k] = 1 + min(dp[i - 1][j][k], dp[i][j - 1][k]); //filter a or b
                    continue;
                }

                //step 2: all the words include at least one char
                if (X[i - 1] == Y[j - 1] && X[i - 1] == Z[k - 1])
                {
                    dp[i][j][k] = 1 + dp[i - 1][j - 1][k - 1]; //filter abc
                    continue;
                }
                if (X[i - 1] == Y[j - 1])
                {
                    dp[i][j][k] = 1 + min(dp[i - 1][j - 1][k], dp[i][j][k - 1]); //filter ab or c
                    continue;
                }
                if (X[i - 1] == Z[k - 1])
                {
                    dp[i][j][k] = 1 + min(dp[i - 1][j][k - 1], dp[i][j - 1][k]); //filter ac or b
                    continue;
                }
                if (Y[j - 1] == Z[k - 1])
                {
                    dp[i][j][k] = 1 + min(dp[i][j - 1][k - 1], dp[i - 1][j][k]); //filter ac or a
                    continue;
                }
                dp[i][j][k] = 1 + min(dp[i - 1][j][k], min(dp[i][j - 1][k], dp[i][j][k - 1])); //filter a or b or c
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
        if(i && j && k && X[i - 1] == Y[j - 1] && X[i - 1] == Z[k - 1]) //filter abc
        {
            scs.push_back(X[i - 1]);
            --i,--j,--k, --index;
            continue;
        }
        if(i && j && X[i - 1] == Y[j - 1]) //filter ab or c
        {
            if(k && dp[i - 1][j - 1][k] >= dp[i][j][k - 1]) //filter c
            {
                scs.push_back(Z[k - 1]);
                --k, --index;
                continue;
            }
            else{ //filter ab
                scs.push_back(X[i - 1]);
                --i,--j, --index;
                continue;
            }
        }
        if(i && k && X[i - 1] == Z[k - 1]) //filter ac or b
        {
            if(j && dp[i - 1][j][k - 1] >= dp[i][j - 1][k]) //filter b
            {
                scs.push_back(Y[j - 1]);
                --j, --index;
                continue;
            }
            else{ //filter ac
                scs.push_back(X[i - 1]);
                --i,--k, --index;
                continue;
            }
        }
        if(j && k && Z[k - 1] == Y[j - 1]) //filter bc or a
        {
            if(i && dp[i][j - 1][k - 1] <= dp[i - 1][j][k]) //filter a
            {
                scs.push_back(X[i - 1]);
                --i, --index;
                continue;
            }
            else{ //filter bc
                scs.push_back(Y[j - 1]);
                --k,--j, --index;
                continue;
            }
        }
      
        else if(i &&(!j || dp[i - 1][j][k] <= dp[i][j - 1][k]))
        {
            if(!k || dp[i - 1][j][k] <= dp[i][j][k - 1]) //filter a
            {
                scs.push_back(X[i - 1]);
                --i, --index;
                continue;
            }
        }
        if(j &&(!i || dp[i][j - 1][k] <= dp[i - 1][j][k]))
        {
            if(!k || dp[i][j - 1][k] <= dp[i][j][k - 1]) //filter b
            {
                scs.push_back(Y[j - 1]);
                --j, --index;
                continue;
            }
        }
        if(k && (!i || dp[i][j][k - 1] <= dp[i - 1][j][k]))
        {
            if (!j || dp[i][j][k - 1] <= dp[i][j - 1][k]) //filter b
            {
                scs.push_back(Z[k - 1]);
                --k, --index;
                continue;
            }
        }
    } 
  
  
    // reverse the string and return it 
    reverse(scs.begin(), scs.end()); 
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
                            //printf("WORDS: %s, %s, %s\n", x.c_str(), y.c_str(), z.c_str());
                            scs = printShortestSuperSeq(x, y, z);
                            printf("%s\n", scs.c_str());
                            printf("takes %d cycles\n", scs.size());
                            if (max_cyc < scs.size())
                            {
                                max_cyc = scs.size();
                                max_word = scs;
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