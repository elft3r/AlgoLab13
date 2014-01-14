#include <iostream>
#include <numeric>
#include <vector>
#include <map>
#include <bitset>

using namespace std;

int optimalPoints(vector<vector<int> > &chips, 
        map<vector<int>,int> &table, vector<int> topChipPosition){
    int numStacks = chips.size();

    map<vector<int>,int>::iterator found = table.find(topChipPosition);
    if(found != table.end()) {
        return found->second;
    }

    //return min over all subsets of possible taking
    int maxPoints = 0;
    int maxSubset = 0;
    //iterate over all subsets of taking coins from the top
    for(int s=1; s < (1<<numStacks); s++) {
        int numChipsTaken = 0;
        //s represents a subset of {0,...,n-1}
        // cout<<"take subset " << std::bitset<5>(s).to_string() << std::endl;
        vector<int> newTopChipPosition(numStacks);
        int color=-1;
        for(int k=0; k<numStacks; k++) {
            //iterate over all elements of s
            newTopChipPosition[k] = topChipPosition[k];
            if((s & (1<<k)) != 0){
                // cout<<"stack " << k << " is in subset" << std::endl;
                if(topChipPosition[k]<0) {
                    // cout<<"stack " << k << " is empty" << std::endl;
                    continue;
                }

                //k is in S
                //ensure that colors are the same
                int stackColor = chips[k][topChipPosition[k]];
                if(color == -1) {
                    color = stackColor;
                    // cout<<"take color " << color << std::endl;
                } else if (color!=stackColor) {
                    // cout<<"stack " << k << "has different color"<< std::endl;
                    continue;
                }
                // cout<<"remove chip from stack " << k << std::endl;
                ++numChipsTaken;
                newTopChipPosition[k] = newTopChipPosition[k]-1; 
            }
        }
        int sumChips=0;
        for(int i=0; i< numStacks; i++) {
            sumChips +=newTopChipPosition[i]+1;
        }

        int newPoints = 0;
        if(numChipsTaken > 1)
            newPoints = (1<<(numChipsTaken-2));

        if(sumChips > 0 && numChipsTaken>0)
            newPoints = newPoints + optimalPoints(chips, table, 
                            newTopChipPosition);

        // cout<<"points: "<<newPoints<<endl;
        if(newPoints > maxPoints) {
            maxPoints = newPoints;
            maxSubset = s;
        }
    }
    // cout<<"max points for this stack: "<< maxPoints<< " using " 
        // << std::bitset<5>(maxSubset).to_string()<< endl;
    table[topChipPosition] = maxPoints;
    return maxPoints;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int testCases;
    cin >> testCases;

    for(int k=0; k < testCases; k++) {
        int numStacks;
        cin >> numStacks;
        
        vector< vector<int> > chips(numStacks);
        vector<int> topChipPosition(numStacks);
        for(int i=0; i<numStacks; i++) {
            int stackHeight;
            cin >> stackHeight;
            chips[i] = vector<int>(stackHeight);
            topChipPosition[i] = stackHeight-1;
        }

        for(int i=0; i<numStacks; i++) {
            for(int j=0; j<chips[i].size(); j++) {
                //the chip at the top of the stack is added last
                int c;
                cin >> c;
                chips[i][j] = c;
            }
        }
        map<vector<int>, int> table;
        cout << optimalPoints(chips,table,topChipPosition) << endl;
    }
}


