#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
using namespace std;
struct SIGNIN {
    string username;
    string password;
};

struct Node {
    SIGNIN signin;
    Node* next;

    Node() {
        next = nullptr;
    }
};

class User {
    Node** Usertable;
    int capacity;

public:
    User();
    ~User();
    int hashfunc(int& id);
    int HashFunc(const string& name);
    void Userregister(const string& uname, const string& password);
    bool Login(const string& uname, const string& password);
    void viewProfile(const string& username);
    void delayProgram(int time);
};
