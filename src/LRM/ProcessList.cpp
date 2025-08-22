#include "ProcessList.h"

#include <vector>

#include "Process.h"
#include "imgui.h"

void RenderProcessTable(std::vector<Process*>& processes, int& selectedIndex) {
    if (ImGui::BeginTable("Processes", 5,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                              ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("PID");
        ImGui::TableSetupColumn("Command");
        ImGui::TableSetupColumn("VIRT");
        ImGui::TableSetupColumn("RES");
        ImGui::TableSetupColumn("SHR");
        ImGui::TableHeadersRow();

        int process_size = processes.size();

        for (int i = 0; i < processes.size(); ++i) {
            const auto& proc = *processes[i];

            if (proc.cmd.empty()) {
                continue;
            }

            ImGui::TableNextRow();

            ImGui::SetItemAllowOverlap();

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", proc.cmd.c_str());
            ImGui::SetItemTooltip("%s", proc.program.c_str());

            ImGui::TableSetColumnIndex(2);
            // convert to bytes then convert to mb
            ImGui::Text("%.2fM", static_cast<float>(proc.virtual_mem) * 4096 /
                                     (1024 * 1024));

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.2fM", static_cast<float>(proc.resident_mem) * 4096 /
                                     (1024 * 1024));

            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%.2fM", static_cast<float>(proc.shared_mem) * 4096 /
                                     (1024 * 1024));

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", proc.pid);
            std::string row_label = "##row" + std::to_string(i);

            ImGui::SameLine();
            if (ImGui::Selectable(row_label.c_str(), selectedIndex == i,
                                  ImGuiSelectableFlags_SpanAllColumns)) {
                selectedIndex = i;
            }
        }

        ImGui::EndTable();
    }
}
