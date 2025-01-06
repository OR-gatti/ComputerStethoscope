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
    SystemUsageMonitor();        // コンストラクタ
    ~SystemUsageMonitor();       // デストラクタ

    double getCpuUsage();        // CPU使用率を取得
    double getRamUsage();        // RAM使用率を取得
    double getGpuUsage();        // GPU使用率を取得（NVIDIA専用）
    double getVramUsage();       // VRAM使用率を取得（NVIDIA専用）
};

#endif // SYSTEM_USAGE_MONITOR_H
