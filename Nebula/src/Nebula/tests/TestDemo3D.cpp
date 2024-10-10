#include "TestDemo3D.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{

	TestDemo3D::TestDemo3D(GLFWwindow* window)
		: m_Proj(glm::perspective(glm::radians(45.0f), 700.0f / 700.0f, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_Translation(0.0f, 0.0f, 0.0f),
		m_Rotation(0.0f),
		m_Transposition(0.0f, 0.0f, 0.0f),
		m_Window(window),
		m_FOV(45.0f) {

		float vertecies[] = {
		       // Positions       /  Texture coordiantes//
			-0.5f, 0.0f,  0.5f,          0.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,          5.0f, 0.0f,
			 0.5f, 0.0f, -0.5f,          0.0f, 0.0f,
			 0.5f, 0.0f,  0.5f,          5.0f, 0.0f,
			 0.0f, 0.8f,  0.0f,          2.5f, 5.0f
		};

		unsigned int indicies[] = {
			0, 1, 2,
			0, 2, 3,
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4
		};


		GLCall(glEnable(GL_DEPTH_TEST));

		m_VA = std::make_unique<VertexArray>();
		m_VB = std::make_unique<VertexBuffer>(vertecies, static_cast<unsigned int>(sizeof(vertecies)));

		VertexBufferLayout layout;
		layout.Push<float>(3); // Positions
		layout.Push<float>(2); // TexCoords
		m_VA->AddBuffer(*m_VB, layout);

		m_IB = std::make_unique<IndexBuffer>(indicies, static_cast<unsigned int>(sizeof(indicies) / sizeof(unsigned int)));

		m_Shader = std::make_unique<Shader>("res/shaders/basic.vert", "res/shaders/basic.frag");
		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("res/textures/brick.png", GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT);


		m_VA->Unbind();
		m_VB->Unbind();
		m_IB->Unbind();
		m_Shader->Unbind();
	}

	TestDemo3D::~TestDemo3D() {}

	void TestDemo3D::OnUpdate(float deltaTime) {}

	void TestDemo3D::OnRender() {

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		int width, height;

		glfwGetFramebufferSize(m_Window, &width, &height);
		GLCall(glViewport(0, 0, width, height));

		float ratio = (float)width/height;

		m_Proj = glm::perspective(glm::radians(m_FOV), ratio, 0.1f, 100.0f);


		Renderer renderer;
		m_Shader->Bind();
		m_Texture->Bind(0);

		m_Shader->SetUniform1i("u_Texture", 0);

		{
			m_Model = glm::translate(glm::mat4(1.0f), m_Translation);
			m_Model = glm::rotate(m_Model, glm::radians(m_Rotation), glm::vec3(0.0f, 1.0f, 0.0f));

			m_Shader->SetUniformMat4f("u_Model", m_Model);
			m_Shader->SetUniformMat4f("u_View", m_View);
			m_Shader->SetUniformMat4f("u_Proj", m_Proj);
			renderer.Draw(*m_VA, *m_IB, *m_Shader);
		}

	}

	void TestDemo3D::OnImGuiRender() {
		ImGui::SliderFloat3("Translation", &m_Translation.x, -10.0f, 10.0f);
		ImGui::SliderFloat("Rotation", &m_Rotation, 0.0f, 360.0f);
		ImGui::SliderFloat("FOV", &m_FOV, 30.0f, 120.0f);
	}
}
