#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;

uniform mat4 projection;

out vec3 color;

void main(void)
{
    gl_Position = projection * vec4(in_position, 1.0);
    color = in_color;
}
