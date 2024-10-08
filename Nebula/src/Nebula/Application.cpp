#include "Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Utility.h"
#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "vendor/stb_image/stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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

		window = glfwCreateWindow(700, 700, "Nebula", NULL, NULL);
		if (!window) {
			std::cout << "[-] There was an error while creating the window\n";
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK) {
			std::cout << "[-] There was an error while initializing GLEW\n";
			return -1;
		}
		
		{
			GLCall(glEnable(GL_BLEND));
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

			Renderer renderer;

			ImGui::CreateContext();

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init();
			ImGui::StyleColorsDark();

			test::Test* currentTest = nullptr;
			test::TestMenu* testMenu = new test::TestMenu(currentTest);
			currentTest = testMenu;

			testMenu->RegisterTest<test::TestClearColor>("Clear Color");
			testMenu->RegisterTest<test::TestTexture>("Texture Test");
			testMenu->RegisterTest<test::TestDemo3D>("3D Demo");

			ImGuiIO& io = ImGui::GetIO(); (void)io;

			while (!glfwWindowShouldClose(window)) {
				
				int width, height;
				glfwGetFramebufferSize(window, &width, &height); // Get the window size
				glViewport(0, 0, width, height);  // Set the viewport
				
				renderer.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);

				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				ImGui::Begin("Test");

				if (currentTest) {
					currentTest->OnUpdate(0.0f);
					currentTest->OnRender();

					if (currentTest != testMenu && ImGui::Button("<-")) {
						delete currentTest;
						currentTest = testMenu;
					}
					currentTest->OnImGuiRender();
				}

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

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