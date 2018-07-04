#version 450 core

layout(set = 0, binding = 0) uniform ModelViewProjection
{
	mat4 model;
	mat4 view;
	mat4 projection;
} mvp;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

void main()
{
    gl_Position = mvp.model * mvp.view * mvp.projection * vec4(position, 1.0);
    fragColor 	= color;
}