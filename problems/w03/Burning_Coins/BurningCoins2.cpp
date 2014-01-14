#include <iostream>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;

    for(int i=0; i < testCases; i++) {
        int numCoins;
        cin >> numCoins;
        int minOpt[numCoins][numCoins];
        for(int j=0; j < numCoins; j++) {
            cin >> minOpt[j][j];
        }
        for(int j=0; j < numCoins-1; j++) {
            minOpt[j][j+1] = max(minOpt[j][j], minOpt[j+1][j+1]);
            // cout << j << " " << j+1 << " " << minOpt[j][j+1] << endl;
        }
        for(int m=2; m < numCoins; m++) {
            for(int j=0; j < numCoins-m; j++) {
                int k= j+m;

                minOpt[j][k] 
                    = max(minOpt[j][j] + ((minOpt[j+2][k] > minOpt[j+1][k-1]) ? minOpt[j+1][k-1] : minOpt[j+2][k]),
                            minOpt[k][k] + ((minOpt[j+1][k-1] > minOpt[j][k-2]) ? minOpt[j][k-2] : minOpt[j+1][k-1]));
                // cout << j << " " << k << " " << minOpt[j][k] << endl;

            }
        }
        cout << minOpt[0][numCoins-1] << endl;
    }
}

