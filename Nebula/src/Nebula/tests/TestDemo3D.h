#pragma once

#include "Test.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"

#include <glm/glm.hpp>

#include <memory>

namespace test
{
	class TestDemo3D : public Test {
	public:
		TestDemo3D(GLFWwindow* window);
		~TestDemo3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VA;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::vec3 m_Translation;
		float m_Rotation;
		glm::vec3 m_Transposition;
		glm::mat4 m_Proj, m_View, m_Model;

		GLFWwindow* m_Window;

		int m_WindowWidth, m_WindowHeight;

		float m_FOV;
	};
}
