#include "processor.hpp"


processor::processor()
{

}

processor::~processor()
{

}

void processor::findModelinfo()
{
    std::ifstream file;
    std::string fileData;
    std::string modelInfo;
    file.open(CPUMODELFILE);
    if(file.fail())
    {
        std::cout << "CPU INFO FILE COULD NOT BE FOUND. ";
    }
    while(getline(file, fileData))
    {
        if(fileData.substr(0,10) == "model name" )
        {
            size_t position = fileData.find(":"); 
            pi.model = fileData.substr(position + 2);
        }
    }
    file.close();
}
void processor::findCores()
{
    std::ifstream file;
    std::string fileData;
    std::string modelInfo;
    file.open(CPUMODELFILE);
    if(file.fail())
    {
        std::cout << "CPU INFO FILE COULD NOT BE FOUND. ";
    }
    while(getline(file, fileData))
    {
        if(fileData.substr(0,9) == "cpu cores" )
        {
            size_t position = fileData.find(":"); 
            pi.physicalCores = stoi(fileData.substr(position + 2));
        }
        if(fileData.substr(0,8) == "siblings" )
        {
            size_t position = fileData.find(":"); 
            pi.logicalCores = stoi(fileData.substr(position + 2));
        }
    }
    pi.coreTemps.resize(pi.physicalCores + 1);
    file.close();
}

void processor::findTemperatures()
{
    //Find coretemp sensor, default path for sensors in "/sys/class/hwmon"
    bool found = false;
    std::string fileName = "/sys/class/hwmon/hwmon0/name";
    std::ifstream file;
    std::string sensorName;
    int fileCounter = 0;
    int totalCounter = 0;
    file.open(fileName);

    //POTENTIALLY CHANGE TO IN THE FUTURE
    /* 
    std::string path = "/sys/devices/platform/coretemp.0/hwmon";
    for (const auto & entry : std::experimental::filesystem::directory_iterator(path))
        std::cout << entry.path() << std::endl;
    */
    while(file)
    {
        getline(file, sensorName);
        if(sensorName == "coretemp")
        {
            found = true;
            temperatureFilePath = fileName.replace(fileName.find("/name"), 5, "");
            tempSensor = "coretemp";
            std::cout << temperatureFilePath;
            file.close();
            return;
        }
        else
        {
            file.close();
            int noPos = fileName.find(std::to_string(fileCounter));
            fileCounter++;
            fileName.replace(noPos, 1, std::to_string(fileCounter));
            file.open(fileName);
        }
    }
    file.close();
    //No coretemp sensor found, attemp to find x86_pkg_temp 
    if(!found)
    {
         std::cout << "we in here";
        fileName = "/sys/class/thermal/thermal_zone0/type";
        file.open(fileName);
        while(file)
        {
            getline(file, sensorName);
            if(sensorName == "x86_pkg_temp")
            {
                temperatureFilePath = fileName.replace(fileName.find("/type"), 5, "");\
                tempSensor = "x86_pkg_temp";
                std::cout << temperatureFilePath;
                file.close();
                return;
            }
            else
            {
                file.close();
                int noPos = fileName.find(std::to_string(fileCounter));
                fileCounter++;
                fileName.replace(noPos, 1, std::to_string(fileCounter));
                file.open(fileName);
            }
        }
    }
    else
    {
        std::cout << "Reliable CPU temperatures could not be found!\n";
    }
    file.close();
}

void processor::findFrequencies()
{
    //Unimplemented
}

void processor::updateTemperatures()
{
    if(pi.coreTemps.size() == 0)
    {
        findCores();
    }
    assert(pi.coreTemps.size() != 0);
    std::ifstream file;
    std::string fileName = temperatureFilePath + "/temp1_input";
    std::string temps;
    file.open(fileName);
    if(!file)
    {
        std::cout << temperatureFilePath << " cannot be opened.\n";
        return;
    }
    //Temp1_input = all cores, temp2_input = core 0, tempp3_input = core 1....
    int coreCounter = 1;
    while(file)
    {
        //std::cout << "COUNT: " << coreCounter << " FILENAME: " << fileName <<"\n";
        getline(file, temps);
        pi.coreTemps[coreCounter - 1] = std::stoi(temps);
        file.close();
        int noPos = fileName.rfind(std::to_string(coreCounter));
        coreCounter++;
        fileName.replace(noPos, 1, std::to_string(coreCounter));
        //std::cout << "COUNT: " << coreCounter << " FILENAME: " << fileName <<"\n";
        file.open(fileName);
    }
    file.close();
}

bool processor::isHyperThreaded()
{
    if(pi.physicalCores == pi.logicalCores)
        return false;
    return true;
}

std::string processor::getModel()
{
    if(pi.model.empty())
        return "NOT FOUND";
    return pi.model;
}

uint32_t  processor::getPhysicalCores()
{
  return (pi.physicalCores < MAXCORE ? pi.physicalCores : 0);
}

uint32_t  processor::getLogicalCores()
{
  return (pi.physicalCores < MAXCORE * 2 ? pi.physicalCores : 0);
} 

void processor::printTemps()
{
    for(auto t: pi.coreTemps)
    {
        std::cout << t << "   ";
    }
    std::cout << "\n";
}