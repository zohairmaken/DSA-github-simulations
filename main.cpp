#include "repositorymanager.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <windows.h>
int main() {
    std::cout << "\033[1;36m"; 

    std::cout << "  GGGG   i  TTTTT  H   H  U   U  BBBB  \n";
    std::cout << " G       i    T    H   H  U   U  B   B \n";
    std::cout << " G  GG   i    T    HHHHH  U   U  BBBB  \n";
    std::cout << " G   G   i    T    H   H  U   U  B   B \n";
    std::cout << "  GGGG   i    T    H   H   UUU   BBBB  \n";

    std::cout << "\033[0m" << std::endl; 
    Sleep(1000);
    std::cout << "\033[1;36m"; 

    std::cout << "\t\t\t  SSSS  i  MMMMMMMMM   U   U  L       AAA   TTTTT  IIIII   OOOOO  N   N   SSSS  \n";
    std::cout << "\t\t\t S      i  M   M   M   U   U  L      A   A    T      I     O   O  NN  N   S     \n";
    std::cout << "\t\t\t  SSS   i  M   M   M   U   U  L      AAAAA    T      I     O   O  N N N   SSSS  \n";
    std::cout << "\t\t\t     S  i  M   M   M   U   U  L      A   A    T      I     O   O  N  NN      S \n";
    std::cout << "\t\t\t SSSS   i  M   M   M    UUU   LLLLL  A   A    T    IIIII   OOOOO  N   N   SSSS  \n";

    std::cout << "\033[0m" << std::endl;
    Sleep(1000);
    system("cls");
    std::cout << "\033[1;31m";

    std::cout << "\n\n\n\t\t\tLOADING";
    std::cout.flush();

    
    for (int i = 0; i < 6; ++i) {
        std::cout << "|";
        std::cout.flush();

      
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "\033[0m" << std::endl;
    system("cls");
    repositorymanager manager;
    manager.run();
    return 0;
}
