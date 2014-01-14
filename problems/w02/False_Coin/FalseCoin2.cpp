#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;

bool *initializeArray(bool init, int n) {
    bool *array = new bool[n];
    for(int i=0; i < n; i++) {
        array[i] = init;
    }
    return array;
}

string falseCoin(bool suspicious[], int numCoins) {
    int fCoin = -1;
    for(int i=0; i<numCoins; i++) {
        if(suspicious[i]) {
            if(fCoin!=-1) {
                return "0";
            } else {
                fCoin = i+1;
            }
        }
    }
    return static_cast<ostringstream*>( &(ostringstream() << fCoin) )->str();
}

int main() {
    cin.sync_with_stdio(false);
    int testCases;
    cin >> testCases;

    for(int i=0; i < testCases; i++) {
        // char newline;
        // cin >> newline;

        int numCoins, numWeighings;
        cin >> numCoins >> numWeighings;
        bool *suspicious = initializeArray(true, numCoins);

        for(int j=0; j < numWeighings; j++) {
            int amountInPan;
            cin >> amountInPan;
            bool *weighingCoins = initializeArray(false, numCoins);
            // read currently weighted coins
            for(int k=0; k < 2*amountInPan; k++) {
                int currentCoin;
                cin >> currentCoin;
                weighingCoins[currentCoin-1] = true;
            }

            char outcome;
            cin >> outcome;
            if(outcome == '=') {
                //every weighing coin true, is false in coins
                for(int k=0; k<numCoins; k++) {
                    if(weighingCoins[k]) {
                        suspicious[k] = false;
                    }
                }
            } else {            
                //set all others to false
                for(int k=0; k<numCoins; k++) {
                    if(!weighingCoins[k]) {
                        suspicious[k] = false;
                    }
                }
            }
            delete weighingCoins;
        }

        cout << falseCoin(suspicious, numCoins) << endl;
        delete suspicious;
    }
}
