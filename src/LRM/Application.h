#pragma once

#include <vector>

#include "CpuManager.h"
#include "Process.h"

namespace Application {
void ProcessUI(std::vector<Process>& processes);
void CpuUI(CPU& cpu);
};  // namespace Application
