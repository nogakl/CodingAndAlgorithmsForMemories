#include <bits/stdc++.h> 
#include <iostream>
#include <string>
#include <list>
using namespace std; 
  
// returns shortest supersequence of X and Y 
string printShortestSuperSeq(string X, string Y, string Z) 
{ 
    int m = X.length(); 
    int n = Y.length(); 
    int l = Z.length();
  
    // dp[i][j] contains length of shortest supersequence 
    // for X[0..i-1] and Y[0..j-1] and Z[0..k-1]
    int dp[m + 1][n + 1][l + 1]; 
  
    // Fill table in bottom up manner 
    for (int i = 0; i <= m; i++) 
    { 
        for (int j = 0; j <= n; j++) 
        { 
            for (int k = 0 ; k < l ; ++k)
            // Below steps follow recurrence relation 

            //step 1: at least one of those words ended before the call
            if(i == 0){
                if(j == 0)
                    dp[i][j][k] = k; //filter c
                else if (k == 0)
                    dp[i][j][k] = j; //filter b
                else if(Y[j - 1] == Z[k - 1])
                    dp[i][j][k] = 1 + dp[i][j - 1][k - 1]; //filter bc
                else
                     dp[i][j][k] = 1 + min(dp[i][j - 1][k], dp[i][j][k - 1]); //filter b or c 
            }
            else if(j == 0)
            {
                if(k == 0)
                    dp[i][j][k] = i; //filter a
                else if(X[i - 1] == Z[k - 1])
                        dp[i][j][k] = 1 + dp[i - 1][j][k - 1]; //filter ac
                else
                    dp[i][j][k] = 1 + min(dp[i - 1][j][k], dp[i][j][k - 1]); //filter a or c
            }
            else if (k == 0)
            {
                if(X[i - 1] == Y[j - 1])
                        dp[i][j][k] = 1 + dp[i - 1][j - 1][k]; //filter ab
                else
                    dp[i][j][k] = 1 + min(dp[i - 1][j][k], dp[i][j - 1][k]); //filter a or b
            }

            //step 2: all the words include at least one char
            else if(X[i - 1] == Y[j - 1] && X[i - 1] == Z[k - 1])
                dp[i][j][k] = 1 + dp[i - 1][j - 1][k - 1]; //filter abc
            else if(X[i - 1] == Y[j - 1])
                dp[i][j][k] = 1 + min(dp[i - 1][j - 1][k] , dp[i][j][k - 1]); //filter ab or c
            else if(X[i - 1] == Z[k - 1])
                dp[i][j][k] = 1 + min(dp[i - 1][j][k - 1] , dp[i][j - 1][k]); //filter ac or b
            else if(Y[j - 1] == Z[k - 1])
                dp[i][j][k] = 1 + min(dp[i][j - 1][k - 1] , dp[i - 1][j][k]); //filter ac or a
            else
                dp[i][j][k] = 1 + min(dp[i - 1][j][k], min(dp[i][j - 1][k], dp[i][j][k - 1])); //filter a or b or c
        } 
    } 
  
    // Following code is used to print shortest supersequence 
  
    // dp[m][n] stores the length of the shortest supersequence 
    // of X and Y 
    int index = dp[m][n][l]; 
  
    // string to store the shortest supersequence 
    string scs; 
  
    // Start from the bottom right corner and one by one 
    // push characters in output string 
    int i = m, j = n, k = l; 
    while (i > 0 || j > 0 || k > 0) 
    { 
        if(i && j && k && X[i - 1] == Y[j - 1] && X[i - 1] == Z[k - 1]) //filter abc
        {
            scs.push_back(X[i - 1]);
            --i,--j,--k, --index;
        }
        else if(i && j && X[i - 1] == Y[j - 1]) //filter ab or c
        {
            if(k && dp[i - 1][j - 1][k] > dp[i][j][k - 1]) //filter c
            {
                scs.push_back(Z[k - 1]);
                --k, --index;
            }
            else{ //filter ab
                scs.push_back(X[i - 1]);
                --i,--j, --index;
            }
        }
        else if(i && k && X[i - 1] == Z[k - 1]) //filter ac or b
        {
            if(j && dp[i - 1][j][k - 1] > dp[i][j - 1][k]) //filter b
            {
                scs.push_back(Y[j - 1]);
                --j, --index;
            }
            else{ //filter ac
                scs.push_back(X[i - 1]);
                --i,--k, --index;
            }
        }
        else if(j && k && Z[k - 1] == Y[j - 1]) //filter bc or a
        {
            if(dp[i][j - 1][k - 1] < dp[i - 1][j][k]) //filter a
            {
                scs.push_back(X[i - 1]);
                --i, --index;
            }
            else{ //filter bc
                scs.push_back(Y[j - 1]);
                --k,--j, --index;
            }
        }
      
        else if(i &&(!j || dp[i - 1][j][k] < dp[i][j - 1][k]))
        {
            if(!k || dp[i - 1][j][k] < dp[i][j][k - 1]) //filter a
            {
                scs.push_back(X[i - 1]);
                --i, --index;
            }
        }
        else if(j &&(!i || dp[i][j - 1][k] < dp[i - 1][j][k]))
        {
            if(!k || dp[i][j - 1][k] < dp[i][j][k - 1]) //filter b
            {
                scs.push_back(Y[j - 1]);
                --j, --index;
            }
        }
        else
        {
            scs.push_back(Z[k - 1]);
            --k, --index;
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
    for (int i = start; i <= end; i++) 
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
    char chosen[w_size+1]; 
  
    // Call the recursice tfunction 
    CombinationRepetitionUtil(chosen, arr, 0, w_size, n-1, all_words); 
} 

// Driver program to test above function 
int main() 
{ 
    std::list<string> all_words;
    char arr[] = {'A', 'C', 'G', 'T'}; 
    int n = sizeof(arr)/sizeof(arr[0]); 
    int w_size = 10; 
    CombinationRepetition(arr, n, w_size, &all_words);

    for (string str : all_words) {
        printf("%s\n",str.c_str());
    }
   /* 
    string X = "abc"; 
    string Y = "abc"; 
    string Z = "cbc";
  
    printf("%s\n",printShortestSuperSeq(X, Y, Z).c_str()); 
  */
    return 0; 
} 