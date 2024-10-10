#include "Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"

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

		int windowWidth = 700;
		int windowHeight = 700;

		window = glfwCreateWindow(windowWidth, windowHeight, "Nebula", NULL, NULL);
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

			testMenu->RegisterTest<test::TestClearColor>("Clear Color", window);
			testMenu->RegisterTest<test::TestTexture>("Texture Test", window);
			testMenu->RegisterTest<test::TestDemo3D>("3D Demo", window);

			ImGuiIO& io = ImGui::GetIO(); (void)io;

			bool VSync = false;

			while (!glfwWindowShouldClose(window)) {
				
				renderer.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);

				GLCall(glfwSwapInterval((int)VSync));

				/*
				int width, height;
				glfwGetFramebufferSize(window, &width, &height);
				GLCall(glViewport(0, 0, width, height));
				*/

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