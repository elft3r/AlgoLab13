#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

void swap(int* array, int i, int j) {
    int tmp;
    tmp = array[j];
    array[j] = array[i];
    array[i] = tmp;
}

int overpasses;

//! \brief Merges two sorted vectors into one sorted vector
//! \param left A sorted vector of integers
//! \param right A sorted vector of integers
//! \return A sorted vector that is the result of merging two sorted
//! vectors.
vector<int> merge(const vector<int>& left, const vector<int>& right)
{
    // Fill the resultant vector with sorted results from both vectors
    vector<int> result;
    unsigned left_it = 0, right_it = 0;
 
    while(left_it < left.size() && right_it < right.size())
    {
        // If the left value is smaller than the right it goes next
        // into the resultant vector
        if(left[left_it] < right[right_it])
        {
            result.push_back(left[left_it]);
            left_it++;
            overpasses= (overpasses + right_it) % 10000;
        }
        else
        {
            result.push_back(right[right_it]);
            right_it++;
        }
    }
 
    // Push the remaining data from both vectors onto the resultant
    while(left_it < left.size())
    {
        result.push_back(left[left_it]);
        overpasses= (overpasses + right_it) % 10000;
        left_it++;
    }
 
    while(right_it < right.size())
    {
        result.push_back(right[right_it]);
        right_it++;
    }
 
    return result;
}
//! \brief Performs a recursive merge sort on the given vector
//! \param vec The vector to be sorted using the merge sort
//! \return The sorted resultant vector after merge sort is
//! complete.
vector<int> merge_sort(vector<int>& vec)
{
    // Termination condition: List is completely sorted if it
    // only contains a single element.
    if(vec.size() == 1)
    {
        return vec;
    }
 
    // Determine the location of the middle element in the vector
    std::vector<int>::iterator middle = vec.begin() + (vec.size() / 2);
 
    vector<int> left(vec.begin(), middle);
    vector<int> right(middle, vec.end());
 
    // Perform a merge sort on the two smaller vectors
    left = merge_sort(left);
    right = merge_sort(right);
 
    return merge(left, right);
}

/*
 * return number of bubble sort steps necessary
 */
int bubbleSort(int* order, int numRacers) {
    bool swapped = false;
    int numSwaps = 0;
    int n = numRacers;

    do {
        int newN = 0;
        swapped = false;
        for(int i=1; i<n; i++) {
            if(order[i-1] > order[i]) {
                swap(order, i-1, i);
                swapped = true;
                newN = i;
                numSwaps++;
            } 
        }
        n = newN;
    } while(!n==0);

    return numSwaps;
}

// uses bubbleSort to control for correctness of the merge sort overpasses counting
void controlIfCorrect(vector<int> order) {
        int orderArray[order.size()];
        copy(order.begin(), order.end(), orderArray);

        int overpBubble = bubbleSort(orderArray, order.size()); 

        if(overpBubble == overpasses) {
            // cout << "ok: " << overpasses << endl;
        } else {
            cout << "different answer: bubble: " << overpBubble << "merge: " << overpasses << endl;
            cout << "testset: " << endl;
            copy(order.begin(), order.end(), ostream_iterator<int>(cout, " "));
        }
}

int main() {
    cin.sync_with_stdio(false);
    int testCases;
    cin >> testCases;

    for(int i=0; i < testCases; i++) {
        int numRacers;
        cin >> numRacers;

        // int order[numRacers];
        vector<int> order;
        for(int j=0; j < numRacers; j++) {
            int racer;
            cin >> racer;
            order.push_back(racer);
        }

        overpasses = 0;
        merge_sort(order);

        // controlIfCorrect(order);

        cout << overpasses % 10000 << endl;

    }
}

