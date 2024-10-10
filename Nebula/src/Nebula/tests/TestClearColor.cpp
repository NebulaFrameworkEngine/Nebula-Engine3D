#include "TestClearColor.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	TestClearColor::TestClearColor(GLFWwindow* window)
		: m_ClearColor{ 0.07f, 0.13f, 0.17f, 1.0f },
		m_Proj(glm::perspective(glm::radians(45.0f), 700.0f / 700.0f, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_Window(window) {
	}

	TestClearColor::~TestClearColor() {
	}

	void TestClearColor::OnUpdate(float deltaTime) {
	}

	void TestClearColor::OnRender() {

		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		int width, height;

		glfwGetFramebufferSize(m_Window, &width, &height);
		GLCall(glViewport(0, 0, width, height));

		float ratio = (float)width / height;

		m_Proj = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);
	}
	void TestClearColor::OnImGuiRender() {
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}
}