#include "repositorymanager.h"
#include"Graph.h"
#include"Sighin.h"
#include"constructors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include<vector>
#include <ctime>
#include <cstdlib>

using namespace std;

repositorymanager::repositorymanager() : root(nullptr) {}

repositorynode* repositorymanager::createrepository(string owner, string name, bool isprivate) {
    root = insert(root, owner, name, isprivate);
    return root;
}

repositorynode* repositorymanager::insert(repositorynode* node, string owner, string name, bool isprivate) {
    if (node == nullptr) {
        return new repositorynode(owner, name, isprivate);
    }
    else if (name.compare(node->name) < 0) {
        node->left = insert(node->left, owner, name, isprivate);
    }
    else {
        node->right = insert(node->right, owner, name, isprivate);
    }
    return node;
}

repositorynode* repositorymanager::search(repositorynode* node, string name) {
    if (node == nullptr || node->name == name) {
        return node;
    }
    if (name < node->name) {
        return search(node->left, name);
    }
    else {
        return search(node->right, name);
    }
}

void repositorymanager::forkrepo(string forkowner, string original_name) {
    repositorynode* originalrepo = search(root, original_name);
    if (originalrepo != nullptr) {
        repositorynode* forkedrepo = createrepository(forkowner, originalrepo->name + "_fork", originalrepo->isprivate);
        copyfiles(originalrepo, forkedrepo);
        copycommits(originalrepo, forkedrepo);
        originalrepo->forkCount++;
        cout << "Repository forked successfully." << endl;
    }
    else {
        cout << "Original repository not found." << endl;
    }
}

void repositorymanager::copyfiles(repositorynode* originalnode, repositorynode* newnode) {
    if (originalnode == nullptr || newnode == nullptr) {
        return;
    }
    file* originalfileptr = originalnode->filehead;
    file* newfileptr = nullptr;
    while (originalfileptr != nullptr) {
        file* newfile = new file(originalfileptr->name);
        if (newnode->filehead == nullptr) {
            newnode->filehead = newfile;
            newfileptr = newfile;
        }
        else {
            newfileptr->next = newfile;
            newfileptr = newfileptr->next;
        }
        originalfileptr = originalfileptr->next;
    }
}

void repositorymanager::copycommits(repositorynode* originalnode, repositorynode* newnode) {
    if (originalnode == nullptr || newnode == nullptr) {
        return;
    }
    commit* originalcommitptr = originalnode->commithead;
    commit* newcommitptr = nullptr;
    while (originalcommitptr != nullptr) {
        commit* newcommit = new commit(originalcommitptr->id, originalcommitptr->message, originalcommitptr->timestamp);
        if (newnode->commithead == nullptr) {
            newnode->commithead = newcommit;
            newcommitptr = newcommit;
        }
        else {
            newcommitptr->next = newcommit;
            newcommitptr = newcommitptr->next;
        }
        originalcommitptr = originalcommitptr->next;
    }
}

string repositorymanager::generateCommitID() {
    string id = "";
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const int idLength = 10;
    for (int i = 0; i < idLength; ++i) {
        id += chars[rand() % chars.length()];
    }
    return id;
}

void repositorymanager::addcommit(repositorynode* node, string commit_message) {
    if (node == nullptr) {
        cout << "Repository not found." << endl;
        return;
    }
    string commit_id = generateCommitID();
    time_t timestamp = time(nullptr);
    commit* newcommit = new commit(commit_id, commit_message, timestamp);
    newcommit->next = node->commithead;
    node->commithead = newcommit;
}

void repositorymanager::savecommits(string repo_name) {
    repositorynode* node = search(root, repo_name);
    if (node == nullptr) {
        cerr << "Repository '" << repo_name << "' not found." << endl;
        return;
    }
    string filename = repo_name + "_commits.csv";
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open the file " << filename << " for repository " << repo_name << endl;
        return;
    }
    file << "Commit ID,Message,Timestamp" << endl;
    commit* commitPtr = node->commithead;
    while (commitPtr != nullptr) {
        file << commitPtr->id << "," << commitPtr->message << "," << commitPtr->timestamp << endl;
        commitPtr = commitPtr->next;
    }
    file.close();
    cout << "Commits for repository '" << repo_name << "' saved to CSV successfully." << endl;
}

void repositorymanager::loadcommits(string repo_name) {
    repositorynode* node = search(root, repo_name);
    if (node == nullptr) {
        cerr << "Repository '" << repo_name << "' not found." << endl;
        return;
    }
    string filename = repo_name + "_commits.csv";
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file " << filename << " for repository " << repo_name << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string commit_id, commit_message, timestampStr;
        time_t timestamp;
        getline(iss, commit_id, ',');
        getline(iss, commit_message, ',');
        getline(iss, timestampStr, ',');
        timestamp = stoi(timestampStr);
        commit* newcommit = new commit(commit_id, commit_message, timestamp);
        newcommit->next = node->commithead;
        node->commithead = newcommit;
    }
    file.close();
    cout << "Commits for repository '" << repo_name << "' loaded from CSV successfully." << endl;
}

void repositorymanager::addfile(repositorynode* node, string filename) {
    if (node == nullptr) {
        cout << "Repository not found." << endl;
        return;
    }
    file* newfile = new file(filename);
    newfile->next = node->filehead;
    node->filehead = newfile;
}

void repositorymanager::deletefile(repositorynode* node, string filename) {
    if (node == nullptr) {
        cout << "Repository not found." << endl;
        return;
    }
    file* current = node->filehead;
    file* prev = nullptr;
    while (current != nullptr && current->name != filename) {
        prev = current;
        current = current->next;
    }
    if (current == nullptr) {
        cout << "File not found in the repository." << endl;
        return;
    }
    if (prev == nullptr) {
        node->filehead = current->next;
    }
    else {
        prev->next = current->next;
    }
    delete current;
    cout << "File '" << filename << "' deleted from the repository." << endl;
}

void repositorymanager::displayRepositoryStats(repositorynode* node) {
    if (node == nullptr) {
        cout << "Repository not found." << endl;
        return;
    }
    cout << "Repository Name: " << node->name << endl;
    cout << "Owner: " << node->owner << endl;
    cout << "Private: " << (node->isprivate ? "Yes" : "No") << endl;
    cout << "Fork Count: " << node->forkCount << endl;
    int fileCount = 0;
    file* currentFile = node->filehead;
    while (currentFile != nullptr) {
        fileCount++;
        currentFile = currentFile->next;
    }
    cout << "File Count: " << fileCount << endl;
    int commitCount = 0;
    commit* currentCommit = node->commithead;
    while (currentCommit != nullptr) {
        commitCount++;
        currentCommit = currentCommit->next;
    }
    cout << "Commit Count: " << commitCount << endl;
}

void repositorymanager::displayRepositoryStats(string repoName) {
    repositorynode* node = search(root, repoName);
    displayRepositoryStats(node);
}

void repositorymanager::loadfromcsv(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void repositorymanager::savetocsv(string filename) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Unable to open the file " << filename << endl;
        return;
    }
    file << "Owner,Name,IsPrivate,ForkCount" << endl;
    savetocsv(root, file);
    file.close();
    cout << "Repositories saved to CSV successfully." << endl;
}

void repositorymanager::savetocsv(repositorynode* node, ofstream& file) {
    if (node == nullptr) {
        return;
    }
    savetocsv(node->left, file);
    file << node->owner << "," << node->name << "," << (node->isprivate ? "Yes" : "No") << "," << node->forkCount << endl;
    savetocsv(node->right, file);
}

void repositorymanager::displayMenu() {
    cout << "\033[1;36m1. Create a repository\033[0m" << endl;
    cout << "\033[1;36m2. Delete a repository\033[0m" << endl;
    cout << "\033[1;36m3. Fork a repository\033[0m" << endl;
    cout << "\033[1;36m4. Add commits\033[0m" << endl;
    cout << "\033[1;36m5. Save repositories to CSV\033[0m" << endl;
    cout << "\033[1;36m6. Load repositories from CSV\033[0m" << endl;
    cout << "\033[1;36m7. Add file to repository\033[0m" << endl;
    cout << "\033[1;36m8. Delete file from repository\033[0m" << endl;
    cout << "\033[1;36m9. View repository stats\033[0m" << endl;
    cout << "\033[1;36m10. Exit\033[0m" << endl;
    cout << "\033[1;36mEnter your choice: \033[0m";
}



void repositorymanager::run() {
    User user;
    string username, password;
    int followers;
    vector<pair<string, bool>> repositories;

    bool logoutRequested = false;  // Flag to indicate logout request

    while (!logoutRequested) {
        cout << "\nMenu\n";
        cout << "\033[1;36m1. Press 1 for new registration\033[0m" << endl;
        cout << "\033[1;36m2. Press 2 to sign in\033[0m" << endl;
        cout << "\033[1;36m3. Press 3 to view a profile\033[0m" << endl;
        cout << "\033[1;36m4. Press 4 to log out\033[0m" << endl;
        cout << "Enter the number of your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "User Registration:" << endl;
            cout << "Enter username: ";
            cin >> username;

            cout << "Enter password: ";
            cin >> password;

            user.Userregister(username, password);
            break;
        }
        case 2: {
            cout << "\nUser Login:" << endl;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            system("cls");
            cout << "\n\t\t" << "\033[1;33mLoading:\033[0m ";
            user.delayProgram(1);
            cout << " - ";
            user.delayProgram(1);
            cout << " - ";
            user.delayProgram(1);
            cout << " - ";
            user.delayProgram(1);
            cout << " - ";
            cout << endl;
            if (user.Login(username, password)) {

                cout << "\033[1;32mLogin successful!\033[0m" << endl;
                user.delayProgram(3);

                cout << endl;
                system("cls");
                cout << "Press 1 to Repositories page." << endl;
                cout << "Press 2 to Social page." << endl;
                cout << "Enter your choice: ";
                cin >> choice;
                cout << "\n\t\t" << "\033[1;33mLoading:\033[0m ";
                user.delayProgram(1);
                cout << " - ";
                user.delayProgram(1);
                cout << " - ";
                user.delayProgram(1);
                cout << " - ";
                user.delayProgram(1);
                cout << " - ";
                cout << endl;
                if (choice == 1)
                {
                    int choice;
                    string owner, name, commit_message, filename, repoName, fileName;
                    int isprivate;
                    do {
                        displayMenu();
                        cin >> choice;
                        switch (choice) {
                        case 1: {
                            cout << "Enter owner name (owner name must be the same as your username): ";
                            cin >> owner;
                            cout << "Enter repository name: ";
                            cin >> name;
                            cout << "Is the repository private? (1 for Yes, 0 for No): ";
                            cin >> isprivate;
                            createrepository(owner, name, isprivate);
                            cout << "\033[1;32mRepository created successfully.\033[0m" << endl;
                            cout << endl;
                            user.delayProgram(2);

                            break;
                        }
                        case 2: {
                            cout << "Enter repository name to delete: ";
                            cin >> name;
                            deleterepository(name);
                            cout << "\033[1;32mRepository deleted successfully.\033[0m" << endl;
                            break;
                        }
                        case 3: {
                            cout << "Enter the name of the repository to fork: ";
                            cin >> name;
                            cout << "Enter the owner of the forked repository: ";
                            cin >> owner;
                            forkrepo(owner, name);
                            user.delayProgram(2);

                            break;
                        }
                        case 4: {
                            cout << "Enter the name of the repository to add commits: ";
                            cin >> name;
                            cout << "Enter the commit message: ";
                            cin.ignore();
                            getline(cin, commit_message);
                            addcommit(search(root, name), commit_message);
                            cout << "\033[1;32mCommit added successfully.\033[0m" << endl;
                            user.delayProgram(2);
                            break;
                        }
                        case 5: {
                            savetocsv("repo.csv");
                            cout << "\033[1;32mRepositories saved to CSV successfully.\033[0m" << endl;
                            user.delayProgram(2);
                            break;
                        }
                        case 6: {
                            loadfromcsv("repo.csv");
                            cout << "\033[1;32mRepositories loaded from CSV successfully.\033[0m" << endl;
                            user.delayProgram(2);
                            break;
                        }
                        case 7: {
                            cout << "Enter repository name: ";
                            cin >> repoName;
                            cout << "Enter file name to add: ";
                            cin >> fileName;
                            addfile(search(root, repoName), fileName);
                            cout << "\033[1;32mFile added to repository.\033[0m" << endl;
                            user.delayProgram(2);
                            break;
                        }
                        case 8: {
                            cout << "Enter repository name: ";
                            cin >> repoName;
                            cout << "Enter file name to delete: ";
                            cin >> fileName;
                            deletefile(search(root, repoName), fileName);
                            user.delayProgram(2);
                            break;
                        }
                        case 9: {
                            cout << "Enter repository name: ";
                            cin >> repoName;
                            displayRepositoryStats(repoName);
                            user.delayProgram(2);
                            break;
                        }
                        case 10: {
                            logoutRequested = true;
                            break;

                        }
                        default: {
                            cout << "\033[1;31mInvalid choice. Please enter a number between 1 and 12.\033[0m" << endl;
                        }
                        }
                    } while (!logoutRequested);
                    if (logoutRequested) {
                        cout << "\033[1;33mReturning to login page...\033[0m" << endl;
                        user.delayProgram(1);
                        cout << "-";
                        user.delayProgram(1);
                        cout << "-";


                        system("cls");
                        run();
                    }
                }

                else if (choice == 2)
                {

                    user.delayProgram(1);
                    cout << "--";
                    Graph network;
                    network.loadFromFile("social_network.csv");

                    string command;
                    while (true) {
                        cout << "Enter a command (follow/unfollow/display/save/exit): ";
                        cin >> command;
                        if (command == "follow") {
                            string user, follower;
                            cout << "Enter the user name: ";
                            cin >> user;
                            cout << "Enter the follower name: ";
                            cin >> follower;
                            network.follow(user, follower);
                        }

                        else if (command == "unfollow") {
                            string user, follower;
                            cout << "Enter the user name: ";
                            cin >> user;
                            cout << "Enter the follower name: ";
                            cin >> follower;
                            network.unfollow(user, follower);
                        }

                        else if (command == "display") {
                            string user;
                            cout << "Enter the user name: ";
                            cin >> user;
                            network.displayFollowers(user);
                        }

                        else if (command == "save") {
                            network.saveToFile("social_network.csv");
                            cout << "\033[1;32mSave it into file successfully !!\033[0m" << endl;
                        }

                        else if (command == "exit") {
                            cout << "\033[1;32mExit successfully....\033[0m" << endl;
                            break;
                        }

                        else {
                            cout << "\033[1;31mInvalid command. Please try again.\033[0m" << endl;
                        }
                    }

                }
            }
            else {
                cout << "\033[1;31mLogin failed!\033[0m" << endl;
            }
            break;
        }
        case 3: {
            cout << "\nView Profile:" << endl;
            cout << "Enter username: ";
            cin >> username;
            system("cls");
            cout << "\n\t\t" << "\033[1;33mLoading:\033[0m ";
            user.delayProgram(1);
            cout << " - ";
            user.delayProgram(1);
            cout << " - ";
            user.delayProgram(1);
            cout << " - ";
            user.delayProgram(1);
            cout << " - ";
            cout << endl;
            user.viewProfile(username);
            break;
        }
        case 4: {
            system("cls");
            cout << "\033[1;32mLogged out successfully!\033[0m" << endl;
            logoutRequested = true;  // Set flag to indicate logout
            break;
        }
        default: {
            cout << "\033[1;31mInvalid choice!\033[0m" << endl;
            break;
        }
        }
    }
}


void repositorymanager::deleterepository(string name) {
    root = deleterepository(root, name);
}

repositorynode* repositorymanager::deleterepository(repositorynode* node, string name) {
    if (node == nullptr) {
        return nullptr;
    }
    if (name < node->name) {
        node->left = deleterepository(node->left, name);
    }
    else if (name > node->name) {
        node->right = deleterepository(node->right, name);
    }
    else {
        if (node->left == nullptr) {
            repositorynode* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == nullptr) {
            repositorynode* temp = node->left;
            delete node;
            return temp;
        }
        repositorynode* temp = minvaluenode(node->right);
        node->name = temp->name;
        node->right = deleterepository(node->right, temp->name);
    }
    return node;
}

repositorynode* repositorymanager::minvaluenode(repositorynode* node) {
    repositorynode* current = node;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}


