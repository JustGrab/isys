#include "processor.hpp"

#include <iostream>
#include <unistd.h>

int main() {
    processor p;
    p.findModelinfo();
    p.findTemperatures();
    std::cout << p.getModel();
    std::cout << std::endl;
    while(1)
    {
        p.updateTemperatures();
        p.printTemps(); 
        usleep(1000000);
    }
    return 0;
}