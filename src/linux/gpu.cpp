#include <fstream>
#include <iostream>

#include "gpu.hpp"

//Implement Error Macro

gpu::gpu(/* args */)
{
    nvmlReturn_t result = nvmlInit_v2();
    if (result != NVML_SUCCESS)
    { 
        std::cout << "Unable to load NVML1: "<< nvmlErrorString(result) << std::endl;
        return;
    }
    result = nvmlDeviceGetHandleByUUID(0, &device);
    nvmlDeviceGetUUID(device, info.uuid, sizeof(info.uuid));
    result = nvmlDeviceGetHandleByUUID(info.uuid, &device);
}

bool gpu::reinit()
{
    nvmlReturn_t result = nvmlInit_v2();
    nvmlDeviceGetHandleByIndex_v2(0, &device);
    if(result != NVML_SUCCESS)
    {
        return false;
    }
    return true;
}

gpu::~gpu()
{
    nvmlShutdown();
}

void gpu::findInfo()
{
    nvmlDeviceGetHandleByIndex(0, &device);
    nvmlBrandType_t type;
    result = nvmlDeviceGetBrand(device, &type);
    if(result != NVML_SUCCESS)
    {
        std::cout << "Failed to find devices: " << nvmlErrorString(result) << std::endl;
        return;
    }
    std::cout << type << "          ";
    //result = nvmlSystemGetDriverVersion(info.driver, 15);
}

unsigned int gpu::getTemp()
{
    unsigned int temperature; 
    result = nvmlDeviceGetHandleByIndex(0, &device);
    if(result != NVML_SUCCESS)
    {
        std::cout << "Failed to find devices: " << nvmlErrorString(result) << std::endl;
        return 1;
    }
    result = nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temperature);
    if (NVML_SUCCESS != result) 
    {
	std::cout << "Failed to get temperature : " << nvmlErrorString(result) << std::endl;
        return 1;
    }
    return temperature;
}

unsigned int gpu::getWatts()
{
    unsigned int power;
    nvmlDeviceGetHandleByIndex(0, &device);
    result = nvmlDeviceGetPowerUsage(device, &power);

    if(result != NVML_SUCCESS)
    {
        std::cout << "ERROR: " << nvmlErrorString(result) << std::endl;
        return 0;
    }
    return power / 1000;
}

void gpu::getFanSpeeds()
{
    result = nvmlDeviceGetFanSpeed_v2(device, 0, &fanSpeeds);
    if(result != NVML_SUCCESS)
    {
        std::cout << "ERROR: " << nvmlErrorString(result) << std::endl;
        return;
    }
    std::cout << "Speeds: " << fanSpeeds << " ";
    result = nvmlDeviceGetFanSpeed_v2(device, 1, &fanSpeeds);
    std::cout << fanSpeeds << std::endl;
}