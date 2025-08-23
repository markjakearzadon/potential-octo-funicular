#include "Application.h"

#include <array>
#include <csignal>
#include <iostream>

#include "CpuManager.h"
#include "Process.h"
#include "ProcessList.h"
#include "imgui.h"
#include "implot.h"

namespace {
void CoreUI(CPU &cpu) {
    std::vector<Core> cores = cpu.GetCpuUsage();
    if (!cores.empty())
        cores.erase(cores.begin());

    static std::vector<Core> prevCores;

    ImGui::BeginChild("Cores", ImVec2(0, 0),
                      ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);

    if (!prevCores.empty()) {
        for (int i = 0; i < cores.size(); i++) {
            const Core &currentCore = cores[i];
            const Core &previousCore = prevCores[i];
            int currentCoreIdle = currentCore.stat[3] + currentCore.stat[4];
            int currentCoreTotal = 0;
            for (int i : currentCore.stat)
                currentCoreTotal += i;
            int previousCoreIdle = previousCore.stat[3] + previousCore.stat[4];
            int previousCoreTotal = 0;
            for (int i : previousCore.stat)
                previousCoreTotal += i;
            int TotalDiff = currentCoreTotal - previousCoreTotal;
            int IdleDiff = currentCoreIdle - previousCoreIdle;
            float coreUsage = 0.0f;
            if (TotalDiff > 0)
                coreUsage =
                    (float)(TotalDiff - IdleDiff) / (float)TotalDiff * 100.0f;
            else
                coreUsage = 0;
            ImGui::Text("Core %d: %.1f%% race condition error me thinks", i,
                        coreUsage);
        }
    } else {
        for (int i = 0; i < cores.size(); i++) {
            ImGui::Text("Core %d: 0.0%%", i);
        }
    }
    ImGui::EndChild();
    prevCores = cores;
}

}  // namespace
namespace Application {

void ProcessUI(std::vector<Process> &processes) {
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysVerticalScrollbar;

    static ImGuiTextFilter filter;
    static int selectedIndex = -1;
    std::vector<Process *> filteredProcesses;

    filteredProcesses.clear();
    for (const auto &proc : processes) {
        if (filter.PassFilter(proc.program.c_str())) {
            filteredProcesses.push_back(const_cast<Process *>(&proc));
        }
    }

    if (ImGui::Begin("Process", nullptr, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            filter.Draw("Filter");
            ImGui::Separator();
            if (ImGui::Button("Terminate")) {
                if (!(selectedIndex < 0)) {
                    kill(filteredProcesses[selectedIndex]->pid, SIGINT);
                    selectedIndex = -1;
                }
            }
            ImGui::EndMenuBar();
        }

        RenderProcessTable(filteredProcesses, selectedIndex);
    }
    ImGui::End();
}

void CpuUI(CPU &cpu) {
    static float t = 0;
    static float sample_timer = 0.0f;
    static constexpr int BUFFER_SIZE = 3600;
    static std::array<float, BUFFER_SIZE> usage;
    static std::array<float, BUFFER_SIZE> time;
    static int index = 0;
    static int count = 0;
    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels |
                                   ImPlotAxisFlags_NoGridLines |
                                   ImPlotAxisFlags_NoHighlight;

    float dt = ImGui::GetIO().DeltaTime;
    t += dt;
    sample_timer += dt;

    ImPlotAxisFlags axisflags = ImPlotAxisFlags_NoGridLines |
                                ImPlotAxisFlags_NoHighlight |
                                ImPlotAxisFlags_NoTickMarks;

    ImVec2 plot_size = ImVec2(0, 0);
    ImGui::BeginChild("Cpu", ImVec2(0, 0),
                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);

    if (ImPlot::BeginPlot("CPU", ImVec2(0, 0),
                          ImPlotFlags_NoLegend | ImPlotFlags_NoFrame)) {
        ImPlot::SetupAxis(ImAxis_X1, nullptr, axisflags);
        ImPlot::SetupAxis(ImAxis_Y1, nullptr,
                          axisflags | ImPlotAxisFlags_Opposite);
        ImPlot::SetupAxisLimits(ImAxis_X1, t - 60.0f, t - 0.9,
                                ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 100);
        if (sample_timer >= 1.0f) {
            float cpuUsage = cpu.GetCpuPercentage();
            sample_timer = 0.0f;
            time[index] = t;
            usage[index] = cpuUsage;
            std::cout << cpuUsage << "\n";
            index = (index + 1) % BUFFER_SIZE;
            count = std::min(count + 1, BUFFER_SIZE);
        }
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::PlotShaded("cpu", time.data(), usage.data(), count, 0, 0);
        ImPlot::PopStyleVar();
        ImPlot::PlotLine("cpu", time.data(), usage.data(), count, 0, 0);
        ImPlot::EndPlot();
    }

    // Core usage
    ImGui::EndChild();
    CoreUI(cpu);
}

};  // namespace Application
