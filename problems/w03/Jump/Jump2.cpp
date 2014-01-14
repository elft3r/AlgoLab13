#include <iostream>
#include <limits>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;
    for(int testCase=0; testCase < testCases; testCase++) {
        int numCells, maxJump;
        cin >> numCells >> maxJump;

        int vCell[numCells];
        long long minCost[numCells];

        minCost[0] = 0;
        cin >> vCell[0];
        int globalBestPos = 0;

        for(int curCell=1; curCell < numCells; curCell++) {
            if(globalBestPos < curCell-maxJump) {
                // find minCost
                long long min = numeric_limits<long long>::max(); 
                long long pos = curCell-maxJump;
                for(int k=curCell-maxJump; k < curCell; k++) {
                        long long curMin = minCost[k];
                        if(curMin < min) {
                            min = curMin;
                            pos = k;
                        }
                    }
                globalBestPos = pos;

            }
            cin >> vCell[curCell];
            long long curMinPos = minCost[globalBestPos] + vCell[curCell];
            minCost[curCell] = curMinPos;
            if(minCost[globalBestPos] >= curMinPos) {
                globalBestPos = curCell;
            }

            // cout << "Min on pos " << i << " is " << minCost[curCell] << endl;
        }
   
        cout << minCost[numCells-1] << endl;
    }
    return 0;
}

