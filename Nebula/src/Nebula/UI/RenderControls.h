#pragma once

#include <string>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

class RenderControls {
public:
    RenderControls();
    ~RenderControls();

    void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);

private:
    void DrawControlButton(const std::string& label, float& value, const ImVec4& color, ImFont* font, const ImVec2& buttonSize);

private:
    std::string m_Label;
    float m_ResetValue;
    float m_ColumnWidth;
};
