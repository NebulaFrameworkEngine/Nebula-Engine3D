#include "TestDemo3D.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{

	TestDemo3D::TestDemo3D()
		: m_Proj(glm::perspective(glm::radians(45.0f), 700.0f / 700.0f, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_Model(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_Translation(0.0f, 0.0f, 0.0f),
		m_Rotation(0.0f),
		m_Transposition(0.0f, 0.0f, 0.0f),
		m_VSync(false) {

		// Define a square's vertices with positions and texture coordinates
		float vertecies[] = {
			// Base square (use full texture mapping)
			-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 0.0f,
			-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    1.0f, 0.0f,
			 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    1.0f, 1.0f,
			 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 1.0f,

			 // Pyramid tip (with appropriate texture coordinates)
			 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,    0.5f, 0.5f
		};

		// Ensure appropriate indices are set for drawing the pyramid sides
		unsigned int indicies[] = {
			// Base
			0, 1, 2,
			0, 2, 3,

			// Sides (define triangles for each side with correct vertex indices)
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4
		};


		glEnable(GL_DEPTH_TEST);

		m_VA = std::make_unique<VertexArray>();
		m_VB = std::make_unique<VertexBuffer>(vertecies, static_cast<unsigned int>(sizeof(vertecies)));

		VertexBufferLayout layout;
		layout.Push<float>(3); // Positions
		layout.Push<float>(3); // Color
		layout.Push<float>(2); // TexCoords
		m_VA->AddBuffer(*m_VB, layout);

		m_IB = std::make_unique<IndexBuffer>(indicies, static_cast<unsigned int>(sizeof(indicies) / sizeof(unsigned int)));

		m_Shader = std::make_unique<Shader>("res/shaders/basic.vert", "res/shaders/basic.frag");
		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("res/textures/brick.png", GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT);


		m_VA->Unbind();
		m_VB->Unbind();
		m_IB->Unbind();
		m_Shader->Unbind();
	}

	TestDemo3D::~TestDemo3D() {}

	void TestDemo3D::OnUpdate(float deltaTime) {}

	void TestDemo3D::OnRender() {

		glfwSwapInterval(m_VSync ? 1 : 0);  // Enable vsync

		glClearDepth(1.0f);  // Optional, sets the clear value for the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer renderer;
		m_Shader->Bind();
		m_Texture->Bind(0);

		// Set the uniform for texture slots
		m_Shader->SetUniform1i("u_Texture", 0);

		{
			// Apply translation and rotation
			glm::mat4 m_Model = glm::translate(glm::mat4(1.0f), m_Translation);  // Apply translation
			m_Model = glm::rotate(m_Model, glm::radians(m_Rotation), glm::vec3(0.0f, 1.0f, 0.0f));  // Apply rotation around Y-axis

			glm::mat4 mvp = m_Proj * m_View * m_Model;  // Model-View-Projection
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VA, *m_IB, *m_Shader);
		}

	}

	void TestDemo3D::OnImGuiRender() {
		ImGui::SliderFloat3("Translation", &m_Translation.x, -10.0f, 10.0f);  // Adjust object translation
		ImGui::SliderFloat("Rotation", &m_Rotation, 0.0f, 360.0f);  // Adjust object rotation

		ImGui::Checkbox("V-Sync", &m_VSync);  // Toggle V-Sync
	}
}
