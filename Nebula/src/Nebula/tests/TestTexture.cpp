#include "TestTexture.h"
#include <GLFW/glfw3.h>

#include "Renderer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

namespace test {

	TestTexture::TestTexture(GLFWwindow* window)
		: m_Proj(glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(0.2f, 0.37f, 0), m_TranslationB(0.42f, 0.74f, 0),
		m_Window(window),
		m_WindowWidth(0), m_WindowHeight(0)	{

		float positions[] = {
		//    Positions    / Texture Coordinates //
			-0.05f, -0.05f,    0.0f, 0.0f,
			 0.05f, -0.05f,    1.0f, 0.0f,
			 0.05f,  0.05f,    1.0f, 1.0f,
			-0.05f,  0.05f,    0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_VA = std::make_unique<VertexArray>();
		m_VB = std::make_unique<VertexBuffer>(positions, static_cast<unsigned int>(sizeof(positions)));

		VertexBufferLayout layout;
		layout.Push<float>(2); // Positions
		layout.Push<float>(2); // TexCoords
		m_VA->AddBuffer(*m_VB, layout);

		m_IB = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/basic.vert", "res/shaders/basic.frag");
		m_Shader->Bind();

		m_TextureA = std::make_unique<Texture>("res/textures/cat.png");
		m_TextureB = std::make_unique<Texture>("res/textures/dog.png");

		m_TextureA->Bind(0);
		m_TextureB->Bind(1);

		m_Shader->SetUniform1i("u_Texture", 0);

		m_VA->Unbind();
		m_VB->Unbind();
		m_IB->Unbind();
		m_Shader->Unbind();
	}

	TestTexture::~TestTexture() {}

	void TestTexture::OnUpdate(float deltaTime) {}

	void TestTexture::OnRender() {

		Renderer renderer;
		
		renderer.Clear();

		glfwGetFramebufferSize(m_Window, &m_WindowWidth, &m_WindowHeight);

		float ratio = (float)m_WindowWidth / m_WindowHeight;


		m_Shader->Bind();

		m_TextureA->Bind(0);
		m_Shader->SetUniform1i("u_Texture", 0);
		{
			glm::mat4 m_Model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			m_Model = glm::scale(m_Model, glm::vec3(1.0f / ratio, 1.0f, 1.0f));
			m_Shader->SetUniformMat4f("u_Model", m_Model);
			m_Shader->SetUniformMat4f("u_View", m_View);
			m_Shader->SetUniformMat4f("u_Proj", m_Proj);
			renderer.Draw(*m_VA, *m_IB, *m_Shader);
		}

		m_TextureB->Bind(1);
		m_Shader->SetUniform1i("u_Texture", 1);
		{
			glm::mat4 m_Model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			m_Model = glm::scale(m_Model, glm::vec3(1.0f / ratio, 1.0f, 1.0f));
			m_Shader->SetUniformMat4f("u_Model", m_Model);
			m_Shader->SetUniformMat4f("u_View", m_View);
			m_Shader->SetUniformMat4f("u_Proj", m_Proj);
			renderer.Draw(*m_VA, *m_IB, *m_Shader);
		}
	}

	void TestTexture::OnImGuiRender() {
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 1.0f);
	}

}
