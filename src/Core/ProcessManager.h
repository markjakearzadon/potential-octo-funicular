#pragma once
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

#include "Process.h"

class ProcessManager {
   public:
    ProcessManager();
    ~ProcessManager();

    void start_();
    void update_();

    std::vector<Process> get_process();

   private:
    std::vector<Process> m_process;
    std::atomic<bool> m_running;
    std::mutex m_ProcessMutex;
    std::thread m_processThread;
};