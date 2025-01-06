#ifndef SYSTEM_USAGE_MONITOR_H
#define SYSTEM_USAGE_MONITOR_H

#include <windows.h>
#include <pdh.h>
#include <stdexcept>
#include <nvml.h>

#pragma comment(lib, "pdh.lib")

class SystemUsageMonitor {
private:
    PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuTotal;

public:
    SystemUsageMonitor();        // �R���X�g���N�^
    ~SystemUsageMonitor();       // �f�X�g���N�^

    double getCpuUsage();        // CPU�g�p�����擾
    double getRamUsage();        // RAM�g�p�����擾
    double getGpuUsage();        // GPU�g�p�����擾�iNVIDIA��p�j
    double getVramUsage();       // VRAM�g�p�����擾�iNVIDIA��p�j
};

#endif // SYSTEM_USAGE_MONITOR_H
