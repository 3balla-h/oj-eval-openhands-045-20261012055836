#include <cstdio>
#include <iostream>
using namespace std;

const int MAXN = 500000;

// Let me implement a much simpler solution using two heaps
class MinHeap {
private:
    int arr[MAXN + 1];
    int cnt;
    
public:
    MinHeap() : cnt(0) {}
    
    void push(int x) {
        arr[++cnt] = x;
        for (int i = cnt, j = cnt >> 1; j; i = j, j >>= 1) {
            if (arr[j] > arr[i]) {
                int tmp = arr[j]; arr[j] = arr[i]; arr[i] = tmp;
            } else break;
        }
    }
    
    int top() {
        return cnt > 0 ? arr[1] : -1;
    }
    
    void pop_top() {
        if (cnt == 0) return;
        arr[1] = arr[cnt--];
        
        for (int i = 1; (i << 1) <= cnt;) {
            int j = i << 1;
            if (j + 1 <= cnt && arr[j] > arr[j + 1]) j++;
            if (arr[i] > arr[j]) {
                int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
                i = j;
            } else break;
        }
    }
    
    bool empty() {
        return cnt == 0;
    }
    
    int size() {
        return cnt;
    }
    
    void clear() {
        cnt = 0;
    }
    
    // Extract all elements to array
    int extract_all(int *output) {
        int count = 0;
        while (!empty()) {
            output[++count] = top();
            pop_top();
        }
        return count;
    }
};

class MaxHeap {
private:
    int arr[MAXN + 1];
    int cnt;
    
public:
    MaxHeap() : cnt(0) {}
    
    void push(int x) {
        arr[++cnt] = x;
        for (int i = cnt, j = cnt >> 1; j; i = j, j >>= 1) {
            if (arr[j] < arr[i]) {
                int tmp = arr[j]; arr[j] = arr[i]; arr[i] = tmp;
            } else break;
        }
    }
    
    int top() {
        return cnt > 0 ? arr[1] : -1;
    }
    
    void pop_top() {
        if (cnt == 0) return;
        arr[1] = arr[cnt--];
        
        for (int i = 1; (i << 1) <= cnt;) {
            int j = i << 1;
            if (j + 1 <= cnt && arr[j] < arr[j + 1]) j++;
            if (arr[i] < arr[j]) {
                int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
                i = j;
            } else break;
        }
    }
    
    bool empty() {
        return cnt == 0;
    }
    
    int size() {
        return cnt;
    }
    
    void clear() {
        cnt = 0;
    }
    
    // Extract all elements to array
    int extract_all(int *output) {
        int count = 0;
        while (!empty()) {
            output[++count] = top();
            pop_top();
        }
        return count;
    }
};

class ElevatorSystem {
private:
    int current_floor;
    int direction; // 1 for up, -1 for down
    int requests[MAXN + 1]; // Store all requests
    int request_count;
    
    // Simple selection sort to find nearest in direction
    int find_nearest_in_direction() {
        int nearest = -1;
        int min_dist = 1000000000;
        
        for (int i = 1; i <= request_count; i++) {
            if (direction == 1 && requests[i] > current_floor) {
                int dist = requests[i] - current_floor;
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest = requests[i];
                }
            } else if (direction == -1 && requests[i] < current_floor) {
                int dist = current_floor - requests[i];
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest = requests[i];
                }
            }
        }
        
        return nearest;
    }
    
    // Remove specific request
    void remove_request(int floor) {
        int pos = -1;
        for (int i = 1; i <= request_count; i++) {
            if (requests[i] == floor) {
                pos = i;
                break;
            }
        }
        
        if (pos != -1) {
            for (int i = pos; i < request_count; i++) {
                requests[i] = requests[i + 1];
            }
            request_count--;
        }
    }
    
public:
    ElevatorSystem() : current_floor(0), direction(1), request_count(0) {}
    
    void add_request(int floor) {
        requests[++request_count] = floor;
    }
    
    void cancel_request(int floor) {
        remove_request(floor);
    }
    
    void action() {
        if (request_count == 0) {
            // No requests, stay still
            return;
        }
        
        // Find nearest request in current direction
        int nearest = find_nearest_in_direction();
        
        if (nearest != -1) {
            // Found a request in current direction
            current_floor = nearest;
            remove_request(nearest);
        } else {
            // No requests in current direction, reverse direction
            direction = -direction;
            
            // Find nearest request in new direction
            nearest = find_nearest_in_direction();
            if (nearest != -1) {
                current_floor = nearest;
                remove_request(nearest);
            }
            // If still no requests, elevator stays at current floor
            // and direction remains reversed
        }
    }
    
    int locate() {
        return current_floor;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    
    ElevatorSystem elevator;
    
    for (int i = 0; i < n; i++) {
        string command;
        if (!(cin >> command)) break;
        
        if (command == "action") {
            elevator.action();
        } else if (command == "add") {
            int floor;
            if (!(cin >> floor)) break;
            elevator.add_request(floor);
        } else if (command == "cancel") {
            int floor;
            if (!(cin >> floor)) break;
            elevator.cancel_request(floor);
        } else if (command == "locate") {
            cout << elevator.locate() << "\n";
        }
    }
    
    return 0;
}