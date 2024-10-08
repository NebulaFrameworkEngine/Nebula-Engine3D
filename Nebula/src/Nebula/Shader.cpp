#include "Shader.h"

#include "Renderer.h"
#include <iostream>
#include <sstream>
#include <fstream>

Shader::Shader(const std::string& vertFile, const std::string& fragFile) : m_vertFile(vertFile), m_fragFile(fragFile), m_RendererID(0) {
	ShaderProgramSource source = ParseShader(vertFile, fragFile);

	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

	if (m_RendererID == 0) {
		std::cout << "[-] Shader program creation failed." << std::endl;
	}
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& vertex_path, const std::string& fragment_path) {
	std::ifstream vertex_stream(vertex_path);
	std::ifstream fragment_stream(fragment_path);

	if (!vertex_stream.is_open()) {
		std::cout << "Error: Failed to open vertex shader file: " << vertex_path << std::endl;
	}
	if (!fragment_stream.is_open()) {
		std::cout << "Error: Failed to open fragment shader file: " << fragment_path << std::endl;
	}

	std::stringstream vertex_ss, fragment_ss;
	vertex_ss << vertex_stream.rdbuf();
	fragment_ss << fragment_stream.rdbuf();

	return { vertex_ss.str(), fragment_ss.str() };
}



unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));

		return 0;
	}

	return id;
}

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}



void Shader::Bind() const {
	if (m_RendererID == 0) {
		std::cout << "[-] Shader program creation failed." << std::endl;
	}
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value) {
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}


int Shader::GetUniformLocation(std::string name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

	if (location == -1) {
		std::cout << "[-] There was an error while handling the uniform" << std::endl;
		std::cout << "[-] " << name << " does not exist" << std::endl;
	}

	m_UniformLocationCache[name] = location;
	return location;

}
