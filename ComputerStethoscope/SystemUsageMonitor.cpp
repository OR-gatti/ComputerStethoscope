#include "SystemUsageMonitor.h"
#include <iostream>

SystemUsageMonitor::SystemUsageMonitor() {
    // CPU使用率クエリの初期化
    if (PdhOpenQuery(nullptr, 0, &cpuQuery) != ERROR_SUCCESS) {
        throw std::runtime_error("Failed to open PDH query for CPU usage.");
    }
    if (PdhAddCounter(cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time"), 0, &cpuTotal) != ERROR_SUCCESS) {
        throw std::runtime_error("Failed to add PDH counter for CPU usage.");
    }
    PdhCollectQueryData(cpuQuery); // 初期データ収集

    // NVMLの初期化
    nvmlReturn_t result = nvmlInit();
    if (result != NVML_SUCCESS) {
        throw std::runtime_error("Failed to initialize NVML: " + std::string(nvmlErrorString(result)));
    }
}

SystemUsageMonitor::~SystemUsageMonitor() {
    PdhCloseQuery(cpuQuery);
    nvmlShutdown();
}

double SystemUsageMonitor::getCpuUsage() {
    PdhCollectQueryData(cpuQuery);

    PDH_FMT_COUNTERVALUE counterVal;
    if (PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, nullptr, &counterVal) != ERROR_SUCCESS) {
        throw std::runtime_error("Failed to get formatted counter value for CPU usage.");
    }
    return counterVal.doubleValue;
}

double SystemUsageMonitor::getRamUsage() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (!GlobalMemoryStatusEx(&memInfo)) {
        throw std::runtime_error("Failed to get RAM usage.");
    }
    double totalPhys = static_cast<double>(memInfo.ullTotalPhys) / (1024 * 1024 * 1024); // GB
    double usedPhys = totalPhys - static_cast<double>(memInfo.ullAvailPhys) / (1024 * 1024 * 1024); // GB
    return (usedPhys / totalPhys) * 100.0; // 使用率（％）
}

double SystemUsageMonitor::getGpuUsage() {
    nvmlDevice_t device;
    unsigned int gpuUtilization;

    nvmlReturn_t result = nvmlDeviceGetHandleByIndex(0, &device); // 最初のGPU
    if (result != NVML_SUCCESS) {
        throw std::runtime_error("Failed to get GPU handle: " + std::string(nvmlErrorString(result)));
    }

    nvmlUtilization_t utilization;
    result = nvmlDeviceGetUtilizationRates(device, &utilization);
    if (result != NVML_SUCCESS) {
        throw std::runtime_error("Failed to get GPU utilization: " + std::string(nvmlErrorString(result)));
    }

    gpuUtilization = utilization.gpu; // GPU使用率
    return static_cast<double>(gpuUtilization);
}

double SystemUsageMonitor::getVramUsage() {
    nvmlDevice_t device;
    nvmlMemory_t memory;

    nvmlReturn_t result = nvmlDeviceGetHandleByIndex(0, &device); // 最初のGPU
    if (result != NVML_SUCCESS) {
        throw std::runtime_error("Failed to get GPU handle: " + std::string(nvmlErrorString(result)));
    }

    result = nvmlDeviceGetMemoryInfo(device, &memory);
    if (result != NVML_SUCCESS) {
        throw std::runtime_error("Failed to get VRAM usage: " + std::string(nvmlErrorString(result)));
    }

    double totalMemory = static_cast<double>(memory.total) / (1024 * 1024 * 1024); // GB
    double usedMemory = static_cast<double>(memory.used) / (1024 * 1024 * 1024); // GB
    return (usedMemory / totalMemory) * 100.0; // 使用率（％）
}
