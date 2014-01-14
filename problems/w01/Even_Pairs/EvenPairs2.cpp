#include <iostream>
#include <vector>
#include <utility>

using namespace std;

bool even(int x) {
    return(x%2==0);
}

int main() {
    int n;

    cin >> n;

    int numPairs = 0;
    int Sum[n]; // sum of values including i-th value
    int E[n]; //number of even pairs for indexes smaller than i
    int O[n]; //number of odd pairs for indexes smaller than i


    //first value needs special procedure
    int v;
    cin >> v;
    //initialize accumulators
    Sum[0] = v;
    E[0] = 1; //must be initialized with one!
    O[0] = 0;
    numPairs = (even(Sum[0]) ? E[0] : O[0]);

    for(int i = 1; i < n; i++) {

        cin >> v;
        Sum[i] = Sum[i-1] + v;

        E[i] = E[i-1];
        O[i] = O[i-1];
        if(even(Sum[i-1])) {
            E[i]+=1;
        } else {
            O[i]+=1;
        }

        numPairs+= (even(Sum[i]) ? E[i] : O[i]);
            
        // DEBUG:
        // cout << Sum[i] << " " << E[i] << " " << O[i] << " " << numPairs << endl;
    }

    cout << numPairs << endl;
    return 0;
}
