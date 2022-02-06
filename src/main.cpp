#include "processor.hpp"
#include <unistd.h>

int main() {
    processor p;
    p.findTemperatures();
    while(1)
    {
        p.updateTemperatures();
        p.printTemps(); 
        usleep(1000000);
    }
    return 0;
}