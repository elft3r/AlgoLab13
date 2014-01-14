#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

int greatestDivisor(int number) {
    int i;
    for (i = 2; i <=sqrt(number); i++) {
        if (number % i == 0) {
            return number/i;
        }
    }
    return 1;
}

int cacheDivisor(vector<int> &graph, int i) {
    if(graph[i] == -1) {
        int gcd = greatestDivisor(i);
        graph[i] = gcd;
        return gcd;
    } else {
        return graph[i];
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int testCases = 0;
    cin >> testCases;

    const int maxN = 10000000;
    vector<int> g(maxN,-1);
    for(int testCase=0; testCase < testCases; testCase++) {
        int n, numPairs;

        cin >> n;

        cin >> numPairs;
        for(int i=0; i<numPairs; i++) {
            int x,y;
            cin >> x >> y;
            int pathLen=0;
            while(true) {
                if(x==y) break;
                if(x>y) {
                    x=cacheDivisor(g, x);
                } else {
                    y=cacheDivisor(g, y);
                }
                ++pathLen;
            }
            cout<<pathLen<<endl;
        }
    }
}
