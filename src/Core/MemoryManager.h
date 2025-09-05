#include <atomic>
#include <mutex>
#include <thread>

#include "Mem.h"
class Memory {
   public:
    Memory();
    ~Memory();

    void update_();
    Mem GetMem() const;

   private:
    Mem m_Mememory;
    std::atomic<bool> running;
    mutable std::mutex m_MemoryMutex;
    std::thread m_MemoryThread;
};
