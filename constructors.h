#pragma once
#include "Commit.h"
#include "File.h"
#include "FollowerNode.h"
using namespace std;
Commit::Commit(string id, string message, time_t timestamp)
    : id(id), message(message), timestamp(timestamp), next(nullptr) {}
File::File(string name) : name(name), next(nullptr) {}

FollowerNode::FollowerNode(const string& name) : name(name), next(nullptr) {}
