#pragma once

#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// Define macros for debugging
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

// GLClearError and GLLogCall are for debugging too
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
	void ClearColor(float r, float g, float b, float a) const;
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};