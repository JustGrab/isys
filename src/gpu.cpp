#include <fstream>
#include <iostream>

#include "gpu.hpp"
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
    if (result != NVML_SUCCESS)
    { 
        std::cout << "Unable to load NVML: "<< nvmlErrorString(result) << std::endl;
        return false;
    }
    nvmlDeviceGetHandleByIndex_v2(0, &device);
    return true;
}

gpu::~gpu()
{
    nvmlShutdown();
}

void gpu::findInfo()
{
    //Unimplemented
}

unsigned int gpu::getTemp()
{
    unsigned int temperature; 
    //if (result != NVML_SUCCESS)
    //{ 
     //   std::cout << "Unable to load NVML3: "<< nvmlErrorString(result) << std::endl;
      //  return 1;
    //}
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
    nvmlDevice_t device;
    nvmlReturn_t result = nvmlInit();
    if (result != NVML_SUCCESS)
    { 
        std::cout << "Unable to load NVML2: "<< nvmlErrorString(result) << std::endl;
        return 0;
    }
    nvmlDeviceGetHandleByIndex(0, &device);
    result = nvmlDeviceGetPowerUsage(device, &power);

    //result = nvmlSystemGetDriverVersion(info.driver, 15);
    if(result != NVML_SUCCESS)
    {
        std::cout << "ERROR: " << nvmlErrorString(result) << std::endl;
        return 0;
    }
    return power;
}