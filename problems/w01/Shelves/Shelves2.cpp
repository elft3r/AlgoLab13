#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <math.h>

using namespace std;

struct triple {
    int first;
    int second;
    int third;
};

void computeSlow(triple &result, int lTotal, int lM, int lN) {
        for(int uncovered = 0; uncovered < lTotal; uncovered++) {
            int rest = lTotal - uncovered;
            // cout << uncovered << " rest:" << rest << " ";
            for(int numN = rest/lN; numN >= 0; numN--) {
                rest = lTotal - uncovered - numN*lN;
                // cout << " numN: " << numN << " rest:"<< rest << " ";
                for(int numM = rest/lM; numM >= 0; numM--) {
                    rest = lTotal - uncovered -numN*lN - numM*lM;
                    // cout << " numM: " << numM << " rest:"<< rest << " "<< endl;
                    if(rest==0) {
                        result.first=numM;
                        result.second=numN;
                        result.third=uncovered;

                        return;
                    }
                }
            } 
        }
        result.first = 0;
        result.second = 0;
        result.third = lTotal;
}

void computeFast(triple &result, int lTotal, int lM, int lN) {
    int rest = lTotal;
    int a, b = 0;
    for(int bp=0; bp<=lTotal/lN; bp++) {
        int ap = (lTotal-bp*lN)/lM;
        int restp = lTotal - ap*lM - bp*lN;
        if(restp <= rest) {
            rest = restp;
            b = bp;
            a = ap;
        }
    }
    result.first = a;
    result.second = b;
    result.third = rest;
    return;
}

// runtime sqrt(lTotal)
void computeVeryFast(triple &result, int lTotal, int lM, int lN) {
    int rest = lTotal;
    int a, b = 0;
    if(lN < sqrt(lTotal)) { // lTotal is big 
        for(int ap=lN-1; ap >=0; ap--) {
            int bp = (lTotal-ap*lM)/lN;
            int restp = lTotal - ap*lM - bp*lN;
            if(restp <= rest) {
                rest = restp;
                b = bp;
                a = ap;
            }
        }

    } else { //lN > sqrt(lTotal) => loop iterates < lTotal/sqrt(lTotal) < sqrt(lTotal)
        for(int bp=0; bp<=lTotal/lN; bp++) {
            int ap = (lTotal-bp*lN)/lM;
            int restp = lTotal - ap*lM - bp*lN;
            if(restp <= rest) {
                rest = restp;
                b = bp;
                a = ap;
            }
        }
    }
    result.first = a;
    result.second = b;
    result.third = rest;
    return;
}


int main() {
    int testCases;

    cin >> testCases;

    for(int i=0; i < testCases; i++) {
        int lTotal, lM, lN, nM, nN, uncovered;
        cin >> lTotal >> lM >> lN;
        triple result;
        computeVeryFast(result, lTotal, lM, lN);
        cout << result.first << " " << result.second << " " << result.third << endl;
    }

    return 0;
}

