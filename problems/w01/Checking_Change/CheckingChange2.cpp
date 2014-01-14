#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <stdio.h>
#include <string.h>

using namespace std;

/*
 * c[] coin denominations
 * ci |c[]|
 * n change
 */
int *change(int c[], int ci, int n) {
    int *C = new int[n+1];
    C[0] = 0;
    for(int p=1; p <= n; p++) {
        int min = numeric_limits<int>::max() - 1;
        for(int i=0; i < ci; i++) {
            if(c[i] <= p) {
                if (1 + C[p-c[i]] < min) {
                    min = 1 + C[p-c[i]];
                }
            }
        }
        C[p] = min;
    }
    return C;
} 

int changeRec(int c[], int ci, int n) {
    if(n==0) {
        return 0;
    } else {
        int min = numeric_limits<int>::max() - 1;
        for(int i=ci-1; i >= 0; i--) {
            if(c[i] <= n) {
                int tryChange = changeRec(c, ci, n-c[i]);
                if (1 + tryChange < min) {
                    min = 1 + tryChange;
                }
            }
        }
        return min;
    }
}


//C[n] initialised with -1
int changeFast(vector<int> &C, int c[], int ci, int n) {
    if(n==0) {
        C[n] = 0;
    } else if(C[n] == -1) {
        int min = numeric_limits<int>::max() - 1;
        for(int i=ci-1; i >= 0; i--) {
            if(c[i] <= n) {
                int tryChange = changeFast(C, c, ci, n-c[i]);
                if (1 + tryChange < min) {
                    min = 1 + tryChange;
                }
            }
        }
        C[n] = min;
    }
    return C[n];
}

void reverse(int *word, int len)
{
    int temp;
    for (int i=0;i<len/2;i++)
    {
            temp=word[i];
            word[i]=word[len-i-1];
            word[len-i-1]=temp;
    }
}

int *changeFaster(int c[],int n, const int A, int **C) {
    reverse(c, n);
    for(int j=0; j<A; j++) {
        C[n-1][j] =j;
    }
    for(int i=n-2; i>=0; i--) {
        for(int j=0; j<A; j++) {
            if(c[i] > j || C[i+1][j] < 1 + C[i][j-c[i]]) {
                C[i][j] = C[i+1][j];
            } else {
                C[i][j] = 1 + C[i][j-c[i]];
            }
        }
    }
}


int count( int S[], int m, int n )
{
    // table[i] will be storing the number of solutions for
    // value i. We need n+1 rows as the table is consturcted
    // in bottom up manner using the base case (n = 0)
    int table[n+1];
 
    // Initialize all table values as 0
    memset(table, 0, sizeof(table));
 
    // Base case (If given value is 0)
    table[0] = 0;
 
    // Pick all coins one by one and update the table[] values
    // after the index greater than or equal to the value of the
    // picked coin
    for(int i=0; i<m; i++)
        for(int j=S[i]; j<=n; j++)
            table[j] += table[j-S[i]];
 
    return table[n];
}

// fill vector up such that all indices n - former C.size()  are -1
void fillVector(vector<int> &C, int n) {
    for(int i=C.size(); i < n+1; i++) {
        C.push_back(-1);
    }
}

void printVector(vector<int> &C) {
    for(int i=0; i < C.size(); i++) {
        cout << C[i] << " ";
    }
    cout << endl;
}

int main() {
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);
    int testCases;
    cin >> testCases;

    for(int i=0; i < testCases; i++) {
        int ci, mi;
        cin >> ci >> mi;
        int c[ci];
        for(int j=0; j < ci; j++) {
            int coinValue;
            cin >> coinValue;
            c[j] = coinValue;
        }
        vector<int> C;
        for(int k=0; k < mi; k++) {
            int n;
            cin >> n;
            fillVector(C, n);

            changeFast(C, c, ci, n);
            int numCoins = C[n];
            if(numCoins == numeric_limits<int>::max() - 1) {
                cout << "not possible" << endl;
            } else {
                cout << numCoins << endl;
            }
            // delete C;
        }
    }
}

