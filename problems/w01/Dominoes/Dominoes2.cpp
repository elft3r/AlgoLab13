#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <algorithm>

using namespace std;

int main() {
    std::ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;

    for(int k=0; k < testCases; k++) {
        int n;
        cin >> n;
        int numFall = 0;

        int h0; //height of the first dominoe which is surely falling
        cin >> h0;
        int accH = h0; //accH is the currently highest falling dominoe

        for(numFall=1; numFall < n; numFall++) {
            int h;
            cin >> h;
            accH= accH-1;
            if(accH > 0) {
                accH = max(accH, h);
            } else {
                break; //this dominoe does not fall
            }
        }
        //read rest of the file
        for(int r=numFall+1; r<n;r++) {
            int h;
            cin >> h;
        }

        cout << numFall << endl;
    }
}

