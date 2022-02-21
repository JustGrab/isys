#include "processor.hpp"
#include "gpu.hpp"

#include <iostream>
#include <unistd.h>

int main() {
    processor p;
    gpu g;
    p.findModelinfo();
    p.findTemperatures();
    std::cout << p.getModel();
    std::cout << std::endl;

    while(1)
    {
        //g.findInfo();
        std::cout << "GPU TEMP: " << g.getTemp() << std::endl;
        p.updateTemperatures();
        p.printTemps(); 
        usleep(1000000);
    }
    return 0;
}