#version 450 core

layout(binding = 0) uniform SceneUniform
{
    mat4 model;
    mat4 view;
    mat4 projection;
} suo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main()
{
    gl_Position = suo.projection * suo.view * suo.model * vec4(inPosition, 0.0, 1.0);
    fragColor 	= inColor;
}