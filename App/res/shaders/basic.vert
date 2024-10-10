#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 1) in vec2 texCoord;

out vec3 v_Color;
out vec2 v_TexCoord;

uniform float scale;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main() {
    gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);
    v_Color = color;
    v_TexCoord = texCoord;
}