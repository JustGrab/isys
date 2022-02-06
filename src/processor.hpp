#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <experimental/filesystem>
#include <assert.h>

#define MAXCORE 1024
#define CPUMODELFILE "/proc/cpuinfo"
#define CPUSENSORS "/sys/class/hwmon"


struct processorInfo
{
    std::string model;
    uint32_t  physicalCores;
    uint32_t logicalCores;
    uint32_t normalFrequency;
    uint32_t  averageFrequency;
    std::vector<uint16_t> frequencies;
    std::vector<uint16_t> coreTemps;
    std::vector<uint16_t> maxTemps;
};
struct processorTemperature
{

};

class processor
{

public:
    processor();
    ~processor();
    void findModelinfo();
    void findCores();
    void findTemperatures();
    void findFrequencies();
    bool isHyperThreaded();

    void updateTemperatures();
    void updateFrequencies();

    std::string getModel();
    uint32_t getPhysicalCores();
    uint32_t getLogicalCores();

    void printTemps();

    

private:
    processorInfo pi;
    std::string tempSensor;
    std::string temperatureFilePath;
};


