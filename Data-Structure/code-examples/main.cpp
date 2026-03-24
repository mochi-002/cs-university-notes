// g++ -o mochi mochi.cpp && ./mochi
// Created by HP on Sunday 22-03-2026 15:23:16
// ----------------> practice makes perfect <----------------
#include <bits/stdc++.h>
#include <iomanip>
using namespace std;
#define ll long long

stack<int> promote(stack<int> ref, int target) {
    vector<int> temp;
    while (!ref.empty()) {
        int val = ref.top();
        ref.pop();
        temp.push_back(val);
        if (val == target) {
            if(temp.size() > 1) {
                temp[temp.size() - 1] = val;
                temp[temp.size() - 2] =  temp[temp.size() - 2];
            }
            else {
                temp[temp.size() - 1] = -INT_MAX;
            }
        }
    }
    stack<int> ans;
    for(auto n : temp) {
        if(n != -INT_MAX) {
            ans.push(n);
        }
    }
    return ans;
}

signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int x_x = 1;
    // cin >> x_x;
    for (int testNum = 1; testNum <= x_x; testNum++)
    {
        stack<int> start;
        start.push(10);
        start.push(20);
        start.push(30);
        start.push(40);
        stack<int> end = promote(start, 30);
        while (!end.empty()) {
            cout << end.top() << ' ';
            end.pop();
        }
    }
    return 0;
}