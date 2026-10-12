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
    MinHeap up_heap;      // All requests (we'll filter when needed)
    MaxHeap down_heap;    // All requests (we'll filter when needed)
    int temp_arr[MAXN + 1]; // Temporary array
    
    // Find nearest request above current floor
    int find_nearest_above() {
        int count = up_heap.extract_all(temp_arr);
        int result = -1;
        int min_dist = 1000000000;
        
        for (int i = 1; i <= count; i++) {
            if (temp_arr[i] > current_floor) {
                int dist = temp_arr[i] - current_floor;
                if (dist < min_dist) {
                    min_dist = dist;
                    result = temp_arr[i];
                }
            }
            up_heap.push(temp_arr[i]);
        }
        
        return result;
    }
    
    // Find nearest request below current floor
    int find_nearest_below() {
        int count = down_heap.extract_all(temp_arr);
        int result = -1;
        int min_dist = 1000000000;
        
        for (int i = 1; i <= count; i++) {
            if (temp_arr[i] < current_floor) {
                int dist = current_floor - temp_arr[i];
                if (dist < min_dist) {
                    min_dist = dist;
                    result = temp_arr[i];
                }
            }
            down_heap.push(temp_arr[i]);
        }
        
        return result;
    }
    
    // Remove specific request
    void remove_request(int floor) {
        // Remove from up heap
        int count = up_heap.extract_all(temp_arr);
        for (int i = 1; i <= count; i++) {
            if (temp_arr[i] != floor) {
                up_heap.push(temp_arr[i]);
            }
        }
        
        // Remove from down heap
        count = down_heap.extract_all(temp_arr);
        for (int i = 1; i <= count; i++) {
            if (temp_arr[i] != floor) {
                down_heap.push(temp_arr[i]);
            }
        }
    }
    
public:
    ElevatorSystem() : current_floor(0), direction(1) {}
    
    void add_request(int floor) {
        up_heap.push(floor);
        down_heap.push(floor);
    }
    
    void cancel_request(int floor) {
        remove_request(floor);
    }
    
    void action() {
        if (up_heap.empty() && down_heap.empty()) {
            // No requests, stay still
            return;
        }
        
        if (direction == 1) { // Going up
            int nearest = find_nearest_above();
            if (nearest != -1) {
                current_floor = nearest;
                remove_request(nearest);
            } else {
                // No requests above, reverse direction
                direction = -1;
                int nearest_down = find_nearest_below();
                if (nearest_down != -1) {
                    current_floor = nearest_down;
                    remove_request(nearest_down);
                }
            }
        } else { // Going down
            int nearest = find_nearest_below();
            if (nearest != -1) {
                current_floor = nearest;
                remove_request(nearest);
            } else {
                // No requests below, reverse direction
                direction = 1;
                int nearest_up = find_nearest_above();
                if (nearest_up != -1) {
                    current_floor = nearest_up;
                    remove_request(nearest_up);
                }
            }
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
    cin >> n;
    
    ElevatorSystem elevator;
    
    for (int i = 0; i < n; i++) {
        string command;
        cin >> command;
        
        if (command == "action") {
            elevator.action();
        } else if (command == "add") {
            int floor;
            cin >> floor;
            elevator.add_request(floor);
        } else if (command == "cancel") {
            int floor;
            cin >> floor;
            elevator.cancel_request(floor);
        } else if (command == "locate") {
            cout << elevator.locate() << "\n";
        }
    }
    
    return 0;
}