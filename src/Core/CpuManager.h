#pragma once
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

#include "Models/Cpu.h"

class CPU {
   public:
    CPU();
    ~CPU();

    // const char* GetModel() const {}
    // int GetCoreCount() const;
    // float GetClockSpeedGHz() const {}

    std::vector<Core> GetCpuUsage();
    std::vector<Core> GetCoreUsage();
    float GetCpuPercentage();
    std::vector<Core> GetCores();

    void start_();
    void update_();

   private:
    std::atomic<bool> m_running;
    float m_total;
    float m_idle;
    std::vector<Core> m_cpu;
    std::vector<Core> m_core;
    mutable std::mutex m_CpuMutex;
    std::thread m_CpuThread;
};
