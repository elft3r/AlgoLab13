#include <iostream>
#include <vector>

using namespace std;

void decToBin(int dec, vector<int> &bin, int binLen) {
    int i=binLen-1;
    while(dec > 1) {
        if(dec%2==0) {
            bin[i]=0;
        } else {
            bin[i]=1;
        }
        dec=dec/2;
        i--;
    }
    bin[i] = (dec==0) ? 0 : 1;
    i--;
    while(i >=0) {
        bin[i] = 0;
        i--;
    }
}

void printVector(vector<int> &v) {
    for(vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}

int computeSteps(vector<int> &bulb, vector<int> &resultPattern, int numBulbs, int patternLength) {
    int numPatterns = numBulbs/patternLength;
    int curReplacesToPattern = 0;
    int curReplacesToOpposite = 0;
    //go over every pattern
    for(int i=0; i<numPatterns; i++) {
        int replacesToPattern=0;
        int replacesToOpposite=0;
        for(int j=0; j<patternLength; j++) {
            int iBulb = i*patternLength+j;
            if(bulb[iBulb] != resultPattern[j]) {
                replacesToPattern++;
                // cout << "bulb " << iBulb << " is like opposite" << endl;
            } else {
                replacesToOpposite++;
                // cout << "bulb " << iBulb << " is like pattern" << endl;
            }
        }
        // cout << "pattern " << i << " needs " << replacesToPattern << " and " << replacesToOpposite << endl;
        int bothReplacesToPattern = curReplacesToPattern+replacesToPattern;
        int bothReplacesToOpposite = curReplacesToOpposite+replacesToOpposite;
        curReplacesToPattern = (bothReplacesToPattern < bothReplacesToOpposite + 1)
                                ? bothReplacesToPattern : bothReplacesToOpposite+1;
        curReplacesToOpposite = (bothReplacesToPattern+1 < bothReplacesToOpposite)
                                ? bothReplacesToPattern+1 : bothReplacesToOpposite;
    }

    return curReplacesToPattern;
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;

    for(int i=0; i < testCases; i++) {
        int numBulbs, patternLength, x;
        cin >> numBulbs >> patternLength >> x;
        
        vector<int> bulb(numBulbs);
        for(int j=0; j<numBulbs; j++) {
            int state;
            cin >> state;
            bulb[j] = state;
        }
        // cout << "bulbs: " ;
        // printVector(bulb);

        vector<int> binX(patternLength);
        decToBin(x, binX, patternLength);
        // cout << "result pattern: " ;
        // printVector(binX);
        cout << computeSteps(bulb, binX, numBulbs, patternLength) <<endl;

    }
}

