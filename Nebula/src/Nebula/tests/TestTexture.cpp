#include "TestTexture.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"

#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	TestTexture::TestTexture()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0), m_TranslationB(400, 400, 0),
		m_VSync(false)	{

		// Define a square's vertices with positions and texture coordinates
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f, // 0
			 50.0f, -50.0f, 1.0f, 0.0f, // 1
			 50.0f,  50.0f, 1.0f, 1.0f, // 2
			-50.0f,  50.0f, 0.0f, 1.0f  // 3
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

		// Set the texture slots
		m_TextureA->Bind(0);
		m_TextureB->Bind(1);

		// Set the uniform for texture slots
		m_Shader->SetUniform1i("u_Texture", 0);

		m_VA->Unbind();
		m_VB->Unbind();
		m_IB->Unbind();
		m_Shader->Unbind();
	}

	TestTexture::~TestTexture() {}

	void TestTexture::OnUpdate(float deltaTime) {}

	void TestTexture::OnRender() {
		
		glfwSwapInterval(m_VSync); // Enable vsync

		Renderer renderer;

		m_Shader->Bind();

		// Render first texture
		m_TextureA->Bind(0);
		m_Shader->SetUniform1i("u_Texture", 0);
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VA, *m_IB, *m_Shader);
		}

		// Render second texture
		m_TextureB->Bind(1);
		m_Shader->SetUniform1i("u_Texture", 1);
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VA, *m_IB, *m_Shader);
		}
	}

	void TestTexture::OnImGuiRender() {
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);

		ImGui::Checkbox("V-Sync", &m_VSync);
	}

}
