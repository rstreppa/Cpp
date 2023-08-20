// RunningMedian.cpp : This file contains the 'main' function. Program execution begins and ends there.

/*  
    Compute the running median of a stream of numbers with a window of size K:
    when the size of the numbers being considered exceeds K, drop the oldest observation 
    and proceed until you have consumed the whole stream on numbers
    computeRunningMedian must return a vector of doubles representing the median of the stream at each point in time
    computeRunningMedian has the following signature: 
    vector<double> computeRunningMedian(vector<int> nums, int K)
 */


/*
For this problem, the most appropriate data structure to maintain a running median of a sliding window 
is a combination of two balanced binary search trees (BST).
One BST will contain the smaller half of the numbers and the other BST will contain the larger half. 
The median can then be quickly computed from the largest number in the first BST and the smallest number 
in the second BST. A common implementation of this idea uses two priority queues (heaps): 
a max heap for the smaller half of the numbers and a min heap for the larger half.

Here are a few references that explain this idea in more detail:

very useful, keep
https://www.hackerearth.com/practice/notes/heaps-and-priority-queues/

add this on priority queue: nice it has Python Java C++ implementation
https://www.programiz.com/dsa/priority-queue#:~:text=A%20priority%20queue%20is%20a,their%20order%20in%20the%20queue.

very useful for a practical implementations starting from index 0
https://www.geeksforgeeks.org/priority-queue-using-binary-heap/

very clear theoretically, keep
https://www.cs.cmu.edu/~rdriley/121/notes/heaps.html

very clear stack overflow explanation, keep, also nice C++ implementation
https://stackoverflow.com/questions/18993269/difference-between-priority-queue-and-a-heap

As new numbers come in from the stream, and older numbers are removed
(because they are now outside of the window of size K), we need to efficiently add and remove numbers
from these heaps and adjust the heaps accordingly.

Here's a solution using C++20:
*/


#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class RunningMedian_ideal {
private:
    priority_queue<int> maxHeap; // For the smaller half
    priority_queue<int, vector<int>, greater<int>> minHeap; // For the larger half

    void balanceHeaps() {
        while (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }

        while (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

public:
    void insert(int num) {
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        }
        else {
            minHeap.push(num);
        }
        balanceHeaps();
    }

    void remove(int num) {
        if (num <= maxHeap.top()) {
            maxHeap.remove(num); // Need a custom priority_queue or BST implementation to support removal
        }
        else {
            minHeap.remove(num); // Need a custom priority_queue or BST implementation to support removal
        }
        balanceHeaps();
    }

    double getMedian() const {
        if (maxHeap.size() == minHeap.size()) {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
        else {
            return maxHeap.top();
        }
    }
};


/*
    Note: The standard priority_queue does not support the remove operation. 
    Therefore, you would need a custom implementation that supports removal 
    (or use a data structure like a balanced BST which allows efficient insertion, deletion, and finding the min/max). 
    The above code assumes such an implementation and highlights the logic for maintaining the running median.

    Here's a solution using C++20 and the multiset data structure:
    For simplicity, I'll implement a custom priority queue using a std::multiset. 
    std::multiset is a type of BST (a self-balancing binary search tree, actually) 
    that allows for duplicate values and supports efficient insertion, removal, and finding of elements. 
    This will make our code relatively concise:
*/

class RunningMedian {
private:
    multiset<int> lowerHalf; // For the smaller half
    multiset<int, greater<int>> upperHalf; // For the larger half

    void balanceHeaps() {
        while (upperHalf.size() > lowerHalf.size() + 1) {
            lowerHalf.insert(*upperHalf.begin());
            upperHalf.erase(upperHalf.begin());
        }

        while (lowerHalf.size() > upperHalf.size()) {
            upperHalf.insert(*lowerHalf.begin());
            lowerHalf.erase(lowerHalf.begin());
        }
    }

public:
    void insert(int num) {
        if (upperHalf.empty() || num <= *upperHalf.begin()) {
            upperHalf.insert(num);
        }
        else {
            lowerHalf.insert(num);
        }
        balanceHeaps();
    }

    void remove(int num) {
        if (upperHalf.find(num) != upperHalf.end()) {
            upperHalf.erase(upperHalf.find(num));
        }
        else {
            lowerHalf.erase(lowerHalf.find(num));
        }
        balanceHeaps();
    }

    double getMedian() const {
        if (upperHalf.size() == lowerHalf.size()) {
            return (*upperHalf.begin() + *lowerHalf.begin()) / 2.0;
        }
        else {
            return *upperHalf.begin();
        }
    }
};

vector<double> computeRunningMedian(const vector<int>& stream, int K) {
    RunningMedian runningMedian;
    vector<double> medians;

    for (int i = 0; i < stream.size(); i++) {
        runningMedian.insert(stream[i]);

        if (i >= K) {
            runningMedian.remove(stream[i - K]);
        }

        if (i >= K - 1) {
            medians.push_back(runningMedian.getMedian());
        }
    }

    return medians;
}


int main()
{
	vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int K = 3;
	vector<double> result = computeRunningMedian(nums, K);
	for (int i = 0; i < result.size(); i++) {
		cout << result[i] << " ";
	}
	cout << endl;
}

