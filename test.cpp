#include <iostream>
#include <vector>

using namespace std;


int main(){
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    for (auto it = v.begin(); it != v.end();){
        int n = *it;
        cout << n << endl;
        if (*it == 1){
            cout << "haha " << *it << endl;
            auto tmp = it;
            v.erase(it);

        }else
            it++;
    }
    for (int n : v)
        cout << n << endl;
}
