//#include <bits/stdc++.h> 
#include <iostream>
#include <string>
#include <list>
#include <stdio.h>
using namespace std; 
  
static constexpr int SIZE = 2;
// returns shortest supersequence of X and Y 

/*
    mask : 0xabcdefXX
    a - use filter a
    b - use filter b
    c - use filter c
    d - use filter ab
    e - use filter ac
    f - use filter bc
*/

string printShortestSuperSeq(string X, string Y, string Z, int8_t filter_mask)
{ 
    if (!filter_mask)
    {
        printf("enter mask\n");
        return "ERROR";
    }
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
                        if (filter_mask & 0x20)
                        {
                            dp[i][j][k] = k; //filter c
                            continue;
                        }
                        else
                        {
                            dp[i][j][k] = 5 * SIZE;
                            printf("need filter c\n");
                            return "ERROR";
                        }
                    }
                    if (k == 0) {
                        if (filter_mask & 0x40)
                        {
                            dp[i][j][k] = j; //filter b
                            continue;
                        }
                        else
                        {
                            dp[i][j][k] = 5 * SIZE;
                            printf("need filter b\n");
                            return "ERROR";
                        }
                    }
                    if (Y[SIZE - j] == Z[SIZE - k])
                    {
                        if (filter_mask & 0x04)
                        {
                            dp[i][j][k] = 1 + dp[i][j - 1][k - 1]; //filter bc
                            continue;
                        }
                        if (filter_mask & 0x40 && filter_mask & 0x20)
                        {
                            dp[i][j][k] = 1 + min(dp[i][j - 1][k], dp[i][j][k - 1]); //filter b or c 
                            continue;
                        }
                        else
                        {
                            dp[i][j][k] = 5 * SIZE;
                            printf("need filter b and c\n");
                            return "ERROR";
                        }
                    }

                    if (filter_mask & 0x40 && filter_mask & 0x20)
                    {
                        dp[i][j][k] = 1 + min(dp[i][j - 1][k], dp[i][j][k - 1]); //filter b or c 
                        continue;
                    }
                    else
                    {
                        dp[i][j][k] = 5 * SIZE;
                        printf("need filter b and c\n");
                        return "ERROR";
                    }
                }
                else if (j == 0)
                {
                    if (k == 0)
                    {
                        if(filter_mask & 0x80)
                        {
                            dp[i][j][k] = i; //filter a
                            continue;
                        }
                        else
                        {
                            dp[i][j][k] = 5 * SIZE;
                            printf("need filter a\n");
                            return "ERROR";
                        }
                    }
                    if (X[SIZE - i] == Z[SIZE - k])
                    {
                        if (filter_mask & 0x08)
                        {
                            dp[i][j][k] = 1 + dp[i - 1][j][k - 1]; //filter ac
                            continue;
                        }
                        if (filter_mask & 0x80 && filter_mask & 0x20)
                        {
                            dp[i][j][k] = 1 + min(dp[i - 1][j][k], dp[i][j][k - 1]); //filter a or c
                            continue;
                        }
                        else
                        {
                            dp[i][j][k] = 5 * SIZE;
                            printf("need filter a and c\n");
                            return "ERROR";
                        }
                    }

                    if (filter_mask & 0x80 && filter_mask & 0x20)
                    {
                        dp[i][j][k] = 1 + min(dp[i - 1][j][k], dp[i][j][k - 1]); //filter a or c
                        continue;
                    }
                    else
                    {
                        dp[i][j][k] = 5 * SIZE;
                        printf("need filter a and c\n");
                        return "ERROR";
                    }
                }
                if (k == 0)
                {
                    if (X[SIZE - i] == Y[SIZE - j])
                    {
                        if (filter_mask & 0x10)
                        {
                            dp[i][j][k] = 1 + dp[i - 1][j - 1][k]; //filter ab
                            continue;
                        }
                        if (filter_mask & 0x80 && filter_mask & 0x40)
                        {
                            dp[i][j][k] = 1 + min(dp[i - 1][j][k], dp[i][j - 1][k]); //filter a or b
                            continue;
                        }
                        else
                        {
                            dp[i][j][k] = 5 * SIZE;
                            printf("need filter a and b\n");
                            return "ERROR";
                        }
                    }
                    if (filter_mask & 0x80 && filter_mask & 0x40)
                    {
                        dp[i][j][k] = 1 + min(dp[i - 1][j][k], dp[i][j - 1][k]); //filter a or b
                        continue;
                    }
                    else
                    {
                        dp[i][j][k] = 5 * SIZE;
                        printf("need filter a and b\n");
                        return "ERROR";
                    }
                }

                //step 2: all the words include at least one char
                if (X[SIZE - i] == Y[SIZE - j] && X[SIZE - i] == Z[SIZE - k])
                {
                    dp[i][j][k] = 1 + dp[i - 1][j - 1][k - 1]; //filter abc
                    continue;
                }
                if (X[SIZE - i] == Y[SIZE - j])
                {
                    if (filter_mask & 0x20 && filter_mask & 0x10)
                    {
                        dp[i][j][k] = 1 + min(dp[i - 1][j - 1][k], dp[i][j][k - 1]); //filter ab or c
                        continue;
                    }
                    if (filter_mask & 0x80 && filter_mask & 0x40 && filter_mask & 0x20)
                    {
                        dp[i][j][k] = 1 + min(dp[i - 1][j][k], min(dp[i][j - 1][k], dp[i][j][k - 1])); //filter a or b or c
                        continue;
                    }
                    else
                    {
                        dp[i][j][k] = 5 * SIZE;
                        printf("need filter a and b and c\n");
                        return "ERROR";
                    }
                }
                if (X[SIZE - i] == Z[SIZE - k])
                {
                    if (filter_mask & 0x08 && filter_mask & 0x40)
                    {
                        dp[i][j][k] = 1 + min(dp[i - 1][j][k - 1], dp[i][j - 1][k]); //filter ac or b
                        continue;
                    }
                    if (filter_mask & 0x80 && filter_mask & 0x40 && filter_mask & 0x20)
                    {
                        dp[i][j][k] = 1 + min(dp[i - 1][j][k], min(dp[i][j - 1][k], dp[i][j][k - 1])); //filter a or b or c
                        continue;
                    }
                    else
                    {
                        dp[i][j][k] = 5 * SIZE;
                        printf("need filter a and b and c\n");
                        return "ERROR";
                    }
                }
                if (Y[SIZE - j] == Z[SIZE - k])
                {
                    if (filter_mask & 0x04 && filter_mask & 0x80)
                    {
                        dp[i][j][k] = 1 + min(dp[i][j - 1][k - 1], dp[i - 1][j][k]); //filter bc or a
                        continue;
                    }
                    if (filter_mask & 0x80 && filter_mask & 0x40 && filter_mask & 0x20)
                    {
                        dp[i][j][k] = 1 + min(dp[i - 1][j][k], min(dp[i][j - 1][k], dp[i][j][k - 1])); //filter a or b or c
                        continue;
                    }
                    else
                    {
                        dp[i][j][k] = 5 * SIZE;
                        printf("need filter a and b and c\n");
                        return "ERROR";
                    }
                }
                if (filter_mask & 0x80 && filter_mask & 0x40 && filter_mask & 0x20)
                {
                    dp[i][j][k] = 1 + min(dp[i - 1][j][k], min(dp[i][j - 1][k], dp[i][j][k - 1])); //filter a or b or c
                    continue;
                }
                else
                {
                    dp[i][j][k] = 5 * SIZE;
                    printf("need filter a and b and c\n");
                    return "ERROR";
                }

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
        if(i && j && k && X[SIZE - i] == Y[SIZE - j] && X[SIZE - i] == Z[SIZE - k]) //filter abc
        {
            scs.push_back(X[SIZE - i]);
            --i,--j,--k, --index;
            continue;
        }
        if(i && j && X[SIZE - i] == Y[SIZE - j]) //filter ab or c
        {
            if(k && filter_mask & 0x20 && dp[i - 1][j - 1][k] >= dp[i][j][k - 1]) //filter c
            {
                scs.push_back(Z[SIZE - k]);
                --k, --index;
                continue;
            }
            if(filter_mask & 0x10)
            { //filter ab
                scs.push_back(X[SIZE - i]);
                --i,--j, --index;
                continue;
            }
        }
        if(i && k && X[SIZE - i] == Z[SIZE - k]) //filter ac or b
        {
            if(j && filter_mask & 0x40 && dp[i - 1][j][k - 1] >= dp[i][j - 1][k]) //filter b
            {
                scs.push_back(Y[SIZE - j]);
                --j, --index;
                continue;
            }
            if(filter_mask & 0x08)
            { //filter ac
                scs.push_back(X[SIZE - i]);
                --i,--k, --index;
                continue;
            }
        }
        if(j && k && Z[SIZE - k] == Y[SIZE - j]) //filter bc or a
        {
            if(i && filter_mask & 0x80 && dp[i][j - 1][k - 1] >= dp[i - 1][j][k]) //filter a
            {
                scs.push_back(X[SIZE - i]);
                --i, --index;
                continue;
            }
            if(filter_mask & 0x04)
            { //filter bc
                scs.push_back(Y[SIZE - j]);
                --k,--j, --index;
                continue;
            }
        }
      
        if(i && filter_mask & 0x80 &&  (!j || dp[i - 1][j][k] <= dp[i][j - 1][k]))
        {
            if(!k || dp[i - 1][j][k] <= dp[i][j][k - 1]) //filter a
            {
                scs.push_back(X[SIZE - i]);
                --i, --index;
                continue;
            }
        }
        if(j && filter_mask & 0x40 && (!i || dp[i][j - 1][k] <= dp[i - 1][j][k]))
        {
            if(!k || dp[i][j - 1][k] <= dp[i][j][k - 1]) //filter b
            {
                scs.push_back(Y[SIZE - j]);
                --j, --index;
                continue;
            }
        }
        if(k && filter_mask & 0x20 && (!i || dp[i][j][k - 1] <= dp[i - 1][j][k]))
        {
            if (!j || dp[i][j][k - 1] <= dp[i][j - 1][k]) //filter b
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

                            x = "TG";
                            y = "GT";
                            z = "GA";
                          //  printf("WORDS: %s, %s, %s\n", x.c_str(), y.c_str(), z.c_str());
                            scs = printShortestSuperSeq(x, y, z, 0x80|0x40|0x20);
                            std::cout<<scs.c_str();
                           // printf("takes %d cycles\n", scs.size());
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