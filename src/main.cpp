#include <raylib.h>

#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <stack>

using namespace std;

constexpr float WIDTH = 1280, HEIGHT = 720;
constexpr float VECSIZE = 1024;
constexpr float RECW = WIDTH / VECSIZE, RECH = HEIGHT / VECSIZE;

bool draw(const vector<int>& nums) {
    if (WindowShouldClose()) {
        CloseWindow();
        return true;
    }
    BeginDrawing();
    ClearBackground(BLACK);
    for (int i = 0; i < VECSIZE; i++) {
        float curHeight = nums[i] * RECH;
        DrawRectangleRec({ i * RECW, HEIGHT - curHeight, RECW, curHeight }, { unsigned char(255 * pow(cos(nums[i] * (PI / (VECSIZE * 1.25))), 2)), unsigned char(255 * pow(cos(nums[i] * (PI / (VECSIZE * 1.25)) + (2 * PI / 3)), 2)), unsigned char(255 * pow(cos(nums[i] * (PI / (VECSIZE * 1.25)) + (PI / 3)), 2)), 255 });
    }
    EndDrawing();
    return false;
}

void bogoSort(vector<int>& nums) {
    vector<int> sorted = nums;
    sort(sorted.begin(), sorted.end());
    while (nums != sorted) {
        if (draw(nums)) return;
        random_shuffle(nums.begin(), nums.end());
    }
    while (!draw(nums));
}

void bubbleSort(vector<int>& nums) {
    for(int i = 0; i < VECSIZE; i++) {
        for (int j = 0; j < VECSIZE - i - 1; j++) {
            if (draw(nums)) return;
            if (nums[j] > nums[j + 1]) swap(nums[j], nums[j + 1]);
        }
    }
    while (!draw(nums));
}

void gravitySort(vector<int>& nums) {
    const int x = VECSIZE, y = *max_element(nums.begin(), nums.end()) + 1;
    vector<vector<bool>> abacus(x, vector<bool>(y, false));
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < nums[i]; j++) abacus[i][j] = true;
    }

    bool changed;
    do {
        changed = false;
        if (draw(nums)) return;
        for (int i = 0; i < x; i++) {
            int j;
            for (j = 0; j < y && abacus[i][j]; j++);
            nums[i] = j;
        }
        for (int i = x - 1; i > 0; i--) {
            for (int j = 0; j < y; j++) {
                if (!abacus[i][j] && abacus[i - 1][j]) {
                    swap(abacus[i][j], abacus[i - 1][j]);
                    changed = true;
                }
            }
        }
    } while (changed);
    while (!draw(nums));
}

void insertionSort(vector<int>& nums) {
    for (int i = 1; i < VECSIZE; i++) {
        for (int j = i; j > 0 && nums[j] < nums[j - 1]; j--) {
            if (draw(nums)) return;
            swap(nums[j], nums[j - 1]);
        }
    }
    while (!draw(nums));
}

void heapSort(vector<int>& nums) {
    static const auto cmp = [&](int a, int b) {
        if (draw(nums)) throw "interrupted";
        return a < b;
    };
    try {
        make_heap(nums.begin(), nums.end(), cmp);
        sort_heap(nums.begin(), nums.end(), cmp);
    }
    catch (...) {
        return;
    }
    while (!draw(nums));
}

void mergeSort(vector<int>& nums) {
    queue<pair<vector<int>::iterator, vector<int>::iterator>> itQueue;
    for(auto it = nums.begin(); it != nums.end(); it++) itQueue.push(make_pair(it, it + 1));
    while (itQueue.size() > 1) {
        auto first = itQueue.front();
        itQueue.pop();
        if (first.first > itQueue.front().first) {
            itQueue.push(first);
            continue;
        }
        auto second = itQueue.front();
        itQueue.pop();
        int curSize = second.second - first.first;
        int* copy = new int[curSize];
        auto it1 = first.first, it2 = second.first;
        for (int i = 0; i < curSize; i++) {
            if (it1 != first.second && it2 != second.second) {
                copy[i] = *it1 > *it2 ? *(it2++) : *(it1++);
            }
            else if (it2 != second.second) {
                copy[i] = *(it2++);
            }
            else {
                copy[i] = *(it1++);
            }
        }
        for (int i = 0; i < curSize; i++) {
            if (draw(nums)) return;
            first.first[i] = copy[i];
        }
        delete[] copy;
        itQueue.push(make_pair(first.first, second.second));
    }
    while (!draw(nums));
}

void shellSort(vector<int>& nums) {
    for (int step = VECSIZE / 2; step > 0; step /= 2) {
        for (int i = step; i < VECSIZE; i++) {
            for (int j = i; j > (step - 1) && nums[j] < nums[j - step]; j-= step) {
                if (draw(nums)) return;
                swap(nums[j], nums[j - step]);
            }
        }
    }
    while (!draw(nums));
}

void stableSort(vector<int>& nums) {
    static const auto cmp = [&](int a, int b) {
        if (draw(nums)) throw "interrupted";
        return a < b;
    };
    try {
        stable_sort(nums.begin(), nums.end(), cmp);
    }
    catch (...) {
        return;
    }
    while (!draw(nums));
}

void stdSort(vector<int>& nums) {
    static const auto cmp = [&](int a, int b) {
        if (draw(nums)) throw "interrupted";
        return a < b;
    };
    try {
        sort(nums.begin(), nums.end(), cmp);
    }
    catch (...) {
        return;
    }
    while (!draw(nums));
}

void quickSort(vector<int>& nums) {
    stack<pair<vector<int>::iterator, vector<int>::iterator>> itStack;
    itStack.push(make_pair(nums.begin(), nums.end()));
    while (itStack.size() > 0) {
        auto begin = itStack.top().first;
        auto end = itStack.top().second;
        itStack.pop();
        auto midpoint = begin;
        for (auto it = begin; it != end - 1; it++) {
            if (draw(nums)) return;
            if (*(end - 1) > *it) {
                swap(*midpoint, *it);
                midpoint++;
            }
        }
        swap(*midpoint, *(end - 1));
        if (end - midpoint > 1) itStack.push(make_pair(midpoint, end));
        if (midpoint - begin > 1) itStack.push(make_pair(begin, midpoint));
    }
    while (!draw(nums));
}

void radixSort(vector<int>& nums) {
    for (int iterations = 0; iterations < sizeof(int); iterations++) {
        vector<int> aux = nums;
        vector<int> count(256, 0);
        auto getBits = [iterations](int num) -> unsigned char {
            int mask = 0xff << (8 * iterations);
            return (num & mask) >> (8 * iterations);
        };
        for (int i = 0; i < VECSIZE; i++) { 
            count[getBits(aux[i])]++;
        }
        for (int i = 1; i < 256; i++) {
            count[i] += count[i - 1];
        }
        for (int i = VECSIZE - 1; i >= 0; i--) {
            if (draw(nums)) return;
            nums[--count[getBits(aux[i])]] = aux[i];
        }
    }
    while (!draw(nums));
}

void sort(vector<int>& nums, int framerate, const char* title, void (*sorter)(vector<int>&)) {
   
    random_shuffle(nums.begin(), nums.end());

    InitWindow(WIDTH, HEIGHT, TextFormat("%s (%i fps)", title, framerate));
    SetTargetFPS(framerate);

    sorter(nums);
}

int main() {
    srand(unsigned(time(NULL)));
    SetTraceLogLevel(INT_MAX); // don't write raylib logs to stdout

    vector<int> nums(VECSIZE);
    for (int i = 0; i < VECSIZE; i++) nums[i] = i + 1;

    const vector<string> commands = { 
        "bogo - start the bogo sort visualizer",
        "bubble - start the bubble sort visualizer",
        "gravity - start the gravity sort visualizer",
        "insertion - start the insertion sort visualizer",
        "heap - start the heap sort visualizer",
        "merge - start the merge sort visualizer",
        "radix - start the radix sort visualizer",
        "shell - start the shell sort visualizer",
        "stable - start the std::stable_sort visualizer",
        "std - start the std::sort visualizer",
        "quick - start the quick sort visualizer",
        "exit - exit the program",
        "quit - alternative to exit"
    };

    string command;

    cout << "Welcome to the sorting algorithm visualizer. Type help for a list of commands.\n> ";
    while(getline(cin, command)) {
        if (command == "help") for (auto i : commands) cout << i << endl;
        else if (command == "bogo") sort(nums, 1000, "Bogo Sort", bogoSort);
        else if (command == "bubble") sort(nums, 1000, "Bubble Sort", bubbleSort);
        else if (command == "gravity") sort(nums, 100, "Gravity Sort", gravitySort);
        else if (command == "insertion") sort(nums, 1000, "Insertion Sort", insertionSort);
        else if (command == "heap") sort(nums, 500, "Heap Sort", heapSort);
        else if (command == "merge") sort(nums, 750, "Merge Sort", mergeSort);
        else if (command == "radix") sort(nums, 250, "Radix Sort", radixSort);
        else if (command == "shell") sort(nums, 625, "Shell Sort", shellSort);
        else if (command == "stable") sort(nums, 750, "std::stable_sort", stableSort);
        else if (command == "std") sort(nums, 750, "std::sort", stdSort);
        else if (command == "quick") sort(nums, 500, "Quick Sort", quickSort);
        else if (command == "quit" || command == "exit") return 0;
        else cout << "Invalid command." << endl;
        cout << "> ";
    }
}