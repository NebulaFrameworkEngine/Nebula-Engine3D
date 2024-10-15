#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Texture.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "UI/RenderControls.h"

#include <glm/gtc/matrix_transform.hpp>

#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"
#include "tests/TestDemo3D.h"

namespace Nebula {
	
	Application::Application() {}
	Application::~Application() {}

	int Application::Run() {
		GLFWwindow* window;

		if (!glfwInit()) {
			std::cout << "[-] There was an error while initializing GLFW\n";
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		int windowWidth = 700;
		int windowHeight = 700;

		window = glfwCreateWindow(windowWidth, windowHeight, "Nebula", NULL, NULL);
		if (!window) {
			std::cout << "[-] There was an error while creating the window\n";
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "[-] There was an error while initializing GLAD\n";
			return -1;
		}
		
		{

			glm::mat4 Proj = glm::perspective(glm::radians(45.0f), 700.0f / 700.0f, 0.1f, 100.0f);
			glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

			glm::vec3 Translation(0.0f, 0.0f, 0.0f);
			glm::vec3 Rotation(0.0f, 0.0f, 0.0f);
			glm::vec3 Transposition(0.0f, 0.0f, 0.0f);

			float FOV = 45.0f;


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

			GLCall(glEnable(GL_BLEND));
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


			VertexArray va;
			VertexBuffer vb(vertecies, sizeof(vertecies));

			VertexBufferLayout layout;
			layout.Push<float>(3); // Positions
			layout.Push<float>(2); // TexCoords
			va.AddBuffer(vb, layout);

			IndexBuffer ib(indicies, sizeof(indicies)/sizeof(unsigned int));

			Shader shader("res/shaders/basic.vert", "res/shaders/basic.frag");
			shader.Bind();

			Texture texture("res/textures/brick.png", GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST, GL_REPEAT, GL_REPEAT);

			Renderer renderer;

			va.Unbind();
			vb.Unbind();
			ib.Unbind();
			shader.Unbind();

			ImGui::CreateContext();

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init();
			ImGui::StyleColorsDark();

			test::Test* currentTest = nullptr;
			test::TestMenu* testMenu = new test::TestMenu(currentTest);
			currentTest = testMenu;

			testMenu->RegisterTest<test::TestClearColor>("Clear Color", window);
			testMenu->RegisterTest<test::TestTexture>("Texture Test", window);
			testMenu->RegisterTest<test::TestDemo3D>("3D Demo", window);

			ImGuiIO& io = ImGui::GetIO();

			RenderControls translationControl;
			RenderControls rotationControl;

			bool VSync = false;
			
			int width, height = 0;

			while (!glfwWindowShouldClose(window)) {
				
				renderer.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

				GLCall(glfwSwapInterval((int)VSync));

				glfwGetFramebufferSize(window, &width, &height);
				GLCall(glViewport(0, 0, width, height));

				float ratio = (float)width / height;

				Proj = glm::perspective(glm::radians(FOV), ratio, 0.1f, 100.0f);

				shader.Bind();
				texture.Bind(0);

				shader.SetUniform1i("u_Texture", 0);

				{
					Model = glm::translate(glm::mat4(1.0f), Translation);
					Model = glm::rotate(Model, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
					Model = glm::rotate(Model, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
					Model = glm::rotate(Model, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

					shader.SetUniformMat4f("u_Model", Model);
					shader.SetUniformMat4f("u_View", View);
					shader.SetUniformMat4f("u_Proj", Proj);
					renderer.Draw(va, ib, shader);
				}

				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				ImGui::Begin("Panel");
				
				translationControl.DrawVec3Control("Translation", Translation, 0.0f, 100.0f);
				rotationControl.DrawVec3Control("Rotation", Rotation, 0.0f, 100.0f);

				ImGui::SliderFloat("FOV", &FOV, 30.0f, 120.0f);

				if (ImGui::CollapsingHeader("Tests")) {
					if (currentTest) {
						currentTest->OnUpdate(0.0f);
						currentTest->OnRender();

						if (currentTest != testMenu && ImGui::Button("<-")) {
							delete currentTest;
							currentTest = testMenu;
						}
						currentTest->OnImGuiRender();
					}
				}
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::Checkbox("Enable V-Sync", &VSync);

				ImGui::End();

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				glfwSwapBuffers(window);
				glfwPollEvents();
			}
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
		return 0;
	}
}