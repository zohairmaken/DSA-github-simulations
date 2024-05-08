#pragma once
#pragma once
#include <string>
using namespace std;

class FollowerNode {
public:
    string name;
    FollowerNode* next;
    FollowerNode(const string& name);
};
