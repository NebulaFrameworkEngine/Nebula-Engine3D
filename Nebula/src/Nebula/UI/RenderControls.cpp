#include "RenderControls.h"
#include "imgui/imgui.h"
#include <glm/glm.hpp>
#include <string>

RenderControls::RenderControls()
    : m_ResetValue(0.0f), m_ColumnWidth(100.0f) {
}

RenderControls::~RenderControls() {
}

void RenderControls::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth) {
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    // Get the correct line height
    float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    // Draw the X button and value
    DrawControlButton("X", values.x, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }, boldFont, buttonSize);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::CalcItemWidth() / 3); // Set the width for each DragFloat
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::SameLine(); // Keep Y on the same line

    // Draw the Y button and value
    DrawControlButton("Y", values.y, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f }, boldFont, buttonSize);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::CalcItemWidth() / 3);
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::SameLine(); // Keep Z on the same line

    // Draw the Z button and value
    DrawControlButton("Z", values.z, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f }, boldFont, buttonSize);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::CalcItemWidth() / 3);
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
}

void RenderControls::DrawControlButton(const std::string& label, float& value, const ImVec4& color, ImFont* font, const ImVec2& buttonSize) {
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ color.x + 0.1f, color.y + 0.1f, color.z + 0.1f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
    ImGui::PushFont(font);
    if (ImGui::Button(label.c_str(), buttonSize))
        value = m_ResetValue;  // Reset to default when button is pressed
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
}
