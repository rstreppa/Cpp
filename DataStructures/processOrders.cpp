// processOrders.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
	Process a stream of orders which we will simplify as a stream of pairs of integers:
	1) the first integer is the order value
	2) the second integer is the time arrival of the order (assuming inception time is 0)
	Different order can arrive at the exact same time
	You need to output "true" if the order is accepted and "false" if the order is rejected
	You can accept a maximum of maxOrders within the time span of maxTime.
	Provide also a test case in main with maxOrders = 5 and maxTime = 6

	To solve this problem, we need to maintain a window that spans maxTime and counts the number of orders 
	within this window. The data structure that comes in handy is a queue, 
	because orders arrive in a sequential manner. 
	The queue will help to easily discard the orders that are now out of the maxTime window.
*/
#include <iostream>
#include <queue>
#include <utility>

using namespace std;

class OrderStream {
private:
    queue<pair<int, int>> orders;
    int maxOrders;
    int maxTime;
    int currentOrders;

public:
    OrderStream(int maxOrders, int maxTime) : maxOrders(maxOrders), maxTime(maxTime), currentOrders(0) {}

    bool processOrder(int value, int timeArrival) {
        // Add the new order first
        orders.push({ value, timeArrival });
        ++currentOrders;

        // Remove orders that are outside of the maxTime window.
        while (!orders.empty()) {
            pair<int, int> oldest = orders.front();
            if (oldest.second <= timeArrival - maxTime) {
                orders.pop();
                --currentOrders;
            }
            else {
                break;
            }
        }

        // Check if we can accept more orders
        if (currentOrders <= maxOrders) {
            return true;
        }
        else {
            // remove the order we just added
            orders.pop();
            --currentOrders;
            return false;
        }
    }
};

int main() {
    OrderStream os(5, 6); // Adjusted maxOrders to 5
    pair<int, int> testOrders[] = {
        {100, 1},
        {150, 1},
        {200, 2},
        {250, 3},
        {300, 3},
        {100, 4},
        {150, 4},
        {200, 5},
        {250, 6},
        {300, 7},
        {400, 8},
    };

    for (const auto& order : testOrders) {
        cout << "Order value: " << order.first << ", Time: " << order.second << " --> ";
        cout << (os.processOrder(order.first, order.second) ? "true" : "false") << endl;
    }

    return 0;
}

