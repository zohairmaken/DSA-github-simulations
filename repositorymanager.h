#ifndef REPOSITORYMANAGER_H
#define REPOSITORYMANAGER_H

#include <string>
#include <fstream>

using namespace std;

struct file {
    string name;
    file* next;
    file(string n) : name(n), next(nullptr) {}
};

struct commit {
    string id;
    string message;
    time_t timestamp;
    commit* next;
    commit(string i, string m, time_t t) : id(i), message(m), timestamp(t), next(nullptr) {}
};

struct repositorynode {
    string owner;
    string name;
    bool isprivate;
    int forkCount;
    file* filehead;
    commit* commithead;
    repositorynode* left;
    repositorynode* right;
    repositorynode(string o, string n, bool p) : owner(o), name(n), isprivate(p), forkCount(0), filehead(nullptr), commithead(nullptr), left(nullptr), right(nullptr) {}
};

class repositorymanager {
private:
    repositorynode* root;
    repositorynode* insert(repositorynode* node, string owner, string name, bool isprivate);
    repositorynode* search(repositorynode* node, string name);
    string generateCommitID();
    void copyfiles(repositorynode* originalnode, repositorynode* newnode);
    void copycommits(repositorynode* originalnode, repositorynode* newnode);
    void savetocsv(repositorynode* node, ofstream& file);
public:
    repositorymanager();
    repositorynode* createrepository(string owner, string name, bool isprivate);
    void forkrepo(string forkOwner, string originalName);
    void addcommit(repositorynode* node, string commit_message);
    void savecommits(string repo_name);
    void loadcommits(string repo_name);
    void addfile(repositorynode* node, string filename);
    void deletefile(repositorynode* node, string filename);
    void displayRepositoryStats(repositorynode* node);
    void displayRepositoryStats(string repoName);
    void loadfromcsv(string filename);
    void savetocsv(string filename);
    void deleterepository(string name);
    void displayMenu();
    repositorynode* deleterepository(repositorynode* node, string name);
    repositorynode* minvaluenode(repositorynode* node);

    void run();
};

#endif // REPOSITORYMANAGER_H

