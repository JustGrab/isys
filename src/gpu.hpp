#include <string>

#include "nvml.h"

struct gpuInfo
{
    std::string model;
    std::string irq;
    char uuid[20];
    char driver[80];
};

class gpu
{
public:
    gpu();
    ~gpu();
    bool reinit();
    void findInfo();

    unsigned int getTemp();
    unsigned int getWatts();
    void getFanSpeeds();

private:
    gpuInfo info;
    nvmlReturn_t result;
    nvmlDevice_t device;

    unsigned int fanSpeeds;
};
