#pragma once

#include "Test.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"

#include "glm/glm.hpp"

#include <memory>

namespace test {
	class TestTexture : public Test {
	public:
		TestTexture(GLFWwindow* window);
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VA;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_TextureA;
		std::unique_ptr<Texture> m_TextureB;

		glm::vec3 m_TranslationA, m_TranslationB;
		glm::mat4 m_Proj, m_View, m_Model;

		GLFWwindow* m_Window;

		bool m_VSync;

	};
}
