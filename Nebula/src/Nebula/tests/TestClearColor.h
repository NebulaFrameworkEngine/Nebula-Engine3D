#pragma once

#include "Test.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace test {
	class TestClearColor : public Test {
	public:
		TestClearColor(GLFWwindow* window);
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];

		glm::mat4 m_Proj, m_View, m_Model;

		int m_WindowWidth, m_WindowHeight;

		GLFWwindow* m_Window;
	};
}