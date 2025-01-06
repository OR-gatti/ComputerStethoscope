#include <iostream>
#include<chrono>
#include <iomanip>
#include "SystemUsageMonitor.h"


int main() {
    try {
        SystemUsageMonitor monitor;

        while (true) {
            auto now = std::chrono::system_clock::now();
            std::time_t c_time = std::chrono::system_clock::to_time_t(now);

            std::tm local_time;

            std::cout << "--------------------------------------" << std::endl;
            if (localtime_s(&local_time, &c_time) == 0) { // エラーチェック
                std::cout << std::put_time(&local_time, "%Y/%m/%d %H:%M:%S") << std::endl;
            }
            else {
                std::cerr << "localtime_s failed." << std::endl; // エラー処理
                return 1; // エラーコードを返す
            }
            double cpu = monitor.getCpuUsage();
            double ram = monitor.getRamUsage();
            double gpu = monitor.getGpuUsage();
            double vram = monitor.getVramUsage();

            std::cout << "CPU Usage: " << cpu << "%" << std::endl;
            std::cout << "RAM Usage: " << ram << "%" << std::endl;
            std::cout << "GPU Usage: " << gpu << "%" << std::endl;
            std::cout << "VRAM Usage: " << vram << "%" << std::endl;

            Sleep(1000); // 1秒ごとに更新
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
